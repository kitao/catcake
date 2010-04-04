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


void ckUtilTest()
{
    /*
        static u32 strlen(const char* str)
        static u32 wcslen(const wchar_t* str)
    */
    {
        ckAssert(ckUtil::strlen("") == 0);
        ckAssert(ckUtil::strlen("TEST") == 4);

        ckAssert(ckUtil::wcslen(L"") == 0);
        ckAssert(ckUtil::wcslen(L"TEST") == 4);

        ckAssertThrow(ckUtil::strlen(NULL), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::wcslen(NULL), ckUtil::ExceptionInvalidArgument);
    }

    /*
        static void charToWchar(wchar_t* buf, u32 buf_size, const char* str)
    */
    {
        wchar_t buf1[5], buf2[3];

        ckUtil::charToWchar(buf1, 5, "TEST");
        ckAssert(buf1[0] == L'T' && buf1[1] == L'E' && buf1[2] == L'S' && buf1[3] == L'T' && buf1[4] == L'\0');

        ckUtil::charToWchar(buf2, 3, "TEST");
        ckAssert(buf2[0] == L'T' && buf2[1] == L'E' && buf2[2] == L'\0');

        ckAssertThrow(ckUtil::charToWchar(NULL, 5, "TEST"), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::charToWchar(buf1, 0, "TEST"), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::charToWchar(buf1, 5, NULL), ckUtil::ExceptionInvalidArgument);
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

        ckAssert(ckUtil::getBasename(str1) == str1 + 0);
        ckAssert(ckUtil::getBasename(str2) == str2 + 0);
        ckAssert(ckUtil::getBasename(str3) == str3 + 2);
        ckAssert(ckUtil::getBasename(str4) == str4 + 7);
        ckAssert(ckUtil::getBasename(str5) == str5 + 11);

        ckAssert(ckUtil::getExtension(str1) == str1 + 4);
        ckAssert(ckUtil::getExtension(str2) == str2 + 4);
        ckAssert(ckUtil::getExtension(str3) == str3 + 6);
        ckAssert(ckUtil::getExtension(str4) == str4 + 11);
        ckAssert(ckUtil::getExtension(str5) == str5 + 15);

        ckAssertThrow(ckUtil::getBasename(NULL), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::getExtension(NULL), ckUtil::ExceptionInvalidArgument);
    }

    /*
        static void calcNormalAsTriangles(ckVec* normal, const ckPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing)
    */
    {
        ckPrim::PrimData prim_data[7];
        ckVec normal[7];

        prim_data[0].pos.set(0.0f, 0.0f, 0.0f);
        prim_data[1].pos.set(0.0f, 0.0f, 10.0f);
        prim_data[2].pos.set(10.0f, 0.0f, 10.0f);
        prim_data[3].pos.set(10.0f, 0.0f, 10.0f);
        prim_data[4].pos.set(0.0f, 0.0f, 10.0f);
        prim_data[5].pos.set(30.0f, 30.0f, 10.0f);
        prim_data[6].pos.set(30.0f, 30.0f, 10.0f);

        ckUtil::calcNormalAsTriangles(normal, prim_data, 2, false);
        ckAssert(isEqual(normal[0], ckVec::Z_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 2, true);
        ckAssert(isEqual(normal[0], ckVec::Z_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 3, false);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 3, true);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 5, false);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[3], ckVec::Z_UNIT));
        ckAssert(isEqual(normal[4], ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 5, true);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[3], ckVec::Z_UNIT));
        ckAssert(isEqual(normal[4], ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 6, false);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[3], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[4], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[5], -ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 6, true);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[3], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[4], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[5], -ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 7, false);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[3], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[4], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[5], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[6], ckVec::Z_UNIT));

        ckUtil::calcNormalAsTriangles(normal, prim_data, 7, true);
        ckAssert(isEqual(normal[0], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[1], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[2], ckVec::Y_UNIT));
        ckAssert(isEqual(normal[3], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[4], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[5], -ckVec::Z_UNIT));
        ckAssert(isEqual(normal[6], ckVec::Z_UNIT));

        ckAssertThrow(ckUtil::calcNormalAsTriangles(NULL, prim_data, 1, false), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::calcNormalAsTriangles(normal, NULL, 1, false), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::calcNormalAsTriangles(normal, prim_data, 0, false), ckUtil::ExceptionInvalidArgument);
    }

    /*
        static bool readPNGInfo(u16* width, u16* height, ckTex::TexFormat* format, const void* data, u32 data_size)
        static bool readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size)
    */
    {
        u16 width, height;
        ckTex::TexFormat format;
        ckRes res;

        ckResMgr::loadResource(TEST_DATA_DIR "rgb.png", true);
        ckResMgr::loadResource(TEST_DATA_DIR "rgba.png", true);
        ckResMgr::loadResource(TEST_DATA_DIR "gray.png", true);
        ckResMgr::loadResource(TEST_DATA_DIR "gray_alpha.png", true);

        res = ckResMgr::getResource(ckID_("rgb.png"));

        ckUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        ckAssert(width == 71 && height == 14 && format == ckTex::FORMAT_PNG_RGB);

        res = ckResMgr::getResource(ckID_("rgba.png"));

        ckUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        ckAssert(width == 71 && height == 14 && format == ckTex::FORMAT_PNG_RGBA);

        res = ckResMgr::getResource(ckID_("gray.png"));

        ckUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        ckAssert(width == 71 && height == 14 && format == ckTex::FORMAT_PNG_ALPHA);

        res = ckResMgr::getResource(ckID_("gray_alpha.png"));

        ckUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), res.getDataSize());
        ckAssert(width == 71 && height == 14 && format == ckTex::FORMAT_PNG_RGBA);

        u32 buf_line_size = width * ckDrawMgr::getTexturePixelSize(format);
        u32 buf_size = buf_line_size;
        void* buf = ckMalloc(buf_size);

        ckUtil::readPNGImage(buf, buf_size, buf_line_size, res.getData<void>(), res.getDataSize());

        ckAssertThrow(ckUtil::readPNGInfo(NULL, &height, &format, res.getData<void>(), res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGInfo(&width, NULL, &format, res.getData<void>(), res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGInfo(&width, &height, NULL, res.getData<void>(), res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGInfo(&width, &height, &format, NULL, res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGInfo(&width, &height, &format, res.getData<void>(), 0), ckUtil::ExceptionInvalidArgument);

        ckAssertThrow(ckUtil::readPNGImage(NULL, buf_size, buf_line_size, res.getData<void>(), res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGImage(buf, 0, buf_line_size, res.getData<void>(), res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGImage(buf, buf_size, 0, res.getData<void>(), res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGImage(buf, buf_size, buf_line_size, NULL, res.getDataSize()), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::readPNGImage(buf, buf_size, buf_line_size, res.getData<void>(), 0), ckUtil::ExceptionInvalidArgument);

        ckResMgr::removeResource(ckID_("rgb.png"));
        ckResMgr::removeResource(ckID_("rgba.png"));
        ckResMgr::removeResource(ckID_("gray.png"));
        ckResMgr::removeResource(ckID_("gray_alpha.png"));

        ckFree(buf);
    }

    /*
        static void loadWindowsFont(const char* filename)
    */
    {
#ifdef WIN32
        ckUtil::loadWindowsFont("Arial.ttf");
        ckResMgr::removeResource(ckID_("Arial.ttf"));
#endif
        ckAssertThrow(ckUtil::loadWindowsFont(NULL), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::loadWindowsFont("dummy_font_name"), ckResMgr::ExceptionCannotOpenFile);
    }

    /*
        static void loadShader(ckID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num)
    */
    {
        ckID id = ckID::genID();

        ckUtil::loadShader(id, TEST_DATA_DIR "test.vert", TEST_DATA_DIR "test.frag", 100, 100, 3);
        ckAssert(ckDrawMgr::getShader(id));

        ckDrawMgr::deleteShader(id);
    }

    /*
        static void loadPixelArtModelAs(ckID res_id, const char* ckl_file, const char* png_file, r32 scale)
    */
    {
        ckRes res;
        ckMdlData mdl_data;

        ckUtil::loadPixelArtModelAs(ckID_("valid_model.mdl"), TEST_DATA_DIR "valid_model.ckl", TEST_DATA_DIR "valid_model.png", 10.0f);

        res = ckResMgr::getResource(ckID_("valid_model.mdl"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        ckAssert(mdl_data.getNodeNum() == 2);
        ckAssert(mdl_data.getVertNum() == 14 * 6 * 6 * 2);
        ckAssert(mdl_data.hasNormal());
        ckAssert(mdl_data.getTextureID() == ckID::ZERO);

        ckAssert(mdl_data.getNodeParentIndex(0) == 1 && mdl_data.getNodeParentIndex(1) == 1);
        ckAssert(isEqual(mdl_data.getNodeLocal(0), ckMat::UNIT.translate(10.0f, 20.0f, 30.0f)) && isEqual(mdl_data.getNodeLocal(1), ckMat::UNIT));
        ckAssert(mdl_data.getNodePrimMode(0) == ckPrim::MODE_TRIANGLES && mdl_data.getNodePrimMode(1) == ckPrim::MODE_TRIANGLES);
        ckAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 504);
        ckAssert(mdl_data.getNodeVertNum(0) == 504 && mdl_data.getNodeVertNum(1) == 504);
        ckAssert(mdl_data.getNodeBlendMode(0) == ckDraw::BLEND_OFF && mdl_data.getNodeBlendMode(1) == ckDraw::BLEND_OFF);

        ckAssert(isEqual(mdl_data.getVertPos(0), ckVec(30.01f, 10.01f, -60.01f)));
        ckAssert(mdl_data.getVertCol(0) == ckCol(0, 0, 0));
        ckAssert(isEqual(mdl_data.getVertU(0), 0.0f) && isEqual(mdl_data.getVertV(0), 0.0f));

        ckResMgr::removeResource(ckID_("valid_model.mdl"));

        ckAssertThrow(ckUtil::loadPixelArtModelAs(ckID::ZERO, TEST_DATA_DIR "valid_model.ckl", TEST_DATA_DIR "valid_model.png", 0.0f), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::loadPixelArtModelAs(ckID::genID(), TEST_DATA_DIR "valid_model.ckl", TEST_DATA_DIR "valid_model.png", 0.0f), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::loadPixelArtModelAs(ckID::genID(), TEST_DATA_DIR "invalid_model.ckl", TEST_DATA_DIR "valid_model.png", 1.0f), ckUtil::ExceptionInvalidData);
        ckAssertThrow(ckUtil::loadPixelArtModelAs(ckID::genID(), TEST_DATA_DIR "valid_model.ckl", TEST_DATA_DIR "invalid_model1.png", 1.0f), ckUtil::ExceptionInvalidData);
        ckAssertThrow(ckUtil::loadPixelArtModelAs(ckID::genID(), TEST_DATA_DIR "valid_model.ckl", TEST_DATA_DIR "invalid_model2.png", 1.0f), ckUtil::ExceptionInvalidData);
    }

    /*
        static void import3DSModel(const char* filename, ckID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
        static void import3DSModelAs(ckID res_id, const char* filename, ckID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
    */
    {
        ckRes res;
        ckMdlData mdl_data;

        ckUtil::import3DSModel(TEST_DATA_DIR "valid.3ds", ckID_("test_tex"), false, false, 1.0f);

        res = ckResMgr::getResource(ckID_("valid.3ds"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        ckAssert(mdl_data.getNodeNum() == 3);
        ckAssert(mdl_data.getVertNum() == 108);
        ckAssert(!mdl_data.hasNormal());
        ckAssert(mdl_data.getTextureID() == ckID_("test_tex"));

        ckAssert(mdl_data.getNodeParentIndex(0) == 0 && mdl_data.getNodeParentIndex(1) == 0 && mdl_data.getNodeParentIndex(2) == 0);
        ckAssert(isEqual(mdl_data.getNodeLocal(0), ckMat::UNIT) && isEqual(mdl_data.getNodeLocal(1), ckMat::UNIT) && isEqual(mdl_data.getNodeLocal(2), ckMat::UNIT));
        ckAssert(mdl_data.getNodePrimMode(0) == ckPrim::MODE_TRIANGLES && mdl_data.getNodePrimMode(1) == ckPrim::MODE_TRIANGLES && mdl_data.getNodePrimMode(2) == ckPrim::MODE_TRIANGLES);
        ckAssert(mdl_data.getNodeVertIndex(0) == 0 && mdl_data.getNodeVertIndex(1) == 36 && mdl_data.getNodeVertIndex(2) == 72);
        ckAssert(mdl_data.getNodeVertNum(0) == 36 && mdl_data.getNodeVertNum(1) == 36 && mdl_data.getNodeVertNum(2) == 36);
        ckAssert(mdl_data.getNodeBlendMode(0) == ckDraw::BLEND_OFF && mdl_data.getNodeBlendMode(1) == ckDraw::BLEND_HALF && mdl_data.getNodeBlendMode(2) == ckDraw::BLEND_ADD);

        ckAssert(isEqual(mdl_data.getVertPos(0), ckVec(75.0f, -75.0f, 150.0f)) && isEqual(mdl_data.getVertPos(36), ckVec(-125.0f, -75.0f, 150.0f)) && isEqual(mdl_data.getVertPos(72), ckVec(-125.0f, -75.0f, 350.0f)));
        ckAssert(mdl_data.getVertCol(0) == ckCol::FULL && mdl_data.getVertCol(36) == ckCol::FULL && mdl_data.getVertCol(72) == ckCol::FULL);
        ckAssert( //
            isEqual(mdl_data.getVertU(0), 0.0f) && isEqual(mdl_data.getVertV(0), 0.0f) && //
            isEqual(mdl_data.getVertU(36), 0.0f) && isEqual(mdl_data.getVertV(36), 0.0f) && //
            isEqual(mdl_data.getVertU(72), 0.0f) && isEqual(mdl_data.getVertV(72), 0.0f));

        ckResMgr::removeResource(ckID_("valid.3ds"));

        ckUtil::import3DSModel(TEST_DATA_DIR "valid.3ds", ckID::ZERO, true, true, 2.0f);

        res = ckResMgr::getResource(ckID_("valid.3ds"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        ckAssert(mdl_data.hasNormal());
        ckAssert(isEqual(mdl_data.getVertPos(0), ckVec(150.0f, -150.0f, 300.0f)) && isEqual(mdl_data.getVertPos(36), ckVec(-250.0f, -150.0f, 300.0f)) && isEqual(mdl_data.getVertPos(72), ckVec(-250.0f, -150.0f, 700.0f)));
        ckAssert(isEqual(mdl_data.getVertN(0), ckVec(0.0f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(36), ckVec(0.f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(72), ckVec(0.f, -1.0f, 0.0f)));

        ckResMgr::removeResource(ckID_("valid.3ds"));

        ckUtil::import3DSModelAs(ckID_("test"), TEST_DATA_DIR "valid.3ds", ckID::ZERO, true, false, 3.0f);

        res = ckResMgr::getResource(ckID_("test"));
        mdl_data.initAsReader(res.getData<void>(), res.getDataSize());

        ckAssert(mdl_data.hasNormal());
        ckAssert(isEqual(mdl_data.getVertN(0), ckVec(0.0f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(36), ckVec(0.f, -1.0f, 0.f)) && isEqual(mdl_data.getVertN(72), ckVec(0.f, -1.0f, 0.0f)));

        ckResMgr::removeResource(ckID_("test"));

        ckAssertThrow(ckUtil::import3DSModel(TEST_DATA_DIR "valid.3ds", ckID::ZERO, false, true, 1.0f), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::import3DSModel(TEST_DATA_DIR "invalid.3ds", ckID::ZERO, false, false, 1.0f), ckUtil::ExceptionInvalidData);
        ckAssertThrow(ckUtil::import3DSModel(TEST_DATA_DIR "valid.3ds", ckID::ZERO, false, false, 0.0f), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::import3DSModelAs(ckID::genID(), TEST_DATA_DIR "valid.3ds", ckID::ZERO, false, true, 1.0f), ckUtil::ExceptionInvalidArgument);
        ckAssertThrow(ckUtil::import3DSModelAs(ckID::genID(), TEST_DATA_DIR "invalid.3ds", ckID::ZERO, false, false, 1.0f), ckUtil::ExceptionInvalidData);
        ckAssertThrow(ckUtil::import3DSModelAs(ckID::genID(), TEST_DATA_DIR "valid.3ds", ckID::ZERO, false, false, 0.0f), ckUtil::ExceptionInvalidArgument);
    }
}
