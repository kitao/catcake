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


void pgRend_LightTest()
{
    /*
        void init(pgPrim* prim, pgID lts_id)
        void init(pgPrim* prim, RendData* rend_data, pgID lts_id)
        pgID getLightSetID()
        void setLightSetID(pgID lts_id)
        pgVec& dataN(u16 index)
    */
    {
        pgDrawMgr::createAfterRes();

        pgLts* lts = pgDrawMgr::newLightSet(pgID::genID());

        pgRend_Light rend_lit;

        pgAssertThrow(rend_lit.getLightSetID(), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_lit.setLightSetID(pgID::ZERO), pgRend::ExceptionNotInitialized);
        pgAssertThrow(rend_lit.dataN(0), pgRend::ExceptionNotInitialized);

        pgPrim prim;

        prim.init(pgPrim::MODE_LINES, 3, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_lit.init(&prim, pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        pgAssert(rend_lit.getLightSetID() == pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        for (s32 i = 0; i < 3; i++)
        {
            pgAssert(isEqual(rend_lit.dataN(i), pgVec::Z_UNIT));

            rend_lit.dataN(i).set(1.0f + i, 2.0f + i, 3.0f + i);
            pgAssert(isEqual(rend_lit.dataN(i), pgVec(1.0f + i, 2.0f + i, 3.0f + i)));
        }

        pgAssertThrow(rend_lit.dataN(3), pgRend::ExceptionInvalidArgument);

        pgDrawMgr::renderForSystem();

        prim.reallocData(5);

        for (s32 i = 0; i < 5; i++)
        {
            if (i < 3)
            {
                pgAssert(isEqual(rend_lit.dataN(i), pgVec(1.0f + i, 2.0f + i, 3.0f + i)));
            }
            else
            {
                pgAssert(isEqual(rend_lit.dataN(i), pgVec::Z_UNIT));
            }
        }

        rend_lit.setLightSetID(lts->getID());
        pgAssert(rend_lit.getLightSetID() == lts->getID());

        pgAssertThrow(rend_lit.dataN(5), pgRend::ExceptionInvalidArgument);

        pgDrawMgr::renderForSystem();

        pgAssertThrow(rend_lit.setLightSetID(pgID::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_lit.setLightSetID(pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_lit.init(NULL, pgDrawMgr::DEFAULT_LIGHT_SET_ID), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_lit.init(&prim, pgID::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_lit.init(&prim, pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_lit.init(&prim, NULL, pgDrawMgr::DEFAULT_LIGHT_SET_ID), pgRend::ExceptionInvalidCall);

        pgPrim::PrimData prim_data[3];
        pgRend_Light::RendData rend_data[3];

        prim.init(pgPrim::MODE_LINES, prim_data, 3, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_lit.init(&prim, rend_data, pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        pgAssertThrow(rend_lit.init(NULL, rend_data, pgDrawMgr::DEFAULT_LIGHT_SET_ID), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_lit.init(&prim, NULL, pgDrawMgr::DEFAULT_LIGHT_SET_ID), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_lit.init(&prim, rend_data, pgID::ZERO), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(rend_lit.init(&prim, rend_data, pgID::genID()), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(rend_lit.init(&prim, pgDrawMgr::DEFAULT_LIGHT_SET_ID), pgRend::ExceptionInvalidCall);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void calcNormalAsTriangles(bool is_smoothing)
    */
    {
        pgDrawMgr::createAfterRes();

        pgRend_Light rend_lit;

        pgAssertThrow(rend_lit.calcNormalAsTriangles(false), pgRend::ExceptionNotInitialized);

        pgPrim prim;

        prim.init(pgPrim::MODE_TRIANGLES, 2, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        rend_lit.init(&prim, pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        prim.dataPos(0).set(0.0f, 0.0f, 0.0f);
        prim.dataPos(1).set(0.0f, 0.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Z_UNIT));

        prim.reallocData(3);
        prim.dataPos(2).set(10.0f, 0.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));

        prim.reallocData(5);
        prim.dataPos(3).set(10.0f, 0.0f, 10.0f);
        prim.dataPos(4).set(0.0f, 0.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(3), pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(4), pgVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(3), pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(4), pgVec::Z_UNIT));

        prim.reallocData(6);
        prim.dataPos(5).set(30.0f, 30.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(5), -pgVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(5), -pgVec::Z_UNIT));

        prim.reallocData(7);
        prim.dataPos(6).set(30.0f, 30.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(5), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(6), pgVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        pgAssert(isEqual(rend_lit.dataN(0), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(1), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(2), pgVec::Y_UNIT));
        pgAssert(isEqual(rend_lit.dataN(3), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(4), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(5), -pgVec::Z_UNIT));
        pgAssert(isEqual(rend_lit.dataN(6), pgVec::Z_UNIT));

        pgDrawMgr::destroyBeforeRes();
    }
}
