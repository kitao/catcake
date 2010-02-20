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
        void* beginEditImage()
        void endEditImage()
        void endEditImage(u16 x, u16 y, u16 width, u16 height)
        void clearImage()
    */
    {
        ckDrawMgr::createAfterRes();

        ckFont font;

        ckAssertThrow(font.beginEditImage(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.endEditImage(), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.endEditImage(0, 0, 0, 0), ckFont::ExceptionNotInitialized);
        ckAssertThrow(font.clearImage(), ckFont::ExceptionNotInitialized);

        font.init(12, 34);

        ckMemMgr::memset(font.beginEditImage(), 123, font.getImageSize());
        font.endEditImage();

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            ckAssert(*(static_cast<const u8*>(font.getImage()) + i) == 123);
        }

        font.clearImage();

        for (u32 i = 0; i < font.getImageSize(); i++)
        {
            ckAssert(*(static_cast<const u8*>(font.getImage()) + i) == 0);
        }

        font.beginEditImage();
        font.endEditImage(1, 2, 3, 4);

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
