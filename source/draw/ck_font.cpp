/*
    Copyright (c) 2007-2010 Takashi Kitao

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
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


void* ckFont::beginEditImage()
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->beginEditImage();
}


void ckFont::endEditImage()
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->endEditImage();
}


void ckFont::endEditImage(u16 x, u16 y, u16 width, u16 height)
{
    if (!m_tex)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_tex->endEditImage(x, y, width, height);
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

    ckDrawMgr* ins = ckDrawMgr::instance();

    if (!ins->m_font_info)
    {
        ckThrow(ExceptionInvalidCall);
    }

    char buf1[256];
    CK_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    ckUtil::charToWchar(buf2, 256, buf1);

    s32 draw_width = ckLowLevelAPI::drawFreeTypeFont(m_tex->beginEditImage(), m_tex->getWidth(), m_tex->getHeight(), //
        ins->m_font_info, ins->m_font_index, ins->m_font_size, x, y, buf2);
    m_tex->endEditImage();

    if (draw_width < 0)
    {
        ckThrow(ExceptionDrawStringFailed);
    }

    return draw_width;
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

    s32 draw_width = ckLowLevelAPI::drawFreeTypeFont(m_tex->beginEditImage(), m_tex->getWidth(), m_tex->getHeight(), //
        ins->m_font_info, ins->m_font_index, ins->m_font_size, x, y, buf);
    m_tex->endEditImage();

    if (draw_width < 0)
    {
        ckThrow(ExceptionDrawStringFailed);
    }

    return draw_width;
}
