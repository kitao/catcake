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


#include "test.h"


void pgFontTest()
{
    /*
        pgFont()
        ~pgFont()
        void init(u16 width, u16 height)
        pgID getTextureID() const
        u16 getWidth() const
        u16 getHeight() const
        const void* getImage() const
        u32 getImageSize() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgFont font;

        pgAssertThrow(font.getTextureID(), pgFont::ExceptionNotInitialized);
        pgAssertThrow(font.getWidth(), pgFont::ExceptionNotInitialized);
        pgAssertThrow(font.getHeight(), pgFont::ExceptionNotInitialized);
        pgAssertThrow(font.getImage(), pgFont::ExceptionNotInitialized);
        pgAssertThrow(font.getImageSize(), pgFont::ExceptionNotInitialized);

        font.init(12, 34);

        pgTex* tex1 = pgDrawMgr::getTexture(font.getTextureID());
        pgAssert(font.getWidth() == 12 && font.getHeight() == 34);
        pgAssert(font.getImage() == tex1->getImage());
        pgAssert(font.getImageSize() == tex1->getImageSize());

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            pgAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        font.init(56, 78);

        pgTex* tex2 = pgDrawMgr::getTexture(font.getTextureID());
        pgAssert(font.getWidth() == 56 && font.getHeight() == 78);
        pgAssert(font.getImage() == tex2->getImage());
        pgAssert(font.getImageSize() == tex2->getImageSize());

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            pgAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        pgAssertThrow(font.init(0, 10), pgFont::ExceptionInvalidArgument);
        pgAssertThrow(font.init(10, 0), pgFont::ExceptionInvalidArgument);
        pgAssertThrow(font.init(pgDrawMgr::getMaxTextureLength() + 1, 10), pgFont::ExceptionInvalidArgument);
        pgAssertThrow(font.init(10, pgDrawMgr::getMaxTextureLength() + 1), pgFont::ExceptionInvalidArgument);

        {
            pgFont font;
            pgAssertThrow(font.init(0, 10), pgFont::ExceptionInvalidArgument);
        }

        {
            pgFont font;
            pgAssertThrow(font.init(10, 0), pgFont::ExceptionInvalidArgument);
        }

        {
            pgFont font;
            pgAssertThrow(font.init(pgDrawMgr::getMaxTextureLength() + 1, 10), pgFont::ExceptionInvalidArgument);
        }

        {
            pgFont font;
            pgAssertThrow(font.init(10, pgDrawMgr::getMaxTextureLength() + 1), pgFont::ExceptionInvalidArgument);
        }

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void* editImage()
        void clearImage()
    */
    {
        pgDrawMgr::createAfterRes();

        pgFont font;

        pgAssertThrow(font.editImage(), pgFont::ExceptionNotInitialized);
        pgAssertThrow(font.clearImage(), pgFont::ExceptionNotInitialized);

        font.init(12, 34);

        pgMemMgr::memset(font.editImage(), 123, font.getImageSize());

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            pgAssert(*(static_cast<const u8*>(font.getImage()) + i) == 123);
        }

        font.clearImage();

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            pgAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        u16 drawString(s16 x, s16 y, const char* str, ...)
        u16 drawString(s16 x, s16 y, const wchar_t* str, ...)
    */
    {
        pgDrawMgr::createAfterRes();

        pgResMgr::loadResource(TEST_DATA_DIR "adler.ttf", true);
        pgDrawMgr::setFont(pgID_("adler.ttf"), 0);

        pgFont font;

        pgAssertThrow(font.drawString(0, 0, static_cast<const char*>(NULL)), pgFont::ExceptionNotInitialized);
        pgAssertThrow(font.drawString(0, 0, static_cast<const wchar_t*>(NULL)), pgFont::ExceptionNotInitialized);

        font.init(12, 34);

        pgAssert(font.drawString(-100, -200, "%d", 123) > 0);
        pgAssert(font.drawString(-100, -200, "TEST %s %d", "MESSAGE", 123) == pgDrawMgr::calcFontDrawWidth("TEST MESSAGE 123"));

        pgAssert(font.drawString(-100, -200, L"%d", 123) > 0);
        pgAssert(font.drawString(-100, -200, L"TEST %ls %d", L"MESSAGE", 123) == pgDrawMgr::calcFontDrawWidth(L"TEST MESSAGE 123")); // TODO: failed on Linux

        pgAssertThrow(font.drawString(0, 0, static_cast<const char*>(NULL)), pgFont::ExceptionInvalidArgument);
        pgAssertThrow(font.drawString(0, 0, static_cast<const wchar_t*>(NULL)), pgFont::ExceptionInvalidArgument);

        pgResMgr::removeResource(pgID_("adler.ttf"));

        pgDrawMgr::destroyBeforeRes();
    }
}
