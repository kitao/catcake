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


void pgTexTest()
{
    /*
        pgTex* getPrevN() const
        pgTex* getNextN() const
        pgID getID() const
        u16 getWidth() const
        u16 getHeight() const
        TexFormat getFormat() const
        TexMode getMode() const
        const void* getImage() const
        u32 getImageSize() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgID id1 = pgID::genID();
        pgID id2 = pgID::genID();
        pgID id3 = pgID::genID();

        pgTex* tex1 = pgDrawMgr::newTexture(id1, 10, 20, pgTex::FORMAT_RGB);
        pgTex* tex2 = pgDrawMgr::newTexture(id2, 30, 40, pgTex::FORMAT_RGBA);
        pgTex* tex3 = pgDrawMgr::newTexture(id3, 50, 60, pgTex::FORMAT_ALPHA);

        pgAssert(tex1->getID() == id1);
        pgAssert(tex2->getID() == id2);
        pgAssert(tex3->getID() == id3);

        pgAssert(pgDrawMgr::getFirstTextureN() == tex1 && pgDrawMgr::getLastTextureN() == tex3);
        pgAssert(!tex1->getPrevN() && tex1->getNextN() == tex2);
        pgAssert(tex2->getPrevN() == tex1 && tex2->getNextN() == tex3);
        pgAssert(tex3->getPrevN() == tex2 && !tex3->getNextN());

        pgAssert(tex1->getWidth() == 10 && tex1->getHeight() == 20);
        pgAssert(tex1->getFormat() == pgTex::FORMAT_RGB && tex1->getMode() == pgTex::MODE_READ_WRITE);
        pgAssert(tex1->getImage() && tex1->getImageSize() == 10 * 20 * 3);

        pgAssert(tex2->getWidth() == 30 && tex2->getHeight() == 40);
        pgAssert(tex2->getFormat() == pgTex::FORMAT_RGBA && tex2->getMode() == pgTex::MODE_READ_WRITE);
        pgAssert(tex2->getImage() && tex2->getImageSize() == 30 * 40 * 4);

        pgAssert(tex3->getWidth() == 50 && tex3->getHeight() == 60);
        pgAssert(tex3->getFormat() == pgTex::FORMAT_ALPHA && tex3->getMode() == pgTex::MODE_READ_WRITE);
        pgAssert(tex3->getImage() && tex3->getImageSize() == 50 * 60 * 1);

        pgDrawMgr::deleteTexture(id2);

        pgAssert(pgDrawMgr::getFirstTextureN() == tex1 && pgDrawMgr::getLastTextureN() == tex3);
        pgAssert(!tex1->getPrevN() && tex1->getNextN() == tex3);
        pgAssert(tex3->getPrevN() == tex1 && !tex3->getNextN());

        pgDrawMgr::deleteTexture(id1);

        pgAssert(pgDrawMgr::getFirstTextureN() == tex3 && pgDrawMgr::getLastTextureN() == tex3);
        pgAssert(!tex3->getPrevN() && !tex3->getNextN());

        pgDrawMgr::deleteTexture(id3);

        pgResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);
        pgResMgr::loadResource(TEST_DATA_DIR "rgba.png", true);
        pgResMgr::loadResource(TEST_DATA_DIR "gray.png", true);
        pgResMgr::loadResource(TEST_DATA_DIR "gray_alpha.png", true);

        pgID id4 = pgID_("rgb.png");
        pgID id5 = pgID_("rgba.png");
        pgID id6 = pgID_("gray.png");
        pgID id7 = pgID_("gray_alpha.png");

        pgTex* tex4 = pgDrawMgr::getTexture(id4);
        pgTex* tex5 = pgDrawMgr::getTexture(id5);
        pgTex* tex6 = pgDrawMgr::getTexture(id6);
        pgTex* tex7 = pgDrawMgr::getTexture(id7);

        pgAssert(tex4->getID() == id4);
        pgAssert(tex4->getWidth() == 71 && tex4->getHeight() == 14);
        pgAssert(tex4->getFormat() == pgTex::FORMAT_PNG_RGB && tex4->getMode() == pgTex::MODE_READ_ONLY);
        pgAssert(tex4->getImage() && tex4->getImageSize() == 316);

        pgAssert(tex5->getID() == id5);
        pgAssert(tex5->getWidth() == 71 && tex5->getHeight() == 14);
        pgAssert(tex5->getFormat() == pgTex::FORMAT_PNG_RGBA && tex5->getMode() == pgTex::MODE_READ_ONLY);
        pgAssert(tex5->getImage() && tex5->getImageSize() == 358);

        pgAssert(tex6->getID() == id6);
        pgAssert(tex6->getWidth() == 71 && tex6->getHeight() == 14);
        pgAssert(tex6->getFormat() == pgTex::FORMAT_PNG_ALPHA && tex6->getMode() == pgTex::MODE_READ_ONLY);
        pgAssert(tex6->getImage() && tex6->getImageSize() == 270);

        pgAssert(tex7->getID() == id7);
        pgAssert(tex7->getWidth() == 71 && tex7->getHeight() == 14);
        pgAssert(tex7->getFormat() == pgTex::FORMAT_PNG_RGBA && tex7->getMode() == pgTex::MODE_READ_ONLY);
        pgAssert(tex7->getImage() && tex7->getImageSize() == 307);

        pgDrawMgr::deleteTexture(id4);

        pgResMgr::removeResource(id4);
        pgResMgr::removeResource(id5);

        pgDrawMgr::destroyBeforeRes();

        pgResMgr::removeResource(id6);
        pgResMgr::removeResource(id7);
    }

    /*
        void* editImage()
        void clearImage(pgCol col)
        void resizeImage(u16 width, u16 height)
        void setVolatile()
    */
    {
        pgDrawMgr::createAfterRes();

        pgTex* tex1 = pgDrawMgr::newTexture(pgID::genID(), 10, 11, pgTex::FORMAT_RGB);
        pgTex* tex2 = pgDrawMgr::newTexture(pgID::genID(), 12, 13, pgTex::FORMAT_RGBA);
        pgTex* tex3 = pgDrawMgr::newTexture(pgID::genID(), 14, 15, pgTex::FORMAT_ALPHA);

        pgAssert(tex1->getImageSize() == 10 * 11 * 3);
        pgAssert(tex2->getImageSize() == 12 * 13 * 4);
        pgAssert(tex3->getImageSize() == 14 * 15 * 1);

        void* image1 = tex1->editImage();
        void* image2 = tex2->editImage();
        void* image3 = tex3->editImage();
        pgAssert(image1 && image2 && image3);

        pgMemMgr::memset(image1, 255, tex1->getImageSize());
        pgMemMgr::memset(image2, 255, tex2->getImageSize());
        pgMemMgr::memset(image3, 255, tex3->getImageSize());

        pgDrawMgr::renderForSystem();

        tex1->clearImage(pgCol(1, 2, 3, 4));
        tex2->clearImage(pgCol(1, 2, 3, 4));
        tex3->clearImage(pgCol(1, 2, 3, 4));

        for (s32 i = 0; i < tex1->getWidth() * tex1->getHeight(); i++)
        {
            const u8* pixel = static_cast<const u8*>(tex1->getImage()) + i * 3;

            pgAssert(*(pixel + 0) == 0x01 && *(pixel + 1) == 0x02 && *(pixel + 2) == 0x03);
        }

        for (s32 i = 0; i < tex2->getWidth() * tex2->getHeight(); i++)
        {
            const u8* pixel = static_cast<const u8*>(tex2->getImage()) + i * 4;

            pgAssert(*(pixel + 0) == 0x01 && *(pixel + 1) == 0x02 && *(pixel + 2) == 0x03 && *(pixel + 3) == 0x04);
        }

        for (s32 i = 0; i < tex3->getWidth() * tex3->getHeight(); i++)
        {
            const u8* pixel = static_cast<const u8*>(tex3->getImage()) + i;

            pgAssert(*pixel == 0x04);
        }

        pgDrawMgr::renderForSystem();

        pgAssertThrow(tex1->resizeImage(0, 10), pgTex::ExceptionInvalidArgument);
        pgAssertThrow(tex1->resizeImage(10, 0), pgTex::ExceptionInvalidArgument);
        pgAssertThrow(tex1->resizeImage(pgDrawMgr::getMaxTextureLength() + 1, 10), pgTex::ExceptionInvalidArgument);
        pgAssertThrow(tex1->resizeImage(10, pgDrawMgr::getMaxTextureLength() + 1), pgTex::ExceptionInvalidArgument);

        tex1->resizeImage(10, 20);
        tex2->resizeImage(30, 40);
        tex3->resizeImage(50, 60);

        pgAssert(tex1->getImageSize() == 10 * 20 * 3);
        pgAssert(tex2->getImageSize() == 30 * 40 * 4);
        pgAssert(tex3->getImageSize() == 50 * 60 * 1);

        image1 = tex1->editImage();
        image2 = tex2->editImage();
        image3 = tex3->editImage();
        pgAssert(image1 && image2 && image3);

        pgMemMgr::memset(image1, 255, tex1->getImageSize());
        pgMemMgr::memset(image2, 255, tex2->getImageSize());
        pgMemMgr::memset(image3, 255, tex3->getImageSize());

        pgDrawMgr::renderForSystem();

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        tex1->setVolatile();
        pgAssert(tex1->getMode() == pgTex::MODE_VOLATILE);
        pgAssert(!tex1->getImage() && tex1->getImageSize() == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() < used_memory_size);

        pgAssertThrow(tex1->setVolatile(), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex1->editImage(), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex1->resizeImage(123, 456), pgTex::ExceptionInvalidCall);

        pgResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);
        pgTex* tex4 = pgDrawMgr::getTexture(pgID_("rgb.png"));

        pgAssertThrow(tex4->editImage(), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex4->clearImage(pgCol::ZERO), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex4->resizeImage(123, 456), pgTex::ExceptionInvalidCall);

        used_memory_size = pgMemMgr::getCurUsedMemorySize();

        tex4->setVolatile();
        pgAssert(tex1->getMode() == pgTex::MODE_VOLATILE);
        pgAssert(!tex1->getImage() && tex1->getImageSize() == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgResMgr::removeResource(pgID_("rgb.png"));

        pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        scr->attachScreenTexture(pgTex::FORMAT_RGB);
        pgTex* tex5 = pgDrawMgr::getTexture(scr->getScreenTextureID());

        pgAssert(!tex5->getImage() && tex5->getImageSize() == 0);

        pgAssertThrow(tex5->editImage(), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex5->clearImage(pgCol::ZERO), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex5->resizeImage(123, 456), pgTex::ExceptionInvalidCall);
        pgAssertThrow(tex5->setVolatile(), pgTex::ExceptionInvalidCall);

        pgDrawMgr::renderForSystem();

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        pgID getProxyTextureID() const
        void setProxyTextureID(pgID tex_id)
    */
    {
        pgDrawMgr::createAfterRes();

        pgTex* tex1 = pgDrawMgr::newTexture(pgID::genID(), 10, 20, pgTex::FORMAT_RGB);
        pgTex* tex2 = pgDrawMgr::newTexture(pgID::genID(), 30, 40, pgTex::FORMAT_RGBA);

        pgAssert(tex1->getProxyTextureID() == pgID::ZERO);

        tex1->setProxyTextureID(tex2->getID());
        pgAssert(tex1->getProxyTextureID() == tex2->getID());

        tex1->setProxyTextureID(pgID::ZERO);
        pgAssert(tex1->getProxyTextureID() == pgID::ZERO);

        pgAssertThrow(tex1->setProxyTextureID(tex1->getID()), pgTex::ExceptionInvalidArgument);
        pgAssertThrow(tex1->setProxyTextureID(pgID::genID()), pgDrawMgr::ExceptionNotFound);

        pgDrawMgr::destroyBeforeRes();
    }
}
