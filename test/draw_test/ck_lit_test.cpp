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


void ckLitTest()
{
    /*
        ckLit* getPrevN() const
        ckLit* getNextN() const
        ckID getID() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckLts* lts0 = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);

        ckLit* lit1 = lts0->newPointLight(ckID::genID());
        ckAssert(!lit1->getPrevN() && !lit1->getNextN());

        ckLit* lit2 = lts0->newPointLight(ckID::genID());

        ckAssert(!lit1->getPrevN() && lit1->getNextN() == lit2);
        ckAssert(lit2->getPrevN() == lit1 && !lit2->getNextN());

        lts0->deletePointLight(lit1->getID());

        ckAssert(!lit2->getPrevN() && !lit2->getNextN());

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        bool isActive() const
        void setActive(bool is_active)
        const ckVec& getPos() const
        void setPos(const ckVec& pos)
        r32 getInnerRadius() const
        r32 getOuterRadius() const
        void setRadius(r32 inner_rad, r32 outer_rad)
        ckCol getColor() const
        void setColor(ckCol col)
    */
    {
        ckDrawMgr::createAfterRes();

        ckLts* lts0 = ckDrawMgr::getLightSet(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
        ckLit* lit = lts0->newPointLight(ckID::genID());

        ckAssert(lit->isActive());
        ckAssert(isEqual(lit->getPos(), ckVec::ZERO));
        ckAssert(isEqual(lit->getInnerRadius(), 0.0f) && isEqual(lit->getOuterRadius(), 0.0f));
        ckAssert(lit->getColor() == ckCol(255, 255, 0));

        lit->setActive(false);
        ckAssert(!lit->isActive());

        lit->setPos(ckVec(-10.0f, -20.0f, -30.0f));
        ckAssert(isEqual(lit->getPos(), ckVec(-10.0f, -20.0f, -30.0f)));

        lit->setRadius(30.0f, 40.0f);
        ckAssert(isEqual(lit->getInnerRadius(), 30.0f) && isEqual(lit->getOuterRadius(), 40.0f));

        lit->setRadius(10.0f, 10.0f);
        ckAssert(isEqual(lit->getInnerRadius(), 10.0f) && isEqual(lit->getOuterRadius(), 10.0f));

        lit->setRadius(0.0f, 0.0f);
        ckAssert(isEqual(lit->getInnerRadius(), 0.0f) && isEqual(lit->getOuterRadius(), 0.0f));

        ckAssertThrow(lit->setRadius(-1.0f, 2.0f), ckLit::ExceptionInvalidArgument);
        ckAssertThrow(lit->setRadius(1.0f, -2.0f), ckLit::ExceptionInvalidArgument);
        ckAssertThrow(lit->setRadius(2.0f, 1.0f), ckLit::ExceptionInvalidArgument);

        ckDrawMgr::destroyBeforeRes();
    }
}
