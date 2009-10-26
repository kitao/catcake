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
