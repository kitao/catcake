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

#include "pg_draw_all.h"

#include "pg_util_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgFont::pgFont()
{
    m_tex = NULL;
}


pgFont::~pgFont()
{
    if (pgDrawMgr::isCreated() && m_tex)
    {
        pgDrawMgr::deleteTexture(m_tex->getID());
    }
}


void pgFont::init(u16 width, u16 height)
{
    if (m_tex)
    {
        pgTry
        {
            m_tex->resizeImage(width, height);
        }
        pgCatch(pgTex::ExceptionInvalidArgument)
        {
            pgThrow(ExceptionInvalidArgument);
        }
    }
    else
    {
        pgTry
        {
            m_tex = pgDrawMgr::newTexture(pgID::genID(), width, height, pgTex::FORMAT_ALPHA);
        }
        pgCatch(pgDrawMgr::ExceptionInvalidArgument)
        {
            pgThrow(ExceptionInvalidArgument);
        }
    }

    m_tex->clearImage(pgCol::ZERO);
}


pgID pgFont::getTextureID() const
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->getID();
}


u16 pgFont::getWidth() const
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->getWidth();
}


u16 pgFont::getHeight() const
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->getHeight();
}


const void* pgFont::getImage() const
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->getImage();
}


u32 pgFont::getImageSize() const
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->getImageSize();
}


void* pgFont::editImage()
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->editImage();
}


void pgFont::clearImage()
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_tex->clearImage(pgCol::ZERO);
}


u16 pgFont::drawString(s16 x, s16 y, const char* str, ...)
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    char buf1[256];
    PG_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    pgUtil::charToWchar(buf2, 256, buf1);

    return drawString(x, y, buf2);
}


u16 pgFont::drawString(s16 x, s16 y, const wchar_t* str, ...)
{
    if (!m_tex)
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgDrawMgr* ins = pgDrawMgr::instance();

    if (!ins->m_font_info)
    {
        pgThrow(ExceptionInvalidCall);
    }

    wchar_t buf[256];
    PG_VSWPRINTF(buf, 256, str);

    s32 draw_width = pgLowLevelAPI::drawFreeTypeFont(m_tex->editImage(), m_tex->getWidth(), m_tex->getHeight(), //
        ins->m_font_info, ins->m_font_index, ins->m_font_size, x, y, buf);

    if (draw_width < 0)
    {
        pgThrow(ExceptionDrawStringFailed);
    }

    return draw_width;
}
