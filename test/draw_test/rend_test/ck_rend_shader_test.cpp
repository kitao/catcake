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


void pgRend_ShaderTest()
{
    /*
        void init(pgPrim* prim, pgID shd_id)
        void init(pgPrim* prim, r32* rend_data, pgID shd_id)
        pgID getShaderID()
        pgID get2ndTextureID()
        void set2ndTextureID(pgID tex_id)
        pgID get3rdTextureID()
        void set3rdTextureID(pgID tex_id)
        r32& uniform(u8 uni_index)
        void setUniformVec(u8 uni_index, const pgVec& vec)
        void setUniformCol3x255(u8 uni_index, pgCol col)
        void setUniformCol4x255(u8 uni_index, pgCol col)
        r32& attrib(u16 data_index, u8 att_index)
        void setAttribVec(u16 data_index, u8 att_index, const pgVec& vec)
        void setAttribCol3x255(u16 data_index, u8 att_index, pgCol col)
        void setAttribCol4x255(u16 data_index, u8 att_index, pgCol col)
    */
    {
        static const char s_vert_code[] = "void main() { gl_Position = 0.0; }";
        static const char s_frag_code[] = "void main() { gl_FragColor = 1.0; }";

        pgDrawMgr::createAfterRes();
        pgDrawMgr::newTexture(pgID_("TEST"), 16, 16, pgTex::FORMAT_RGB);

        pgShd* shd1 = pgDrawMgr::newShader(pgID::genID(), s_vert_code, s_frag_code, 0, 0, 0);
        pgShd* shd2 = pgDrawMgr::newShader(pgID::genID(), s_vert_code, s_frag_code, 4, 5, 0);

        pgRend_Shader rend_shd;

        pgAssertThrow(rend_shd.getShaderID(), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.get2ndTextureID(), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.set2ndTextureID(pgID::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.get3rdTextureID(), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.set3rdTextureID(pgID::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.uniform(0), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.setUniformVec(0, pgVec::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.setUniformCol3x255(0, pgCol::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.setUniformCol4x255(0, pgCol::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.attrib(0, 0), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.setAttribVec(0, 0, pgVec::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.setAttribCol3x255(0, 0, pgCol::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_shd.setAttribCol4x255(0, 0, pgCol::ZERO), pgRend::ExceptionNotInitialized);

        pgPrim prim;

        prim.init(pgPrim::MODE_LINES, 2, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_shd.init(&prim, shd1->getID());

        pgAssert(rend_shd.getShaderID() == shd1->getID());
        pgAssert(rend_shd.get2ndTextureID() == pgID::ZERO);
        pgAssert(rend_shd.get3rdTextureID() == pgID::ZERO);

        pgAssertThrow(rend_shd.uniform(0), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setUniformVec(0, pgVec::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setUniformCol3x255(0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setUniformCol4x255(0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.attrib(2, 0), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.attrib(0, 0), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribVec(2, 0, pgVec::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribVec(0, 0, pgVec::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol3x255(2, 0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol3x255(2, 0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol4x255(0, 0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol4x255(0, 0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);

        pgDrawMgr::renderForSystem();

        prim.init(pgPrim::MODE_LINES, 3, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_shd.init(&prim, shd2->getID());

        pgAssert(rend_shd.getShaderID() == shd2->getID());
        pgAssert(rend_shd.get2ndTextureID() == pgID::ZERO);
        pgAssert(rend_shd.get3rdTextureID() == pgID::ZERO);

        rend_shd.set2ndTextureID(pgID::ZERO);
        pgAssert(rend_shd.get2ndTextureID() == pgID::ZERO);

        rend_shd.set3rdTextureID(pgID::ZERO);
        pgAssert(rend_shd.get3rdTextureID() == pgID::ZERO);

        for (s32 i = 0; i < 4; i++)
        {
            rend_shd.uniform(0) = static_cast<r32>(i);
            pgAssert(isEqual(rend_shd.uniform(0), static_cast<r32>(i)));
        }

        rend_shd.setUniformVec(1, pgVec(10.0f, 20.0f, 30.0f));
        pgAssert(isEqual(rend_shd.uniform(1), 10.0f) && isEqual(rend_shd.uniform(2), 20.0f) && isEqual(rend_shd.uniform(3), 30.0f));

        rend_shd.setUniformCol3x255(1, pgCol(11, 22, 33));
        pgAssert(isEqual(rend_shd.uniform(1), 11.0f) && isEqual(rend_shd.uniform(2), 22.0f) && isEqual(rend_shd.uniform(3), 33.0f));

        rend_shd.setUniformCol4x255(0, pgCol(1, 2, 3, 4));
        pgAssert(isEqual(rend_shd.uniform(0), 1.0f) && isEqual(rend_shd.uniform(1), 2.0f) && //
            isEqual(rend_shd.uniform(2), 3.0f) && isEqual(rend_shd.uniform(3), 4.0f));

        for (s32 i = 0; i < 3; i++)
        {
            for (s32 j = 0; j < 5; j++)
            {
                rend_shd.attrib(i, j) = static_cast<r32>(i + j);
                pgAssert(isEqual(rend_shd.attrib(i, j), static_cast<r32>(i + j)));
            }

            rend_shd.setAttribVec(i, 2, pgVec(10.0f + i, 11.0f + i, 12.0f + i));
            pgAssert(isEqual(rend_shd.attrib(i, 2), 10.0f + i) && isEqual(rend_shd.attrib(i, 3), 11.0f + i) && isEqual(rend_shd.attrib(i, 4), 12.0f + i));

            rend_shd.setAttribCol3x255(i, 2, pgCol(i + 2, i + 3, i + 4));
            pgAssert(isEqual(rend_shd.attrib(i, 2), 2.0f + i) && isEqual(rend_shd.attrib(i, 3), 3.0f + i) && isEqual(rend_shd.attrib(i, 4), 4.0f + i));

            rend_shd.setAttribCol4x255(i, 1, pgCol(i + 5, i + 6, i + 7, i + 8));
            pgAssert(isEqual(rend_shd.attrib(i, 1), 5.0f + i) && isEqual(rend_shd.attrib(i, 2), 6.0f + i) && //
                isEqual(rend_shd.attrib(i, 3), 7.0f + i) && isEqual(rend_shd.attrib(i, 4), 8.0f + i));
        }

        pgAssertThrow(rend_shd.uniform(4), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setUniformVec(2, pgVec::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setUniformCol3x255(2, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setUniformCol4x255(1, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.attrib(3, 0), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.attrib(0, 5), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribVec(3, 0, pgVec::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribVec(0, 3, pgVec::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol3x255(3, 0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol3x255(0, 3, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol4x255(3, 0, pgCol::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.setAttribCol4x255(0, 2, pgCol::ZERO), pgRend::ExceptionInvalidArgument);

        pgDrawMgr::renderForSystem();

        pgAssertThrow(rend_shd.set2ndTextureID(pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_shd.set3rdTextureID(pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_shd.init(NULL, shd1->getID()), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.init(&prim, pgID::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.init(&prim, pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_shd.init(&prim, NULL, shd2->getID()), pgRend::ExceptionInvalidCall);

        pgPrim::PrimData prim_data[3];
        r32 rend_data[3];

        prim.init(pgPrim::MODE_LINES, prim_data, 3, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_shd.init(&prim, rend_data, shd2->getID());

        pgAssertThrow(rend_shd.init(NULL, rend_data, shd2->getID()), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.init(&prim, NULL, shd2->getID()), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.init(&prim, rend_data, pgID::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_shd.init(&prim, rend_data, pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_shd.init(&prim, shd2->getID()), pgRend::ExceptionInvalidCall);

        pgDrawMgr::destroyBeforeRes();
    }
}
