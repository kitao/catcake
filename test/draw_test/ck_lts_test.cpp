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


void pgLtsTest()
{
    /*
        static const u32 MAX_PARA_LIGHT_NUM
    */
    {
        pgAssert(pgLts::MAX_PARA_LIGHT_NUM == 3);
    }

    /*
        static const u32 MAX_NEAR_LIGHT_NUM
    */
    {
        pgAssert(pgLts::MAX_NEAR_LIGHT_NUM == 3);
    }

    /*
        pgLts* getPrevN() const
        pgLts* getNextN() const
        pgID getID() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgLts* lts0 = pgDrawMgr::getLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(lts0->getID() == pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        pgAssert(!lts0->getPrevN() && !lts0->getNextN());

        pgID id1 = pgID::genID();
        pgLts* lts1 = pgDrawMgr::newLightSet(id1);
        pgAssert(lts1->getID() == id1);

        pgAssert(!lts0->getPrevN() && lts0->getNextN() == lts1);
        pgAssert(lts1->getPrevN() == lts0 && !lts1->getNextN());

        pgID id2 = pgID::genID();
        pgLts* lts2 = pgDrawMgr::newLightSet(id2);
        pgAssert(lts2->getID() == id2);

        pgAssert(!lts0->getPrevN() && lts0->getNextN() == lts1);
        pgAssert(lts1->getPrevN() == lts0 && lts1->getNextN() == lts2);
        pgAssert(lts2->getPrevN() == lts1 && !lts2->getNextN());

        pgDrawMgr::deleteLightSet(id1);

        pgAssert(!lts0->getPrevN() && lts0->getNextN() == lts2);
        pgAssert(lts2->getPrevN() == lts0 && !lts2->getNextN());

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        pgCol getAmbientColor() const
        void setAmbientColor(pgCol col)
        bool isParaLightActive(u8 index) const
        void setParaLightActive(u8 index, bool is_active)
        const pgVec& getParaLightDir(u8 index) const
        void setParaLightDir(u8 index, const pgVec& dir)
        pgCol getParaLightColor(u8 index) const
        void setParaLightColor(u8 index, pgCol col)
    */
    {
        pgDrawMgr::createAfterRes();

        pgLts* lts = pgDrawMgr::newLightSet(pgID::genID());

        pgAssert(lts->getAmbientColor() == pgCol::ZERO);

        for (u32 i = 0; i < pgLts::MAX_PARA_LIGHT_NUM; i++)
        {
            pgAssert(!lts->isParaLightActive(i));
            pgAssert(isEqual(lts->getParaLightDir(i), pgVec::X_UNIT));
            pgAssert(lts->getParaLightColor(i) == pgCol::ZERO);
        }

        lts->setAmbientColor(pgCol(10, 20, 30, 40));
        pgAssert(lts->getAmbientColor() == pgCol(10, 20, 30, 40));

        for (u32 i = 0; i < pgLts::MAX_PARA_LIGHT_NUM; i++)
        {
            lts->setParaLightActive(i, true);
            pgAssert(lts->isParaLightActive(i));

            lts->setParaLightActive(i, false);
            pgAssert(!lts->isParaLightActive(i));

            lts->setParaLightDir(i, pgVec(-1.0f + i, 2.0f + i, -3.0f + i));
            pgAssert(isEqual(lts->getParaLightDir(i), pgVec(-1.0f + i, 2.0f + i, -3.0f + i).normalize()));

            lts->setParaLightDir(i, pgVec::ZERO);
            pgAssert(isEqual(lts->getParaLightDir(i), pgVec(0.0f, -1.0f, 0.0f)));

            lts->setParaLightColor(i, pgCol(12 + i, 34 + i, 56 + i));
            pgAssert(lts->getParaLightColor(i) == pgCol(12 + i, 34 + i, 56 + i));
        }

        pgAssertThrow(lts->isParaLightActive(pgLts::MAX_PARA_LIGHT_NUM), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->setParaLightActive(pgLts::MAX_PARA_LIGHT_NUM, false), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->getParaLightDir(pgLts::MAX_PARA_LIGHT_NUM), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->setParaLightDir(pgLts::MAX_PARA_LIGHT_NUM, pgVec::X_UNIT), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->getParaLightColor(pgLts::MAX_PARA_LIGHT_NUM), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->setParaLightColor(pgLts::MAX_PARA_LIGHT_NUM, pgCol::ZERO), pgLts::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        u32 getPointLightNum() const
        pgLit* getPointLight(pgID lit_id)
        pgLit* newPointLight(pgID lit_id)
        void deletePointLight(pgID lit_id)
        pgLit* getFirstPointLightN() const
        pgLit* getLastPointLightN() const
        void clearPointLight()
    */
    {
        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDrawMgr::createAfterRes();

        pgLts* lts = pgDrawMgr::getLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);

        pgAssert(lts->getPointLightNum() == 0);
        pgAssert(!lts->getFirstPointLightN() && !lts->getLastPointLightN());

        pgID id1 = pgID::genID();
        pgLit* lit1 = lts->newPointLight(id1);
        pgAssert(lit1->getID() == id1);
        pgAssert(lts->getPointLightNum() == 1);
        pgAssert(lts->getFirstPointLightN() == lit1 && lts->getLastPointLightN() == lit1);

        pgAssertThrow(lts->newPointLight(id1), pgLts::ExceptionSameIDExists);

        pgID id2 = pgID::genID();
        pgLit* lit2 = lts->newPointLight(id2);
        pgAssert(lit2->getID() == id2);
        pgAssert(lts->getPointLightNum() == 2);
        pgAssert(lts->getFirstPointLightN() == lit1 && lts->getLastPointLightN() == lit2);

        lts->deletePointLight(id1);
        pgAssert(lts->getPointLightNum() == 1);
        pgAssert(lts->getFirstPointLightN() == lit2 && lts->getLastPointLightN() == lit2);

        pgAssertThrow(lts->deletePointLight(id1), pgLts::ExceptionNotFound);

        lts->deletePointLight(id2);
        pgAssert(lts->getPointLightNum() == 0);
        pgAssert(!lts->getFirstPointLightN() && !lts->getLastPointLightN());

        lit1 = lts->newPointLight(pgID::genID());
        lit2 = lts->newPointLight(pgID::genID());
        pgAssert(lts->getPointLightNum() == 2);
        pgAssert(lts->getFirstPointLightN() == lit1 && lts->getLastPointLightN() == lit2);

        lts->clearPointLight();
        pgAssert(lts->getPointLightNum() == 0);
        pgAssert(!lts->getFirstPointLightN() && !lts->getLastPointLightN());

        lts->clearPointLight();

        lts->newPointLight(pgID::genID());
        lts->newPointLight(pgID::genID());
        lts->newPointLight(pgID::genID());

        pgAssertThrow(lts->getPointLight(pgID::ZERO), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->newPointLight(pgID::ZERO), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->deletePointLight(pgID::ZERO), pgLts::ExceptionInvalidArgument);

        pgDrawMgr::destroyBeforeRes();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);
    }

    /*
        void findNearLight(const pgVec& pos)
        u8 getNearLightNum() const
        const pgVec& getNearLightDir(u8 index) const
        pgCol getNearLightColor(u8 index) const
    */
    {
        pgDrawMgr::createAfterRes();

        pgLts* lts = pgDrawMgr::getLightSet(pgDrawMgr::DEFAULT_LIGHT_SET_ID);
        pgAssert(lts->getNearLightNum() == 0);

        pgAssertThrow(lts->getNearLightDir(0), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->getNearLightColor(0), pgLts::ExceptionInvalidArgument);

        lts->setParaLightDir(0, -pgVec::X_UNIT);
        lts->setParaLightDir(1, -pgVec::Y_UNIT);
        lts->setParaLightDir(2, -pgVec::Z_UNIT);

        lts->setParaLightColor(0, pgCol(11, 11, 11));
        lts->setParaLightColor(1, pgCol(33, 33, 33));
        lts->setParaLightColor(2, pgCol(22, 22, 22));

        lts->setParaLightActive(0, false);
        lts->setParaLightActive(1, false);
        lts->setParaLightActive(2, true);

        lts->findNearLight(pgVec(100.0f, 100.0f, 100.0f));
        pgAssert(lts->getNearLightNum() == 1);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Z_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(22, 22, 22));

        pgAssertThrow(lts->getNearLightDir(1), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->getNearLightColor(1), pgLts::ExceptionInvalidArgument);

        lts->setParaLightActive(0, false);
        lts->setParaLightActive(1, true);
        lts->setParaLightActive(2, true);

        lts->findNearLight(pgVec(100.0f, 100.0f, 100.0f));
        pgAssert(lts->getNearLightNum() == 2);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Y_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(1), -pgVec::Z_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(33, 33, 33));
        pgAssert(lts->getNearLightColor(1) == pgCol(22, 22, 22));

        pgAssertThrow(lts->getNearLightDir(2), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->getNearLightColor(2), pgLts::ExceptionInvalidArgument);

        lts->setParaLightActive(0, true);
        lts->setParaLightActive(1, true);
        lts->setParaLightActive(2, true);

        lts->findNearLight(pgVec(100.0f, 100.0f, 100.0f));
        pgAssert(lts->getNearLightNum() == 3);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Y_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(1), -pgVec::Z_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(2), -pgVec::X_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(33, 33, 33));
        pgAssert(lts->getNearLightColor(1) == pgCol(22, 22, 22));
        pgAssert(lts->getNearLightColor(2) == pgCol(11, 11, 11));

        pgAssertThrow(lts->getNearLightDir(3), pgLts::ExceptionInvalidArgument);
        pgAssertThrow(lts->getNearLightColor(3), pgLts::ExceptionInvalidArgument);

        pgLit* lit1 = lts->newPointLight(pgID::genID());

        lit1->setPos(pgVec(1000.0f, 1000.0f, 1000.0f));
        lit1->setRadius(100.0f, 200.0f);
        lit1->setColor(pgCol(30, 30, 30));

        lts->findNearLight(pgVec(1050.0f, 1000.0f, 1000.0f));
        pgAssert(lts->getNearLightNum() == 3);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Y_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(1), pgVec::X_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(2), -pgVec::Z_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(33, 33, 33));
        pgAssert(lts->getNearLightColor(1) == pgCol(30, 30, 30));
        pgAssert(lts->getNearLightColor(2) == pgCol(22, 22, 22));

        pgLit* lit2 = lts->newPointLight(pgID::genID());

        lit2->setPos(pgVec(1000.0f, 900.0f, 1000.0f));
        lit2->setRadius(50.0f, 150.0f);
        lit2->setColor(pgCol(40, 40, 40));

        lts->findNearLight(pgVec(1000.0f, 1000.0f, 1000.0f));
        pgAssert(lts->getNearLightNum() == 3);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Y_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(1), -pgVec::Y_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(2), -pgVec::Z_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(33, 33, 33));
        pgAssert(lts->getNearLightColor(1) == pgCol(30, 30, 30));
        pgAssert(lts->getNearLightColor(2) == pgCol(22, 22, 22));

        lts->setParaLightActive(0, true);
        lts->setParaLightActive(1, false);
        lts->setParaLightActive(2, true);

        lts->findNearLight(pgVec(1000.0f, 1150.0f, 1000.0f));
        pgAssert(lts->getNearLightNum() == 3);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Z_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(1), pgVec::Y_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(2), -pgVec::X_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(22, 22, 22));
        pgAssert(lts->getNearLightColor(1) == pgCol(15, 15, 15));
        pgAssert(lts->getNearLightColor(2) == pgCol(11, 11, 11));

        lit1->setActive(false);

        lts->findNearLight(pgVec(1000.0f, 1150.0f, 1000.0f));
        pgAssert(lts->getNearLightNum() == 2);
        pgAssert(isEqual(lts->getNearLightDir(0), -pgVec::Z_UNIT));
        pgAssert(isEqual(lts->getNearLightDir(1), -pgVec::X_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(22, 22, 22));
        pgAssert(lts->getNearLightColor(1) == pgCol(11, 11, 11));

        lts->setParaLightActive(0, false);
        lts->setParaLightActive(1, false);
        lts->setParaLightActive(2, false);

        lit1->setActive(true);

        lts->findNearLight(pgVec(1000.0f, 1150.0f, 1000.0f));
        pgAssert(lts->getNearLightNum() == 1);
        pgAssert(isEqual(lts->getNearLightDir(0), pgVec::Y_UNIT));
        pgAssert(lts->getNearLightColor(0) == pgCol(15, 15, 15));

        lts->findNearLight(pgVec(1000.0f, 1201.0f, 1000.0f));
        pgAssert(lts->getNearLightNum() == 0);

        pgDrawMgr::destroyBeforeRes();
    }
}
