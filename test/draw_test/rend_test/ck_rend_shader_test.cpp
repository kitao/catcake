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


void ckRend_ShaderTest()
{
    /*
        void init(ckPrim* prim, ckID shd_id)
        void init(ckPrim* prim, r32* rend_data, ckID shd_id)
        ckID getShaderID()
        ckID get2ndTextureID()
        void set2ndTextureID(ckID tex_id)
        ckID get3rdTextureID()
        void set3rdTextureID(ckID tex_id)
        r32& uniform(u8 uni_index)
        void setUniformVec(u8 uni_index, const ckVec& vec)
        void setUniformCol3(u8 uni_index, ckCol col)
        void setUniformCol4(u8 uni_index, ckCol col)
        r32& attrib(u16 data_index, u8 att_index)
        void setAttribVec(u16 data_index, u8 att_index, const ckVec& vec)
        void setAttribCol3(u16 data_index, u8 att_index, ckCol col)
        void setAttribCol4(u16 data_index, u8 att_index, ckCol col)
    */
    {
        static const char s_vert_code[] = "void main() { gl_Position = 0.0; }";
        static const char s_frag_code[] = "void main() { gl_FragColor = 1.0; }";

        ckDrawMgr::createAfterRes();
        ckDrawMgr::newTexture(ckID_("TEST"), 16, 16, ckTex::FORMAT_RGB);

        ckShd* shd1 = ckDrawMgr::newShader(ckID::genID(), s_vert_code, s_frag_code, 0, 0, 0);
        ckShd* shd2 = ckDrawMgr::newShader(ckID::genID(), s_vert_code, s_frag_code, 4, 5, 0);

        ckRend_Shader rend_shd;

        ckAssertThrow(rend_shd.getShaderID(), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.get2ndTextureID(), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.set2ndTextureID(ckID::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.get3rdTextureID(), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.set3rdTextureID(ckID::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.uniform(0), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.setUniformVec(0, ckVec::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.setUniformCol3(0, ckCol::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.setUniformCol4(0, ckCol::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.attrib(0, 0), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.setAttribVec(0, 0, ckVec::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.setAttribCol3(0, 0, ckCol::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_shd.setAttribCol4(0, 0, ckCol::ZERO), ckRend::ExceptionNotInitialized);

        ckPrim prim;

        prim.init(ckPrim::MODE_LINES, 2, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_shd.init(&prim, shd1->getID());

        ckAssert(rend_shd.getShaderID() == shd1->getID());
        ckAssert(rend_shd.get2ndTextureID() == ckID::ZERO);
        ckAssert(rend_shd.get3rdTextureID() == ckID::ZERO);

        ckAssertThrow(rend_shd.uniform(0), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setUniformVec(0, ckVec::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setUniformCol3(0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setUniformCol4(0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.attrib(2, 0), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.attrib(0, 0), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribVec(2, 0, ckVec::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribVec(0, 0, ckVec::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol3(2, 0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol3(2, 0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol4(0, 0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol4(0, 0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);

        ckDrawMgr::renderForSystem();

        prim.init(ckPrim::MODE_LINES, 3, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_shd.init(&prim, shd2->getID());

        ckAssert(rend_shd.getShaderID() == shd2->getID());
        ckAssert(rend_shd.get2ndTextureID() == ckID::ZERO);
        ckAssert(rend_shd.get3rdTextureID() == ckID::ZERO);

        rend_shd.set2ndTextureID(ckID::ZERO);
        ckAssert(rend_shd.get2ndTextureID() == ckID::ZERO);

        rend_shd.set3rdTextureID(ckID::ZERO);
        ckAssert(rend_shd.get3rdTextureID() == ckID::ZERO);

        for (s32 i = 0; i < 4; i++)
        {
            rend_shd.uniform(0) = static_cast<r32>(i);
            ckAssert(isEqual(rend_shd.uniform(0), static_cast<r32>(i)));
        }

        rend_shd.setUniformVec(1, ckVec(10.0f, 20.0f, 30.0f));
        ckAssert(isEqual(rend_shd.uniform(1), 10.0f) && isEqual(rend_shd.uniform(2), 20.0f) && isEqual(rend_shd.uniform(3), 30.0f));

        rend_shd.setUniformCol3(1, ckCol(11, 22, 33));
        ckAssert(isEqual(rend_shd.uniform(1), 11.0f) && isEqual(rend_shd.uniform(2), 22.0f) && isEqual(rend_shd.uniform(3), 33.0f));

        rend_shd.setUniformCol4(0, ckCol(1, 2, 3, 4));
        ckAssert(isEqual(rend_shd.uniform(0), 1.0f) && isEqual(rend_shd.uniform(1), 2.0f) && //
            isEqual(rend_shd.uniform(2), 3.0f) && isEqual(rend_shd.uniform(3), 4.0f));

        for (s32 i = 0; i < 3; i++)
        {
            for (s32 j = 0; j < 5; j++)
            {
                rend_shd.attrib(i, j) = static_cast<r32>(i + j);
                ckAssert(isEqual(rend_shd.attrib(i, j), static_cast<r32>(i + j)));
            }

            rend_shd.setAttribVec(i, 2, ckVec(10.0f + i, 11.0f + i, 12.0f + i));
            ckAssert(isEqual(rend_shd.attrib(i, 2), 10.0f + i) && isEqual(rend_shd.attrib(i, 3), 11.0f + i) && isEqual(rend_shd.attrib(i, 4), 12.0f + i));

            rend_shd.setAttribCol3(i, 2, ckCol(i + 2, i + 3, i + 4));
            ckAssert(isEqual(rend_shd.attrib(i, 2), 2.0f + i) && isEqual(rend_shd.attrib(i, 3), 3.0f + i) && isEqual(rend_shd.attrib(i, 4), 4.0f + i));

            rend_shd.setAttribCol4(i, 1, ckCol(i + 5, i + 6, i + 7, i + 8));
            ckAssert(isEqual(rend_shd.attrib(i, 1), 5.0f + i) && isEqual(rend_shd.attrib(i, 2), 6.0f + i) && //
                isEqual(rend_shd.attrib(i, 3), 7.0f + i) && isEqual(rend_shd.attrib(i, 4), 8.0f + i));
        }

        ckAssertThrow(rend_shd.uniform(4), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setUniformVec(2, ckVec::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setUniformCol3(2, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setUniformCol4(1, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.attrib(3, 0), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.attrib(0, 5), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribVec(3, 0, ckVec::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribVec(0, 3, ckVec::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol3(3, 0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol3(0, 3, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol4(3, 0, ckCol::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.setAttribCol4(0, 2, ckCol::ZERO), ckRend::ExceptionInvalidArgument);

        ckDrawMgr::renderForSystem();

        ckAssertThrow(rend_shd.set2ndTextureID(ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_shd.set3rdTextureID(ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_shd.init(NULL, shd1->getID()), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.init(&prim, ckID::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.init(&prim, ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_shd.init(&prim, NULL, shd2->getID()), ckRend::ExceptionInvalidCall);

        ckPrim::PrimData prim_data[3];
        r32 rend_data[3];

        prim.init(ckPrim::MODE_LINES, prim_data, 3, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_shd.init(&prim, rend_data, shd2->getID());

        ckAssertThrow(rend_shd.init(NULL, rend_data, shd2->getID()), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.init(&prim, NULL, shd2->getID()), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.init(&prim, rend_data, ckID::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_shd.init(&prim, rend_data, ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_shd.init(&prim, shd2->getID()), ckRend::ExceptionInvalidCall);

        ckDrawMgr::destroyBeforeRes();
    }
}
