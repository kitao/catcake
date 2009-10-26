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


#include <stdarg.h>

#include "ck_draw_all.h"

#include "ck_util_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckFont::ckFont()
{
    m_tex = NULL;
}


ckFont::~ckFont()
{
    if (ckDrawMgr::isCreated() && m_tex)
    {
        ckDrawMgr::deleteTexture(m_tex->getID());
    }
}


void ckFont::init(u16 width, u16 height)
{
    if (m_tex)
    {
        ckTry
        {
            m_tex->resizeImage(width, height);
        }
        ckCatch(ckTex::ExceptionInvalidArgument)
        {
            ckThrow(ExceptionInvalidArgument);
        }
    }
    else
    {
        ckTry
        {
            m_tex = ckDrawMgr::newTexture(ckID::genID(), width, height, ckTex::FORMAT_ALPHA);
        }
        ckCatch(ckDrawMgr::ExceptionInvalidArgument)
        {
            ckThrow(ExceptionInvalidArgument);
        }
    }

    m_tex->clearImage(ckCol::ZERO);
}


ckID ckFont::getTextureID() const
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->getID();
}


u16 ckFont::getWidth() const
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->getWidth();
}


u16 ckFont::getHeight() const
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->getHeight();
}


const void* ckFont::getImage() const
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->getImage();
}


u32 ckFont::getImageSize() const
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->getImageSize();
}


void* ckFont::editImage()
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->editImage();
}


void ckFont::clearImage()
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->clearImage(ckCol::ZERO);
}


u16 ckFont::drawString(s16 x, s16 y, const char* str, ...)
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    char buf1[256];
    CK_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    ckUtil::charToWchar(buf2, 256, buf1);

    return drawString(x, y, buf2);
}


u16 ckFont::drawString(s16 x, s16 y, const wchar_t* str, ...)
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckDrawMgr* ins = ckDrawMgr::instance();

    if (!ins->m_font_info)
    {
        ckThrow(ExceptionInvalidCall);
    }

    wchar_t buf[256];
    CK_VSWPRINTF(buf, 256, str);

    s32 draw_width = ckLowLevelAPI::drawFreeTypeFont(m_tex->editImage(), m_tex->getWidth(), m_tex->getHeight(), //
        ins->m_font_info, ins->m_font_index, ins->m_font_size, x, y, buf);

    if (draw_width < 0)
    {
        ckThrow(ExceptionDrawStringFailed);
    }

    return draw_width;
}
