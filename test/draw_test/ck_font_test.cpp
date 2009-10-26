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


void ckFontTest()
{
    /*
        ckFont()
        ~ckFont()
        void init(u16 width, u16 height)
        ckID getTextureID() const
        u16 getWidth() const
        u16 getHeight() const
        const void* getImage() const
        u32 getImageSize() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckFont font;

        ckAssertThrow(font.getTextureID(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.getWidth(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.getHeight(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.getImage(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.getImageSize(), ckFont::ExceptionNotInitialized);

        font.init(12, 34);

        ckTex* tex1 = ckDrawMgr::getTexture(font.getTextureID());
        ckAssert(font.getWidth() == 12 && font.getHeight() == 34);
        ckAssert(font.getImage() == tex1->getImage());
        ckAssert(font.getImageSize() == tex1->getImageSize());

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            ckAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        font.init(56, 78);

        ckTex* tex2 = ckDrawMgr::getTexture(font.getTextureID());
        ckAssert(font.getWidth() == 56 && font.getHeight() == 78);
        ckAssert(font.getImage() == tex2->getImage());
        ckAssert(font.getImageSize() == tex2->getImageSize());

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            ckAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        ckAssertThrow(font.init(0, 10), ckFont::ExceptionInvalidArgument);
        ckAssertThrow(font.init(10, 0), ckFont::ExceptionInvalidArgument);
        ckAssertThrow(font.init(ckDrawMgr::getMaxTextureLength() + 1, 10), ckFont::ExceptionInvalidArgument);
        ckAssertThrow(font.init(10, ckDrawMgr::getMaxTextureLength() + 1), ckFont::ExceptionInvalidArgument);

        {
            ckFont font;
            ckAssertThrow(font.init(0, 10), ckFont::ExceptionInvalidArgument);
        }

        {
            ckFont font;
            ckAssertThrow(font.init(10, 0), ckFont::ExceptionInvalidArgument);
        }

        {
            ckFont font;
            ckAssertThrow(font.init(ckDrawMgr::getMaxTextureLength() + 1, 10), ckFont::ExceptionInvalidArgument);
        }

        {
            ckFont font;
            ckAssertThrow(font.init(10, ckDrawMgr::getMaxTextureLength() + 1), ckFont::ExceptionInvalidArgument);
        }

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void* editImage()
        void clearImage()
    */
    {
        ckDrawMgr::createAfterRes();

        ckFont font;

        ckAssertThrow(font.editImage(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.clearImage(), ckFont::ExceptionNotInitialized);

        font.init(12, 34);

        ckMemMgr::memset(font.editImage(), 123, font.getImageSize());

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            ckAssert(*(static_cast<const u8*>(font.getImage()) + i) == 123);
        }

        font.clearImage();

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            ckAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        u16 drawString(s16 x, s16 y, const char* str, ...)
        u16 drawString(s16 x, s16 y, const wchar_t* str, ...)
    */
    {
        ckDrawMgr::createAfterRes();

        ckResMgr::loadResource(TEST_DATA_DIR "adler.ttf", true);
        ckDrawMgr::setFont(ckID_("adler.ttf"), 0);

        ckFont font;

        ckAssertThrow(font.drawString(0, 0, static_cast<const char*>(NULL)), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.drawString(0, 0, static_cast<const wchar_t*>(NULL)), ckFont::ExceptionNotInitialized);

        font.init(12, 34);

        ckAssert(font.drawString(-100, -200, "%d", 123) > 0);
        ckAssert(font.drawString(-100, -200, "TEST %s %d", "MESSAGE", 123) == ckDrawMgr::calcFontDrawWidth("TEST MESSAGE 123"));

        ckAssert(font.drawString(-100, -200, L"%d", 123) > 0);
        ckAssert(font.drawString(-100, -200, L"TEST %ls %d", L"MESSAGE", 123) == ckDrawMgr::calcFontDrawWidth(L"TEST MESSAGE 123")); // TODO: failed on Linux

        ckAssertThrow(font.drawString(0, 0, static_cast<const char*>(NULL)), ckFont::ExceptionInvalidArgument);
        ckAssertThrow(font.drawString(0, 0, static_cast<const wchar_t*>(NULL)), ckFont::ExceptionInvalidArgument);

        ckResMgr::removeResource(ckID_("adler.ttf"));

        ckDrawMgr::destroyBeforeRes();
    }
}
