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


void pgUtilTest()
{
    /*
        static u32 strlen(const char* str)
        static u32 wcslen(const wchar_t* str)
    */
    {
        pgAssert(pgUtil::strlen("") == 0);
        pgAssert(pgUtil::strlen("TEST") == 4);

        pgAssert(pgUtil::wcslen(L"") == 0);
        pgAssert(pgUtil::wcslen(L"TEST") == 4);

        pgAssertThrow(pgUtil::strlen(NULL), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::wcslen(NULL), pgUtil::ExceptionInvalidArgument);
    }

    /*
        static void charToWchar(wchar_t* buf, u32 buf_size, const char* str)
    */
    {
        wchar_t buf1[5], buf2[3];

        pgUtil::charToWchar(buf1, 5, "TEST");
        pgAssert(buf1[0] == L'T' && buf1[1] == L'E' && buf1[2] == L'S' && buf1[3] == L'T' && buf1[4] == L'\0');

        pgUtil::charToWchar(buf2, 3, "TEST");
        pgAssert(buf2[0] == L'T' && buf2[1] == L'E' && buf2[2] == L'\0');

        pgAssertThrow(pgUtil::charToWchar(NULL, 5, "TEST"), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::charToWchar(buf1, 0, "TEST"), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::charToWchar(buf1, 5, NULL), pgUtil::ExceptionInvalidArgument);
    }

    /*
        static const char* getBasename(const char* filename)
        static const char* getExtension(const char* filename)
    */
    {
        const char* str1 = "TEST";
        const char* str2 = "ABC.DEF";
        const char* str3 = "C:abc.def";
        const char* str4 = "C:/ABC/DEF.GHI";
        const char* str5 = "C:\\abc\\def\\ghi.jkl";

        pgAssert(pgUtil::getBasename(str1) == str1 + 0);
        pgAssert(pgUtil::getBasename(str2) == str2 + 0);
        pgAssert(pgUtil::getBasename(str3) == str3 + 2);
        pgAssert(pgUtil::getBasename(str4) == str4 + 7);
        pgAssert(pgUtil::getBasename(str5) == str5 + 11);

        pgAssert(pgUtil::getExtension(str1) == str1 + 4);
        pgAssert(pgUtil::getExtension(str2) == str2 + 4);
        pgAssert(pgUtil::getExtension(str3) == str3 + 6);
        pgAssert(pgUtil::getExtension(str4) == str4 + 11);
        pgAssert(pgUtil::getExtension(str5) == str5 + 15);

        pgAssertThrow(pgUtil::getBasename(NULL), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::getExtension(NULL), pgUtil::ExceptionInvalidArgument);
    }

    /*
        static void calcNormalAsTriangles(pgVec* normal, const pgPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing)
    */
    {
        pgPrim::PrimData prim_data[7];
        pgVec normal[7];

        prim_data[0].pos.set(0.0f, 0.0f, 0.0f);
        prim_data[1].pos.set(0.0f, 0.0f, 10.0f);
        prim_data[2].pos.set(10.0f, 0.0f, 10.0f);
        prim_data[3].pos.set(10.0f, 0.0f, 10.0f);
        prim_data[4].pos.set(0.0f, 0.0f, 10.0f);
        prim_data[5].pos.set(30.0f, 30.0f, 10.0f);
        prim_data[6].pos.set(30.0f, 30.0f, 10.0f);

        pgUtil::calcNormalAsTriangles(normal, prim_data, 2, false);
        pgAssert(isEqual(normal[0], pgVec::Z_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 2, true);
        pgAssert(isEqual(normal[0], pgVec::Z_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 3, false);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 3, true);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 5, false);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[3], pgVec::Z_UNIT));
        pgAssert(isEqual(normal[4], pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 5, true);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[3], pgVec::Z_UNIT));
        pgAssert(isEqual(normal[4], pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 6, false);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[3], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[4], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[5], -pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 6, true);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[3], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[4], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[5], -pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 7, false);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[3], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[4], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[5], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[6], pgVec::Z_UNIT));

        pgUtil::calcNormalAsTriangles(normal, prim_data, 7, true);
        pgAssert(isEqual(normal[0], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[1], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[2], pgVec::Y_UNIT));
        pgAssert(isEqual(normal[3], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[4], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[5], -pgVec::Z_UNIT));
        pgAssert(isEqual(normal[6], pgVec::Z_UNIT));

        pgAssertThrow(pgUtil::calcNormalAsTriangles(NULL, prim_data, 1, false), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::calcNormalAsTriangles(normal, NULL, 1, false), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::calcNormalAsTriangles(normal, prim_data, 0, false), pgUtil::ExceptionInvalidArgument);
    }

    /*
        static bool readPNGInfo(u16* width, u16* height, pgTex::TexFormat* format, const void* data, u32 data_size)
        static bool readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size)
    */
    {
        u16 width, height;
        pgTex::TexFormat format;
        pgRes res;

        pgResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);
        pgResMgr::loadResource(TEST_DATA_DIR "rgba.png", true);
        pgResMgr::loadResource(TEST_DATA_DIR "gray.png", true);
        pgResMgr::loadResource(TEST_DATA_DIR "gray_alpha.png", true);

        res = pgResMgr::getResource(pgID_("rgb.png"));

        pgUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        pgAssert(width == 71 && height == 14 && format == pgTex::FORMAT_PNG_RGB);

        res = pgResMgr::getResource(pgID_("rgba.png"));

        pgUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        pgAssert(width == 71 && height == 14 && format == pgTex::FORMAT_PNG_RGBA);

        res = pgResMgr::getResource(pgID_("gray.png"));

        pgUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        pgAssert(width == 71 && height == 14 && format == pgTex::FORMAT_PNG_ALPHA);

        res = pgResMgr::getResource(pgID_("gray_alpha.png"));

        pgUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        pgAssert(width == 71 && height == 14 && format == pgTex::FORMAT_PNG_RGBA);

        u32 buf_line_size = width * pgDrawMgr::getTexturePixelSize(format);
        u32 buf_size = buf_line_size;
        void* buf = pgMalloc(buf_size);

        pgUtil::readPNGImage(buf, buf_size, buf_line_size, res.getData<void>(), res.getDataSize());

        pgAssertThrow(pgUtil::readPNGInfo(NULL, &height, &format, res.getData<void>(), res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGInfo(&width, NULL, &format, res.getData<void>(), res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGInfo(&width, &height, NULL, res.getData<void>(), res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGInfo(&width, &height, &format, NULL, res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), 0), pgUtil::ExceptionInvalidArgument);

        pgAssertThrow(pgUtil::readPNGImage(NULL, buf_size, buf_line_size, res.getData<void>(), res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGImage(buf, 0, buf_line_size, res.getData<void>(), res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGImage(buf, buf_size, 0, res.getData<void>(), res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGImage(buf, buf_size, buf_line_size, NULL, res.getDataSize()), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::readPNGImage(buf, buf_size, buf_line_size, res.getData<void>(), 0), pgUtil::ExceptionInvalidArgument);

        pgResMgr::removeResource(pgID_("rgb.png"));
        pgResMgr::removeResource(pgID_("rgba.png"));
        pgResMgr::removeResource(pgID_("gray.png"));
        pgResMgr::removeResource(pgID_("gray_alpha.png"));

        pgFree(buf);
    }

    /*
        static void loadWindowsFont(const char* filename)
    */
    {
#ifdef WIN32
        pgUtil::loadWindowsFont("Arial.ttf");
        pgResMgr::removeResource(pgID_("Arial.ttf"));
#endif
        pgAssertThrow(pgUtil::loadWindowsFont(NULL), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::loadWindowsFont("dummy_font_name"), pgResMgr::ExceptionCannotOpenFile);
    }

    /*
        static void loadShader(pgID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num)
    */
    {
        pgID id = pgID::genID();

        pgUtil::loadShader(id, TEST_DATA_DIR "test.vert", TEST_DATA_DIR "test.frag", 100, 100, 3);
        pgAssert(pgDrawMgr::getShader(id));

        pgDrawMgr::deleteShader(id);
    }

    /*
        static void loadPixelArtModel(const char* pxm_file, const char* png_file, r32 scale)
        static void loadPixelArtModelAs(pgID res_id, const char* pxm_file, const char* png_file, r32 scale)
    */
    {
        pgRes res;
        pgMdlData mdl_data;

        pgUtil::loadPixelArtModel(TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "valid_pxm_image.png", 10.0f);

        res = pgResMgr::getResource(pgID_("valid.pxm"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgAssert(mdl_data.getNodeNum() == 2);
        pgAssert(mdl_data.getVertNum() == 14 * 6 * 6 * 2);
        pgAssert(mdl_data.hasNormal());
        pgAssert(mdl_data.getTextureID() == pgID::ZERO);

        pgAssert(mdl_data.getNodeParentIndex(0) == 1 && mdl_data.getNodeParentIndex(1) == 1);
        pgAssert(isEqual(mdl_data.getNodeLocal(0), pgMat::UNIT.translate(10.0f, 20.0f, 30.0f)) && isEqual(mdl_data.getNodeLocal(1), pgMat::UNIT));
        pgAssert(mdl_data.getNodePrimMode(0) == pgPrim::MODE_TRIANGLES && mdl_data.getNodePrimMode(1) == pgPrim::MODE_TRIANGLES);
        pgAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 504);
        pgAssert(mdl_data.getNodeVertNum(0) == 504 && mdl_data.getNodeVertNum(1) == 504);
        pgAssert(mdl_data.getNodeBlendMode(0) == pgDraw::BLEND_OFF && mdl_data.getNodeBlendMode(1) == pgDraw::BLEND_OFF);

        pgAssert(isEqual(mdl_data.getVertPos(0), pgVec(30.01f, 10.01f, -60.01f)));
        pgAssert(mdl_data.getVertCol(0) == pgCol(0, 0, 0));
        pgAssert(isEqual(mdl_data.getVertU(0), 0.0f) && isEqual(mdl_data.getVertV(0), 0.0f));

        pgResMgr::removeResource(pgID_("valid.pxm"));

        pgUtil::loadPixelArtModelAs(pgID_("test"), TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "valid_pxm_image.png", 1.0f);

        res = pgResMgr::getResource(pgID_("test"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgResMgr::removeResource(pgID_("test"));

        pgAssertThrow(pgUtil::loadPixelArtModel(TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "valid_pxm_image.png", 0.0f), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::loadPixelArtModel(TEST_DATA_DIR "invalid.pxm", TEST_DATA_DIR "valid_pxm_image.png", 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::loadPixelArtModel(TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "invalid_pxm_image1.png", 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::loadPixelArtModel(TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "invalid_pxm_image2.png", 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::loadPixelArtModelAs(pgID::genID(), TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "valid_pxm_image.png", 0.0f), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::loadPixelArtModelAs(pgID::genID(), TEST_DATA_DIR "invalid.pxm", TEST_DATA_DIR "valid_pxm_image.png", 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::loadPixelArtModelAs(pgID::genID(), TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "invalid_pxm_image1.png", 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::loadPixelArtModelAs(pgID::genID(), TEST_DATA_DIR "valid.pxm", TEST_DATA_DIR "invalid_pxm_image2.png", 1.0f), pgUtil::ExceptionInvalidData);
    }

    /*
        static void import3DS(const char* filename, pgID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
        static void import3DSAs(pgID res_id, const char* filename, pgID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
    */
    {
        pgRes res;
        pgMdlData mdl_data;

        pgUtil::import3DS(TEST_DATA_DIR "valid.3ds", pgID_("test_tex"), false, false, 1.0f);

        res = pgResMgr::getResource(pgID_("valid.3ds"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgAssert(mdl_data.getNodeNum() == 3);
        pgAssert(mdl_data.getVertNum() == 108);
        pgAssert(!mdl_data.hasNormal());
        pgAssert(mdl_data.getTextureID() == pgID_("test_tex"));

        pgAssert(mdl_data.getNodeParentIndex(0) == 0 && mdl_data.getNodeParentIndex(1) == 0 && mdl_data.getNodeParentIndex(2) == 0);
        pgAssert(isEqual(mdl_data.getNodeLocal(0), pgMat::UNIT) && isEqual(mdl_data.getNodeLocal(1), pgMat::UNIT) && isEqual(mdl_data.getNodeLocal(2), pgMat::UNIT));
        pgAssert(mdl_data.getNodePrimMode(0) == pgPrim::MODE_TRIANGLES && mdl_data.getNodePrimMode(1) == pgPrim::MODE_TRIANGLES && mdl_data.getNodePrimMode(2) == pgPrim::MODE_TRIANGLES);
        pgAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 36 && mdl_data.getNodeVertIndex(2) == 72);
        pgAssert(mdl_data.getNodeVertNum(0) == 36 && mdl_data.getNodeVertNum(1) == 36 && mdl_data.getNodeVertNum(2) == 36);
        pgAssert(mdl_data.getNodeBlendMode(0) == pgDraw::BLEND_OFF && mdl_data.getNodeBlendMode(1) == pgDraw::BLEND_HALF && mdl_data.getNodeBlendMode(2) == pgDraw::BLEND_ADD);

        pgAssert(isEqual(mdl_data.getVertPos(0), pgVec(75.0f, -75.0f, 150.0f)) && isEqual(mdl_data.getVertPos(36), pgVec(-125.0f, -75.0f, 150.0f)) && isEqual(mdl_data.getVertPos(72), pgVec(-125.0f, -75.0f, 350.0f)));
        pgAssert(mdl_data.getVertCol(0) == pgCol::FULL && mdl_data.getVertCol(36) == pgCol::FULL && mdl_data.getVertCol(72) == pgCol::FULL);
        pgAssert( //
            isEqual(mdl_data.getVertU(0), 0.0f) && isEqual(mdl_data.getVertV(0), 0.0f) && //
            isEqual(mdl_data.getVertU(36), 0.0f) && isEqual(mdl_data.getVertV(36), 0.0f) && //
            isEqual(mdl_data.getVertU(72), 0.0f) && isEqual(mdl_data.getVertV(72), 0.0f));

        pgResMgr::removeResource(pgID_("valid.3ds"));

        pgUtil::import3DS(TEST_DATA_DIR "valid.3ds", pgID::ZERO, true, true, 2.0f);

        res = pgResMgr::getResource(pgID_("valid.3ds"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgAssert(mdl_data.hasNormal());
        pgAssert(isEqual(mdl_data.getVertPos(0), pgVec(150.0f, -150.0f, 300.0f)) && isEqual(mdl_data.getVertPos(36), pgVec(-250.0f, -150.0f, 300.0f)) && isEqual(mdl_data.getVertPos(72), pgVec(-250.0f, -150.0f, 700.0f)));
        pgAssert(isEqual(mdl_data.getVertN(0), pgVec(0.0f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(36), pgVec(0.f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(72), pgVec(0.f, -1.0f, 0.0f)));

        pgResMgr::removeResource(pgID_("valid.3ds"));

        pgUtil::import3DSAs(pgID_("test"), TEST_DATA_DIR "valid.3ds", pgID::ZERO, true, false, 3.0f);

        res = pgResMgr::getResource(pgID_("test"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgAssert(mdl_data.hasNormal());
        pgAssert(isEqual(mdl_data.getVertN(0), pgVec(0.0f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(36), pgVec(0.f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(72), pgVec(0.f, -1.0f, 0.0f)));

        pgResMgr::removeResource(pgID_("test"));

        pgAssertThrow(pgUtil::import3DS(TEST_DATA_DIR "valid.3ds", pgID::ZERO, false, true, 1.0f), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::import3DS(TEST_DATA_DIR "invalid.3ds", pgID::ZERO, false, false, 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::import3DS(TEST_DATA_DIR "valid.3ds", pgID::ZERO, false, false, 0.0f), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::import3DSAs(pgID::genID(), TEST_DATA_DIR "valid.3ds", pgID::ZERO, false, true, 1.0f), pgUtil::ExceptionInvalidArgument);
        pgAssertThrow(pgUtil::import3DSAs(pgID::genID(), TEST_DATA_DIR "invalid.3ds", pgID::ZERO, false, false, 1.0f), pgUtil::ExceptionInvalidData);
        pgAssertThrow(pgUtil::import3DSAs(pgID::genID(), TEST_DATA_DIR "valid.3ds", pgID::ZERO, false, false, 0.0f), pgUtil::ExceptionInvalidArgument);
    }
}
