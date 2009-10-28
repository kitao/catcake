/*
    Copyright (c) 2007-2009 Takashi Kitao
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    `  notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
    `  notice, this list of conditions and the following disclaimer in the
    `  documentation and/or other materials provided with the distribution.

    3. The name of the author may not be used to endorse or promote products
    `  derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "ck_draw_all.h"

#include "ck_util_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckTex* ckTex::getPrevN() const
{
    ckDrawMgr* ins = ckDrawMgr::instance();
    const ckID* id = ins->m_tex_map.getPrevKeyN(m_id);

    return id ? *ins->m_tex_map.get(*id) : NULL;
}


ckTex* ckTex::getNextN() const
{
    ckDrawMgr* ins = ckDrawMgr::instance();
    const ckID* id = ins->m_tex_map.getNextKeyN(m_id);

    return id ? *ins->m_tex_map.get(*id) : NULL;
}


ckID ckTex::getID() const
{
    return m_id;
}


u16 ckTex::getWidth() const
{
    return m_width;
}


u16 ckTex::getHeight() const
{
    return m_height;
}


ckTex::TexFormat ckTex::getFormat() const
{
    return m_format.getType();
}


ckTex::TexMode ckTex::getMode() const
{
    return m_mode.getType();
}


const void* ckTex::getImage() const
{
    return m_image;
}


u32 ckTex::getImageSize() const
{
    return m_image_size;
}


void* ckTex::editImage()
{
    if (m_mode != MODE_READ_WRITE)
    {
        ckThrow(ExceptionInvalidCall);
    }

    m_flag.setEnabled(ckTex::FLAG_UPLOAD);

    return const_cast<void*>(m_image);
}


void ckTex::clearImage(ckCol col)
{
    u8* image = static_cast<u8*>(editImage());

    switch (m_format.getType())
    {
    case FORMAT_RGB:
        {
            u32 pixel_num = m_width * m_height;
            u8* pixel = static_cast<u8*>(const_cast<void*>(m_image));

            for (u32 i = 0; i < pixel_num; i++)
            {
                *(pixel + 0) = col.r;
                *(pixel + 1) = col.g;
                *(pixel + 2) = col.b;

                pixel += 3;
            }
        }
        break;

    case FORMAT_RGBA:
        {
            u32 pixel_num = m_width * m_height;
            u8* pixel = static_cast<u8*>(const_cast<void*>(m_image));

            for (u32 i = 0; i < pixel_num; i++)
            {
                *(pixel + 0) = col.r;
                *(pixel + 1) = col.g;
                *(pixel + 2) = col.b;
                *(pixel + 3) = col.a;

                pixel += 4;
            }
        }
        break;

    case FORMAT_ALPHA:
        ckMemMgr::memset(image, col.a, m_width * m_height);
        break;

    default:
        break;
    }
}


void ckTex::resizeImage(u16 width, u16 height)
{
    if (m_mode != MODE_READ_WRITE)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (width == 0 || height == 0 || width > ckDrawMgr::getMaxTextureLength() || height > ckDrawMgr::getMaxTextureLength())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_width = width;
    m_height = height;

    u16 valid_width = ckDrawMgr::getValidTextureLength(width);
    u16 valid_height = ckDrawMgr::getValidTextureLength(height);

    if (width != valid_width || height != valid_height)
    {
        m_flag.setEnabled(FLAG_UV_ADJUST);

        m_u_param_a = static_cast<r32>(width) / valid_width;
        m_u_param_b = 0.0f;

        m_v_param_a = static_cast<r32>(height) / valid_height;
        m_v_param_b = 0.0f;
    }
    else
    {
        m_flag.setDisabled(FLAG_UV_ADJUST);

        m_u_param_a = 1.0f;
        m_u_param_b = 0.0f;

        m_v_param_a = 1.0f;
        m_v_param_b = 0.0f;
    }

    if (m_image)
    {
        ckFree(const_cast<void*>(m_image));
    }

    m_image_size = ckDrawMgr::getTexturePixelSize(m_format.getType()) * m_width * m_height;
    m_image = ckMalloc(m_image_size);

    m_flag.setEnabled(ckTex::FLAG_UPLOAD);
}


void ckTex::setVolatile()
{
    if (m_mode != MODE_READ_ONLY && m_mode != MODE_READ_WRITE)
    {
        ckThrow(ExceptionInvalidCall);
    }

    getTexObj(); // upload the image to the vram

    if (m_mode == MODE_READ_WRITE)
    {
        ckFree(const_cast<void*>(m_image));
        m_image = NULL;
        m_image_size = 0;
    }

    m_mode = MODE_VOLATILE;
}


ckID ckTex::getProxyTextureID() const
{
    return m_proxy_tex ? m_proxy_tex->getID() : ckID::ZERO;
}


void ckTex::setProxyTextureID(ckID tex_id)
{
    if (tex_id == m_id)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_proxy_tex = (tex_id == ckID::ZERO) ? NULL : ckDrawMgr::getTexture(tex_id);
}


ckTex::ckTex(ckID tex_id, u16 width, u16 height, TexFormat format, TexMode mode, const void* image, u32 image_size)
{
    m_id = tex_id;
    m_width = width;
    m_height = height;
    m_format = format;
    m_mode = mode;
    m_tex_obj = 0;
    m_proxy_tex = NULL;

    m_flag.clear();

    u16 valid_width = ckDrawMgr::getValidTextureLength(width);
    u16 valid_height = ckDrawMgr::getValidTextureLength(height);

    if (width != valid_width || height != valid_height)
    {
        m_flag.setEnabled(FLAG_UV_ADJUST);

        m_u_param_a = static_cast<r32>(width) / valid_width;
        m_u_param_b = 0.0f;

        m_v_param_a = static_cast<r32>(height) / valid_height;
        m_v_param_b = 0.0f;
    }
    else
    {
        m_u_param_a = 1.0f;
        m_u_param_b = 0.0f;

        m_v_param_a = 1.0f;
        m_v_param_b = 0.0f;
    }

    switch (m_mode.getType())
    {
    case MODE_READ_ONLY:
        m_image = image;
        m_image_size = image_size;
        break;

    case MODE_READ_WRITE:
        m_image_size = ckDrawMgr::getTexturePixelSize(m_format.getType()) * m_width * m_height;
        m_image = ckMalloc(m_image_size);
        break;

    case MODE_FRAMEBUFFER:
        m_image = NULL;
        m_image_size = 0;

        m_flag.setEnabled(FLAG_UV_ADJUST);

        m_v_param_a = -m_v_param_a;
        m_v_param_b = 1.0f;
        break;

    default:
        ckThrow(ExceptionInvalidArgument);
    }

    m_flag.setEnabled(ckTex::FLAG_UPLOAD);

    ckDrawMgr::instance()->m_tex_map.add(tex_id, this);
}


ckTex::~ckTex()
{
    ckDrawMgr::instance()->m_tex_map.remove(m_id);

    if (m_tex_obj != 0)
    {
        ckLowLevelAPI::unregisterTexture(m_tex_obj);
    }

    if (m_mode == MODE_READ_WRITE)
    {
        ckFree(const_cast<void*>(m_image));
    }
}


CK_DEFINE_OPERATOR_EQUAL(ckTex)


u32 ckTex::getTexObj()
{
    if (m_flag.isEnabled(FLAG_UPLOAD))
    {
        if (m_mode == MODE_VOLATILE)
        {
            ckThrow(ExceptionVolatileTextureUploaded);
        }
        else if (m_mode != MODE_FRAMEBUFFER)
        {
            if (m_tex_obj != 0)
            {
                ckLowLevelAPI::unregisterTexture(m_tex_obj);
            }

            if (m_format == FORMAT_PNG_RGB || m_format == FORMAT_PNG_RGBA || m_format == FORMAT_PNG_ALPHA)
            {
                expandAndRegisterTexture_png();
            }
            else if (m_flag.isEnabled(FLAG_UV_ADJUST))
            {
                expandAndRegisterTexture_ptx();
            }
            else
            {
                m_tex_obj = ckLowLevelAPI::registerTexture( //
                    m_width, m_height, static_cast<ckLowLevelAPI::TextureFormat>(m_format.getType()), m_image);
            }
        }

        m_flag.setDisabled(FLAG_UPLOAD);
    }

    return m_tex_obj;
}


void ckTex::expandAndRegisterTexture_ptx()
{
    u16 valid_width = ckDrawMgr::getValidTextureLength(m_width);
    u16 valid_height = ckDrawMgr::getValidTextureLength(m_height);
    u16 pixel_size = ckDrawMgr::getTexturePixelSize(m_format.getType());
    u16 src_line_size = m_width * pixel_size;
    u16 dest_line_size = valid_width * pixel_size;

    void* new_image = ckMalloc(valid_width * valid_height * pixel_size);

    const u8* src = static_cast<const u8*>(m_image);
    u8* dest = static_cast<u8*>(new_image);

    for (u32 i = 0; i < m_height; i++)
    {
        ckMemMgr::memcpy(dest, src, src_line_size);

        src += src_line_size - pixel_size;
        dest += src_line_size;

        for (u32 j = m_width; j < valid_width; j++)
        {
            ckMemMgr::memcpy(dest, src, pixel_size);

            dest += pixel_size;
        }

        src += pixel_size;
    }

    src = dest - dest_line_size;

    for (u32 i = m_height; i < valid_height; i++)
    {
        ckMemMgr::memcpy(dest, src, dest_line_size);

        dest += dest_line_size;
    }

    m_tex_obj = ckLowLevelAPI::registerTexture( //
        valid_width, valid_height, static_cast<ckLowLevelAPI::TextureFormat>(m_format.getType()), new_image);

    ckFree(new_image);
}


void ckTex::expandAndRegisterTexture_png()
{
    u16 valid_width = ckDrawMgr::getValidTextureLength(m_width);
    u16 valid_height = ckDrawMgr::getValidTextureLength(m_height);
    u16 pixel_size = ckDrawMgr::getTexturePixelSize(m_format.getType());
    u16 src_line_size = m_width * pixel_size;
    u16 dest_line_size = valid_width * pixel_size;

    void* new_image = ckMalloc(valid_width * valid_height * pixel_size);

    ckUtil::readPNGImage(new_image, valid_width * valid_height * pixel_size, dest_line_size, m_image, m_image_size);

    u8* src = static_cast<u8*>(new_image) + src_line_size - pixel_size;
    u8* dest = src + pixel_size;

    if (m_width < valid_width)
    {
        for (u32 i = 0; i < m_height; i++)
        {
            for (u32 j = m_width; j < valid_width; j++)
            {
                ckMemMgr::memcpy(dest, src, pixel_size);

                dest += pixel_size;
            }

            src += dest_line_size;
            dest += src_line_size;
        }
    }

    src = static_cast<u8*>(new_image) + dest_line_size * (m_height - 1);
    dest = src + dest_line_size;

    for (u32 i = m_height; i < valid_height; i++)
    {
        ckMemMgr::memcpy(dest, src, dest_line_size);

        dest += dest_line_size;
    }

    m_tex_obj = ckLowLevelAPI::registerTexture( //
        valid_width, valid_height, static_cast<ckLowLevelAPI::TextureFormat>(m_format.getType() - FORMAT_PNG_RGB), new_image);

    ckFree(new_image);
}
