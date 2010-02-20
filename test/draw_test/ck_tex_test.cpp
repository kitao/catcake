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


void ckTexTest()
{
    /*
        ckTex* getPrevN() const
        ckTex* getNextN() const
        ckID getID() const
        u16 getWidth() const
        u16 getHeight() const
        TexFormat getFormat() const
        TexMode getMode() const
        const void* getImage() const
        u32 getImageSize() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckID id1 = ckID::genID();
        ckID id2 = ckID::genID();
        ckID id3 = ckID::genID();

        ckTex* tex1 = ckDrawMgr::newTexture(id1, 10, 20, ckTex::FORMAT_RGB);
        ckTex* tex2 = ckDrawMgr::newTexture(id2, 30, 40, ckTex::FORMAT_RGBA);
        ckTex* tex3 = ckDrawMgr::newTexture(id3, 50, 60, ckTex::FORMAT_ALPHA);

        ckAssert(tex1->getID() == id1);
        ckAssert(tex2->getID() == id2);
        ckAssert(tex3->getID() == id3);

        ckAssert(ckDrawMgr::getFirstTextureN() == tex1 && ckDrawMgr::getLastTextureN() == tex3);
        ckAssert(!tex1->getPrevN() && tex1->getNextN() == tex2);
        ckAssert(tex2->getPrevN() == tex1 && tex2->getNextN() == tex3);
        ckAssert(tex3->getPrevN() == tex2 && !tex3->getNextN());

        ckAssert(tex1->getWidth() == 10 && tex1->getHeight() == 20);
        ckAssert(tex1->getFormat() == ckTex::FORMAT_RGB && tex1->getMode() == ckTex::MODE_READ_WRITE);
        ckAssert(tex1->getImage() && tex1->getImageSize() == 10 * 20 * 3);

        ckAssert(tex2->getWidth() == 30 && tex2->getHeight() == 40);
        ckAssert(tex2->getFormat() == ckTex::FORMAT_RGBA && tex2->getMode() == ckTex::MODE_READ_WRITE);
        ckAssert(tex2->getImage() && tex2->getImageSize() == 30 * 40 * 4);

        ckAssert(tex3->getWidth() == 50 && tex3->getHeight() == 60);
        ckAssert(tex3->getFormat() == ckTex::FORMAT_ALPHA && tex3->getMode() == ckTex::MODE_READ_WRITE);
        ckAssert(tex3->getImage() && tex3->getImageSize() == 50 * 60 * 1);

        ckDrawMgr::deleteTexture(id2);

        ckAssert(ckDrawMgr::getFirstTextureN() == tex1 && ckDrawMgr::getLastTextureN() == tex3);
        ckAssert(!tex1->getPrevN() && tex1->getNextN() == tex3);
        ckAssert(tex3->getPrevN() == tex1 && !tex3->getNextN());

        ckDrawMgr::deleteTexture(id1);

        ckAssert(ckDrawMgr::getFirstTextureN() == tex3 && ckDrawMgr::getLastTextureN() == tex3);
        ckAssert(!tex3->getPrevN() && !tex3->getNextN());

        ckDrawMgr::deleteTexture(id3);

        ckResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);
        ckResMgr::loadResource(TEST_DATA_DIR "rgba.png", true);
        ckResMgr::loadResource(TEST_DATA_DIR "gray.png", true);
        ckResMgr::loadResource(TEST_DATA_DIR "gray_alpha.png", true);

        ckID id4 = ckID_("rgb.png");
        ckID id5 = ckID_("rgba.png");
        ckID id6 = ckID_("gray.png");
        ckID id7 = ckID_("gray_alpha.png");

        ckTex* tex4 = ckDrawMgr::getTexture(id4);
        ckTex* tex5 = ckDrawMgr::getTexture(id5);
        ckTex* tex6 = ckDrawMgr::getTexture(id6);
        ckTex* tex7 = ckDrawMgr::getTexture(id7);

        ckAssert(tex4->getID() == id4);
        ckAssert(tex4->getWidth() == 71 && tex4->getHeight() == 14);
        ckAssert(tex4->getFormat() == ckTex::FORMAT_PNG_RGB && tex4->getMode() == ckTex::MODE_READ_ONLY);
        ckAssert(tex4->getImage() && tex4->getImageSize() == 324);

        ckAssert(tex5->getID() == id5);
        ckAssert(tex5->getWidth() == 71 && tex5->getHeight() == 14);
        ckAssert(tex5->getFormat() == ckTex::FORMAT_PNG_RGBA && tex5->getMode() == ckTex::MODE_READ_ONLY);
        ckAssert(tex5->getImage() && tex5->getImageSize() == 353);

        ckAssert(tex6->getID() == id6);
        ckAssert(tex6->getWidth() == 71 && tex6->getHeight() == 14);
        ckAssert(tex6->getFormat() == ckTex::FORMAT_PNG_ALPHA && tex6->getMode() == ckTex::MODE_READ_ONLY);
        ckAssert(tex6->getImage() && tex6->getImageSize() == 274);

        ckAssert(tex7->getID() == id7);
        ckAssert(tex7->getWidth() == 71 && tex7->getHeight() == 14);
        ckAssert(tex7->getFormat() == ckTex::FORMAT_PNG_RGBA && tex7->getMode() == ckTex::MODE_READ_ONLY);
        ckAssert(tex7->getImage() && tex7->getImageSize() == 321);

        ckDrawMgr::deleteTexture(id4);

        ckResMgr::removeResource(id4);
        ckResMgr::removeResource(id5);

        ckDrawMgr::destroyBeforeRes();

        ckResMgr::removeResource(id6);
        ckResMgr::removeResource(id7);
    }

    /*
        void* beginEditImage()
        void endEditImage()
        void endEditImage(u16 x, u16 y, u16 width, u16 height)
        void clearImage(ckCol col)
        void resizeImage(u16 width, u16 height)
        void setVolatile()
    */
    {
        ckDrawMgr::createAfterRes();

        ckTex* tex1 = ckDrawMgr::newTexture(ckID::genID(), 10, 11, ckTex::FORMAT_RGB);
        ckTex* tex2 = ckDrawMgr::newTexture(ckID::genID(), 12, 13, ckTex::FORMAT_RGBA);
        ckTex* tex3 = ckDrawMgr::newTexture(ckID::genID(), 14, 15, ckTex::FORMAT_ALPHA);

        ckPrim* prim1 = ckNew(ckPrim);
        ckPrim* prim2 = ckNew(ckPrim);
        ckPrim* prim3 = ckNew(ckPrim);

        prim1->init(ckPrim::MODE_POINTS, 1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        prim2->init(ckPrim::MODE_POINTS, 1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        prim3->init(ckPrim::MODE_POINTS, 1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);

        prim1->setTextureID(tex1->getID());
        prim2->setTextureID(tex2->getID());
        prim3->setTextureID(tex3->getID());

        ckAssertThrow(tex1->endEditImage(), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex1->endEditImage(0, 0, 0, 0), ckTex::ExceptionInvalidCall);

        ckAssert(tex1->getImageSize() == 10 * 11 * 3);
        ckAssert(tex2->getImageSize() == 12 * 13 * 4);
        ckAssert(tex3->getImageSize() == 14 * 15 * 1);

        void* image1 = tex1->beginEditImage();
        void* image2 = tex2->beginEditImage();
        void* image3 = tex3->beginEditImage();
        ckAssert(image1 && image2 && image3);

        ckAssertThrow(tex1->clearImage(ckCol::ZERO), ckTex::ExceptionInvalidCall);
        ckAssertThrow(ckDrawMgr::renderForSystem(), ckTex::ExceptionEndEditImageNotCalled);

        ckAssertThrow(tex1->endEditImage(0, 0, 0, 1), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->endEditImage(0, 0, 1, 0), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->endEditImage(8, 0, 3, 1), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->endEditImage(0, 9, 1, 3), ckTex::ExceptionInvalidArgument);

        ckMemMgr::memset(image1, 255, tex1->getImageSize());
        ckMemMgr::memset(image2, 255, tex2->getImageSize());
        ckMemMgr::memset(image3, 255, tex3->getImageSize());

        tex1->endEditImage();
        tex2->endEditImage();
        tex3->endEditImage(1, 2, 3, 4);

        ckDrawMgr::renderForSystem();

        tex1->clearImage(ckCol(1, 2, 3, 4));
        tex2->clearImage(ckCol(1, 2, 3, 4));
        tex3->clearImage(ckCol(1, 2, 3, 4));

        for (s32 i = 0; i < tex1->getWidth() * tex1->getHeight(); i++)
        {
            const u8* pixel = static_cast<const u8*>(tex1->getImage()) + i * 3;

            ckAssert(*(pixel + 0) == 0x01 && *(pixel + 1) == 0x02 && *(pixel + 2) == 0x03);
        }

        for (s32 i = 0; i < tex2->getWidth() * tex2->getHeight(); i++)
        {
            const u8* pixel = static_cast<const u8*>(tex2->getImage()) + i * 4;

            ckAssert(*(pixel + 0) == 0x01 && *(pixel + 1) == 0x02 && *(pixel + 2) == 0x03 && *(pixel + 3) == 0x04);
        }

        for (s32 i = 0; i < tex3->getWidth() * tex3->getHeight(); i++)
        {
            const u8* pixel = static_cast<const u8*>(tex3->getImage()) + i;

            ckAssert(*pixel == 0x04);
        }

        ckDrawMgr::renderForSystem();

        ckAssertThrow(tex1->resizeImage(0, 10), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->resizeImage(10, 0), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->resizeImage(ckDrawMgr::getMaxTextureLength() + 1, 10), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->resizeImage(10, ckDrawMgr::getMaxTextureLength() + 1), ckTex::ExceptionInvalidArgument);

        tex1->resizeImage(10, 20);
        tex2->resizeImage(30, 40);
        tex3->resizeImage(50, 60);

        ckAssert(tex1->getImageSize() == 10 * 20 * 3);
        ckAssert(tex2->getImageSize() == 30 * 40 * 4);
        ckAssert(tex3->getImageSize() == 50 * 60 * 1);

        image1 = tex1->beginEditImage();
        image2 = tex2->beginEditImage();
        image3 = tex3->beginEditImage();
        ckAssert(image1 && image2 && image3);

        ckMemMgr::memset(image1, 255, tex1->getImageSize());
        ckMemMgr::memset(image2, 255, tex2->getImageSize());
        ckMemMgr::memset(image3, 255, tex3->getImageSize());

        tex1->endEditImage();
        tex2->endEditImage();
        tex3->endEditImage(1, 2, 3, 4);

        ckDrawMgr::renderForSystem();

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        tex1->setVolatile();
        ckAssert(tex1->getMode() == ckTex::MODE_VOLATILE);
        ckAssert(!tex1->getImage() && tex1->getImageSize() == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() < used_memory_size);

        ckAssertThrow(tex1->setVolatile(), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex1->beginEditImage(), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex1->resizeImage(123, 456), ckTex::ExceptionInvalidCall);

        ckResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);
        ckTex* tex4 = ckDrawMgr::getTexture(ckID_("rgb.png"));

        ckAssertThrow(tex4->beginEditImage(), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex4->clearImage(ckCol::ZERO), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex4->resizeImage(123, 456), ckTex::ExceptionInvalidCall);

        used_memory_size = ckMemMgr::getCurUsedMemorySize();

        tex4->setVolatile();
        ckAssert(tex1->getMode() == ckTex::MODE_VOLATILE);
        ckAssert(!tex1->getImage() && tex1->getImageSize() == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckResMgr::removeResource(ckID_("rgb.png"));

        ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        scr->attachScreenTexture(ckTex::FORMAT_RGB);
        ckTex* tex5 = ckDrawMgr::getTexture(scr->getScreenTextureID());

        ckAssert(!tex5->getImage() && tex5->getImageSize() == 0);

        ckAssertThrow(tex5->beginEditImage(), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex5->clearImage(ckCol::ZERO), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex5->resizeImage(123, 456), ckTex::ExceptionInvalidCall);
        ckAssertThrow(tex5->setVolatile(), ckTex::ExceptionInvalidCall);

        ckDrawMgr::renderForSystem();

        ckDelete(prim1, ckPrim);
        ckDelete(prim2, ckPrim);
        ckDelete(prim3, ckPrim);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        ckID getProxyTextureID() const
        void setProxyTextureID(ckID tex_id)
    */
    {
        ckDrawMgr::createAfterRes();

        ckTex* tex1 = ckDrawMgr::newTexture(ckID::genID(), 10, 20, ckTex::FORMAT_RGB);
        ckTex* tex2 = ckDrawMgr::newTexture(ckID::genID(), 30, 40, ckTex::FORMAT_RGBA);

        ckAssert(tex1->getProxyTextureID() == ckID::ZERO);

        tex1->setProxyTextureID(tex2->getID());
        ckAssert(tex1->getProxyTextureID() == tex2->getID());

        tex1->setProxyTextureID(ckID::ZERO);
        ckAssert(tex1->getProxyTextureID() == ckID::ZERO);

        ckAssertThrow(tex1->setProxyTextureID(tex1->getID()), ckTex::ExceptionInvalidArgument);
        ckAssertThrow(tex1->setProxyTextureID(ckID::genID()), ckDrawMgr::ExceptionNotFound);

        ckDrawMgr::destroyBeforeRes();
    }
}
