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
