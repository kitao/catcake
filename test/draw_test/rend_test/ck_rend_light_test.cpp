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


void ckRend_LightTest()
{
    /*
        void init(ckPrim* prim, ckID lts_id)
        void init(ckPrim* prim, RendData* rend_data, ckID lts_id)
        ckID getLightSetID()
        void setLightSetID(ckID lts_id)
        ckVec& dataN(u16 index)
    */
    {
        ckDrawMgr::createAfterRes();

        ckLts* lts = ckDrawMgr::newLightSet(ckID::genID());

        ckRend_Light rend_lit;

        ckAssertThrow(rend_lit.getLightSetID(), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_lit.setLightSetID(ckID::ZERO), ckRend::ExceptionNotInitialized);
        ckAssertThrow(rend_lit.dataN(0), ckRend::ExceptionNotInitialized);

        ckPrim prim;

        prim.init(ckPrim::MODE_LINES, 3, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_lit.init(&prim, ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckAssert(rend_lit.getLightSetID() == ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        for (s32 i = 0; i < 3; i++)
        {
            ckAssert(isEqual(rend_lit.dataN(i), ckVec::Z_UNIT));

            rend_lit.dataN(i).set(1.0f + i, 2.0f + i, 3.0f + i);
            ckAssert(isEqual(rend_lit.dataN(i), ckVec(1.0f + i, 2.0f + i, 3.0f + i)));
        }

        ckAssertThrow(rend_lit.dataN(3), ckRend::ExceptionInvalidArgument);

        ckDrawMgr::renderForSystem();

        prim.reallocData(5);

        for (s32 i = 0; i < 5; i++)
        {
            if (i < 3)
            {
                ckAssert(isEqual(rend_lit.dataN(i), ckVec(1.0f + i, 2.0f + i, 3.0f + i)));
            }
            else
            {
                ckAssert(isEqual(rend_lit.dataN(i), ckVec::Z_UNIT));
            }
        }

        rend_lit.setLightSetID(lts->getID());
        ckAssert(rend_lit.getLightSetID() == lts->getID());

        ckAssertThrow(rend_lit.dataN(5), ckRend::ExceptionInvalidArgument);

        ckDrawMgr::renderForSystem();

        ckAssertThrow(rend_lit.setLightSetID(ckID::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_lit.setLightSetID(ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_lit.init(NULL, ckDrawMgr::DEFAULT_LIGHT_SET_ID), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_lit.init(&prim, ckID::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_lit.init(&prim, ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_lit.init(&prim, NULL, ckDrawMgr::DEFAULT_LIGHT_SET_ID), ckRend::ExceptionInvalidCall);

        ckPrim::PrimData prim_data[3];
        ckRend_Light::RendData rend_data[3];

        prim.init(ckPrim::MODE_LINES, prim_data, 3, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        rend_lit.init(&prim, rend_data, ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckAssertThrow(rend_lit.init(NULL, rend_data, ckDrawMgr::DEFAULT_LIGHT_SET_ID), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_lit.init(&prim, NULL, ckDrawMgr::DEFAULT_LIGHT_SET_ID), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_lit.init(&prim, rend_data, ckID::ZERO), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(rend_lit.init(&prim, rend_data, ckID::genID()), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(rend_lit.init(&prim, ckDrawMgr::DEFAULT_LIGHT_SET_ID), ckRend::ExceptionInvalidCall);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void calcNormalAsTriangles(bool is_smoothing)
    */
    {
        ckDrawMgr::createAfterRes();

        ckRend_Light rend_lit;

        ckAssertThrow(rend_lit.calcNormalAsTriangles(false), ckRend::ExceptionNotInitialized);

        ckPrim prim;

        prim.init(ckPrim::MODE_TRIANGLES, 2, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        rend_lit.init(&prim, ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        prim.dataPos(0).set(0.0f, 0.0f, 0.0f);
        prim.dataPos(1).set(0.0f, 0.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Z_UNIT));

        prim.reallocData(3);
        prim.dataPos(2).set(10.0f, 0.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));

        prim.reallocData(5);
        prim.dataPos(3).set(10.0f, 0.0f, 10.0f);
        prim.dataPos(4).set(0.0f, 0.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(3), ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(4), ckVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(3), ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(4), ckVec::Z_UNIT));

        prim.reallocData(6);
        prim.dataPos(5).set(30.0f, 30.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(5), -ckVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(5), -ckVec::Z_UNIT));

        prim.reallocData(7);
        prim.dataPos(6).set(30.0f, 30.0f, 10.0f);

        rend_lit.calcNormalAsTriangles(false);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(5), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(6), ckVec::Z_UNIT));

        rend_lit.calcNormalAsTriangles(true);
        ckAssert(isEqual(rend_lit.dataN(0), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(1), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(2), ckVec::Y_UNIT));
        ckAssert(isEqual(rend_lit.dataN(3), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(4), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(5), -ckVec::Z_UNIT));
        ckAssert(isEqual(rend_lit.dataN(6), ckVec::Z_UNIT));

        ckDrawMgr::destroyBeforeRes();
    }
}
