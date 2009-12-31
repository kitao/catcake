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


void ckLtsTest()
{
    /*
        static const u32 MAX_PARA_LIGHT_NUM
    */
    {
        ckAssert(ckLts::MAX_PARA_LIGHT_NUM == 3);
    }

    /*
        static const u32 MAX_NEAR_LIGHT_NUM
    */
    {
        ckAssert(ckLts::MAX_NEAR_LIGHT_NUM == 3);
    }

    /*
        ckLts* getPrevN() const
        ckLts* getNextN() const
        ckID getID() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckLts* lts0 = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(lts0->getID() == ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckAssert(!lts0->getPrevN() && !lts0->getNextN());

        ckID id1 = ckID::genID();
        ckLts* lts1 = ckDrawMgr::newLightSet(id1);
        ckAssert(lts1->getID() == id1);

        ckAssert(!lts0->getPrevN() && lts0->getNextN() == lts1);
        ckAssert(lts1->getPrevN() == lts0 && !lts1->getNextN());

        ckID id2 = ckID::genID();
        ckLts* lts2 = ckDrawMgr::newLightSet(id2);
        ckAssert(lts2->getID() == id2);

        ckAssert(!lts0->getPrevN() && lts0->getNextN() == lts1);
        ckAssert(lts1->getPrevN() == lts0 && lts1->getNextN() == lts2);
        ckAssert(lts2->getPrevN() == lts1 && !lts2->getNextN());

        ckDrawMgr::deleteLightSet(id1);

        ckAssert(!lts0->getPrevN() && lts0->getNextN() == lts2);
        ckAssert(lts2->getPrevN() == lts0 && !lts2->getNextN());

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        ckCol getAmbientColor() const
        void setAmbientColor(ckCol col)
        bool isParaLightActive(u8 index) const
        void setParaLightActive(u8 index, bool is_active)
        const ckVec& getParaLightDir(u8 index) const
        void setParaLightDir(u8 index, const ckVec& dir)
        ckCol getParaLightColor(u8 index) const
        void setParaLightColor(u8 index, ckCol col)
    */
    {
        ckDrawMgr::createAfterRes();

        ckLts* lts = ckDrawMgr::newLightSet(ckID::genID());

        ckAssert(lts->getAmbientColor() == ckCol::ZERO);

        for (u32 i = 0; i < ckLts::MAX_PARA_LIGHT_NUM; i++)
        {
            ckAssert(!lts->isParaLightActive(i));
            ckAssert(isEqual(lts->getParaLightDir(i), ckVec::X_UNIT));
            ckAssert(lts->getParaLightColor(i) == ckCol::ZERO);
        }

        lts->setAmbientColor(ckCol(10, 20, 30, 40));
        ckAssert(lts->getAmbientColor() == ckCol(10, 20, 30, 40));

        for (u32 i = 0; i < ckLts::MAX_PARA_LIGHT_NUM; i++)
        {
            lts->setParaLightActive(i, true);
            ckAssert(lts->isParaLightActive(i));

            lts->setParaLightActive(i, false);
            ckAssert(!lts->isParaLightActive(i));

            lts->setParaLightDir(i, ckVec(-1.0f + i, 2.0f + i, -3.0f + i));
            ckAssert(isEqual(lts->getParaLightDir(i), ckVec(-1.0f + i, 2.0f + i, -3.0f + i).normalize()));

            lts->setParaLightDir(i, ckVec::ZERO);
            ckAssert(isEqual(lts->getParaLightDir(i), ckVec(0.0f, -1.0f, 0.0f)));

            lts->setParaLightColor(i, ckCol(12 + i, 34 + i, 56 + i));
            ckAssert(lts->getParaLightColor(i) == ckCol(12 + i, 34 + i, 56 + i));
        }

        ckAssertThrow(lts->isParaLightActive(ckLts::MAX_PARA_LIGHT_NUM), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->setParaLightActive(ckLts::MAX_PARA_LIGHT_NUM, false), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->getParaLightDir(ckLts::MAX_PARA_LIGHT_NUM), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->setParaLightDir(ckLts::MAX_PARA_LIGHT_NUM, ckVec::X_UNIT), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->getParaLightColor(ckLts::MAX_PARA_LIGHT_NUM), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->setParaLightColor(ckLts::MAX_PARA_LIGHT_NUM, ckCol::ZERO), ckLts::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        u32 getPointLightNum() const
        ckLit* getPointLight(ckID lit_id)
        ckLit* newPointLight(ckID lit_id)
        void deletePointLight(ckID lit_id)
        ckLit* getFirstPointLightN() const
        ckLit* getLastPointLightN() const
        void clearPointLight()
    */
    {
        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDrawMgr::createAfterRes();

        ckLts* lts = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckAssert(lts->getPointLightNum() == 0);
        ckAssert(!lts->getFirstPointLightN() && !lts->getLastPointLightN());

        ckID id1 = ckID::genID();
        ckLit* lit1 = lts->newPointLight(id1);
        ckAssert(lit1->getID() == id1);
        ckAssert(lts->getPointLightNum() == 1);
        ckAssert(lts->getFirstPointLightN() == lit1 && lts->getLastPointLightN() == lit1);

        ckAssertThrow(lts->newPointLight(id1), ckLts::ExceptionSameIDExists);

        ckID id2 = ckID::genID();
        ckLit* lit2 = lts->newPointLight(id2);
        ckAssert(lit2->getID() == id2);
        ckAssert(lts->getPointLightNum() == 2);
        ckAssert(lts->getFirstPointLightN() == lit1 && lts->getLastPointLightN() == lit2);

        lts->deletePointLight(id1);
        ckAssert(lts->getPointLightNum() == 1);
        ckAssert(lts->getFirstPointLightN() == lit2 && lts->getLastPointLightN() == lit2);

        ckAssertThrow(lts->deletePointLight(id1), ckLts::ExceptionNotFound);

        lts->deletePointLight(id2);
        ckAssert(lts->getPointLightNum() == 0);
        ckAssert(!lts->getFirstPointLightN() && !lts->getLastPointLightN());

        lit1 = lts->newPointLight(ckID::genID());
        lit2 = lts->newPointLight(ckID::genID());
        ckAssert(lts->getPointLightNum() == 2);
        ckAssert(lts->getFirstPointLightN() == lit1 && lts->getLastPointLightN() == lit2);

        lts->clearPointLight();
        ckAssert(lts->getPointLightNum() == 0);
        ckAssert(!lts->getFirstPointLightN() && !lts->getLastPointLightN());

        lts->clearPointLight();

        lts->newPointLight(ckID::genID());
        lts->newPointLight(ckID::genID());
        lts->newPointLight(ckID::genID());

        ckAssertThrow(lts->getPointLight(ckID::ZERO), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->newPointLight(ckID::ZERO), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->deletePointLight(ckID::ZERO), ckLts::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        void findNearLight(const ckVec& pos)
        u8 getNearLightNum() const
        const ckVec& getNearLightDir(u8 index) const
        ckCol getNearLightColor(u8 index) const
    */
    {
        ckDrawMgr::createAfterRes();

        ckLts* lts = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckAssert(lts->getNearLightNum() == 0);

        ckAssertThrow(lts->getNearLightDir(0), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->getNearLightColor(0), ckLts::ExceptionInvalidArgument);

        lts->setParaLightDir(0, -ckVec::X_UNIT);
        lts->setParaLightDir(1, -ckVec::Y_UNIT);
        lts->setParaLightDir(2, -ckVec::Z_UNIT);

        lts->setParaLightColor(0, ckCol(11, 11, 11));
        lts->setParaLightColor(1, ckCol(33, 33, 33));
        lts->setParaLightColor(2, ckCol(22, 22, 22));

        lts->setParaLightActive(0, false);
        lts->setParaLightActive(1, false);
        lts->setParaLightActive(2, true);

        lts->findNearLight(ckVec(100.0f, 100.0f, 100.0f));
        ckAssert(lts->getNearLightNum() == 1);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Z_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(22, 22, 22));

        ckAssertThrow(lts->getNearLightDir(1), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->getNearLightColor(1), ckLts::ExceptionInvalidArgument);

        lts->setParaLightActive(0, false);
        lts->setParaLightActive(1, true);
        lts->setParaLightActive(2, true);

        lts->findNearLight(ckVec(100.0f, 100.0f, 100.0f));
        ckAssert(lts->getNearLightNum() == 2);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Y_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(1), -ckVec::Z_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(33, 33, 33));
        ckAssert(lts->getNearLightColor(1) == ckCol(22, 22, 22));

        ckAssertThrow(lts->getNearLightDir(2), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->getNearLightColor(2), ckLts::ExceptionInvalidArgument);

        lts->setParaLightActive(0, true);
        lts->setParaLightActive(1, true);
        lts->setParaLightActive(2, true);

        lts->findNearLight(ckVec(100.0f, 100.0f, 100.0f));
        ckAssert(lts->getNearLightNum() == 3);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Y_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(1), -ckVec::Z_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(2), -ckVec::X_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(33, 33, 33));
        ckAssert(lts->getNearLightColor(1) == ckCol(22, 22, 22));
        ckAssert(lts->getNearLightColor(2) == ckCol(11, 11, 11));

        ckAssertThrow(lts->getNearLightDir(3), ckLts::ExceptionInvalidArgument);
        ckAssertThrow(lts->getNearLightColor(3), ckLts::ExceptionInvalidArgument);

        ckLit* lit1 = lts->newPointLight(ckID::genID());

        lit1->setPos(ckVec(1000.0f, 1000.0f, 1000.0f));
        lit1->setRadius(100.0f, 200.0f);
        lit1->setColor(ckCol(30, 30, 30));

        lts->findNearLight(ckVec(1050.0f, 1000.0f, 1000.0f));
        ckAssert(lts->getNearLightNum() == 3);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Y_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(1), ckVec::X_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(2), -ckVec::Z_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(33, 33, 33));
        ckAssert(lts->getNearLightColor(1) == ckCol(30, 30, 30));
        ckAssert(lts->getNearLightColor(2) == ckCol(22, 22, 22));

        ckLit* lit2 = lts->newPointLight(ckID::genID());

        lit2->setPos(ckVec(1000.0f, 900.0f, 1000.0f));
        lit2->setRadius(50.0f, 150.0f);
        lit2->setColor(ckCol(40, 40, 40));

        lts->findNearLight(ckVec(1000.0f, 1000.0f, 1000.0f));
        ckAssert(lts->getNearLightNum() == 3);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Y_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(1), -ckVec::Y_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(2), -ckVec::Z_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(33, 33, 33));
        ckAssert(lts->getNearLightColor(1) == ckCol(30, 30, 30));
        ckAssert(lts->getNearLightColor(2) == ckCol(22, 22, 22));

        lts->setParaLightActive(0, true);
        lts->setParaLightActive(1, false);
        lts->setParaLightActive(2, true);

        lts->findNearLight(ckVec(1000.0f, 1150.0f, 1000.0f));
        ckAssert(lts->getNearLightNum() == 3);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Z_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(1), ckVec::Y_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(2), -ckVec::X_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(22, 22, 22));
        ckAssert(lts->getNearLightColor(1) == ckCol(15, 15, 15));
        ckAssert(lts->getNearLightColor(2) == ckCol(11, 11, 11));

        lit1->setActive(false);

        lts->findNearLight(ckVec(1000.0f, 1150.0f, 1000.0f));
        ckAssert(lts->getNearLightNum() == 2);
        ckAssert(isEqual(lts->getNearLightDir(0), -ckVec::Z_UNIT));
        ckAssert(isEqual(lts->getNearLightDir(1), -ckVec::X_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(22, 22, 22));
        ckAssert(lts->getNearLightColor(1) == ckCol(11, 11, 11));

        lts->setParaLightActive(0, false);
        lts->setParaLightActive(1, false);
        lts->setParaLightActive(2, false);

        lit1->setActive(true);

        lts->findNearLight(ckVec(1000.0f, 1150.0f, 1000.0f));
        ckAssert(lts->getNearLightNum() == 1);
        ckAssert(isEqual(lts->getNearLightDir(0), ckVec::Y_UNIT));
        ckAssert(lts->getNearLightColor(0) == ckCol(15, 15, 15));

        lts->findNearLight(ckVec(1000.0f, 1201.0f, 1000.0f));
        ckAssert(lts->getNearLightNum() == 0);

        ckDrawMgr::destroyBeforeRes();
    }
}
