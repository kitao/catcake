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


void ckConfMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterRes()
        static void destroyBeforeRes()
    */
    {
        ckAssert(!ckConfMgr::isCreated());

        ckConfMgr::createAfterRes();
        ckConfMgr::createAfterRes();

        ckAssert(ckSysMgr::isCreated());

        ckConfMgr::destroyBeforeRes();
        ckConfMgr::destroyBeforeRes();

        ckAssert(!ckConfMgr::isCreated());
    }

    /*
        static bool hasConfig(ckID id)
        static ckConf* getConfig(ckID id)
        static ckConf* newConfig(ckID id, const void* data, u32 data_size)
        static void deleteConfig(ckID id)
        static ckConf* getFirstConfigN()
        static ckConf* getLastConfigN()
    */
    {
        ckAssertThrow(ckConfMgr::hasConfig(ckID::ZERO), ckConfMgr::ExceptionNotInitialized);
        ckAssertThrow(ckConfMgr::getConfig(ckID::ZERO), ckConfMgr::ExceptionNotInitialized);
        ckAssertThrow(ckConfMgr::newConfig(ckID::ZERO, NULL, 0), ckConfMgr::ExceptionNotInitialized);
        ckAssertThrow(ckConfMgr::deleteConfig(ckID::ZERO), ckConfMgr::ExceptionNotInitialized);
        ckAssertThrow(ckConfMgr::getFirstConfigN(), ckConfMgr::ExceptionNotInitialized);
        ckAssertThrow(ckConfMgr::getLastConfigN(), ckConfMgr::ExceptionNotInitialized);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckConfMgr::createAfterRes();

        ckAssert(!ckConfMgr::getFirstConfigN() && !ckConfMgr::getLastConfigN());

        ckID id1 = ckID::genID();
        ckConf* conf1 = ckConfMgr::newConfig(id1, "dummy", 5);
        ckAssert(ckConfMgr::hasConfig(id1));
        ckAssert(ckConfMgr::getConfig(id1) == conf1);
        ckAssert(ckConfMgr::getFirstConfigN() == conf1 && ckConfMgr::getLastConfigN() == conf1);

        ckAssertThrow(ckConfMgr::newConfig(id1, "dummy", 5), ckConfMgr::ExceptionSameIDExists);

        ckID id2 = ckID::genID();
        ckConf* conf2 = ckConfMgr::newConfig(id2, "dummy", 5);
        ckAssert(ckConfMgr::hasConfig(id2));
        ckAssert(ckConfMgr::getConfig(id2) == conf2);
        ckAssert(ckConfMgr::getFirstConfigN() == conf1 && ckConfMgr::getLastConfigN() == conf2);

        ckConfMgr::deleteConfig(id1);
        ckAssert(!ckConfMgr::hasConfig(id1));
        ckAssert(ckConfMgr::getFirstConfigN() == conf2 && ckConfMgr::getLastConfigN() == conf2);

        ckAssertThrow(ckConfMgr::getConfig(id1), ckConfMgr::ExceptionNotFound);

        ckConfMgr::deleteConfig(id2);
        ckAssert(!ckConfMgr::hasConfig(id2));
        ckAssert(!ckConfMgr::getFirstConfigN() && !ckConfMgr::getLastConfigN());

        ckConfMgr::newConfig(id1, "dummy", 5);
        ckConfMgr::newConfig(id2, "dummy", 5);

        ckAssertThrow(ckConfMgr::hasConfig(ckID::ZERO), ckConfMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckConfMgr::getConfig(ckID::ZERO), ckConfMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckConfMgr::newConfig(ckID::ZERO, "dummy", 5), ckConfMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckConfMgr::newConfig(ckID::genID(), NULL, 5), ckConfMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckConfMgr::newConfig(ckID::genID(), "dummy", 0), ckConfMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckConfMgr::deleteConfig(ckID::ZERO), ckConfMgr::ExceptionInvalidArgument);

        ckConfMgr::destroyBeforeRes();

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);
    }
}
