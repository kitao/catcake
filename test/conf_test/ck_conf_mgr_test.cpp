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


void pgConfMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterRes()
        static void destroyBeforeRes()
    */
    {
        pgAssert(!pgConfMgr::isCreated());

        pgConfMgr::createAfterRes();
        pgConfMgr::createAfterRes();

        pgAssert(pgSysMgr::isCreated());

        pgConfMgr::destroyBeforeRes();
        pgConfMgr::destroyBeforeRes();

        pgAssert(!pgConfMgr::isCreated());
    }

    /*
        static bool hasConfig(pgID id)
        static pgConf* getConfig(pgID id)
        static pgConf* newConfig(pgID id, const void* data, u32 data_size)
        static void deleteConfig(pgID id)
        static pgConf* getFirstConfigN()
        static pgConf* getLastConfigN()
    */
    {
        pgAssertThrow(pgConfMgr::hasConfig(pgID::ZERO), pgConfMgr::ExceptionNotInitialized);
        pgAssertThrow(pgConfMgr::getConfig(pgID::ZERO), pgConfMgr::ExceptionNotInitialized);
        pgAssertThrow(pgConfMgr::newConfig(pgID::ZERO, NULL, 0), pgConfMgr::ExceptionNotInitialized);
        pgAssertThrow(pgConfMgr::deleteConfig(pgID::ZERO), pgConfMgr::ExceptionNotInitialized);
        pgAssertThrow(pgConfMgr::getFirstConfigN(), pgConfMgr::ExceptionNotInitialized);
        pgAssertThrow(pgConfMgr::getLastConfigN(), pgConfMgr::ExceptionNotInitialized);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgConfMgr::createAfterRes();

        pgAssert(!pgConfMgr::getFirstConfigN() && !pgConfMgr::getLastConfigN());

        pgID id1 = pgID::genID();
        pgConf* conf1 = pgConfMgr::newConfig(id1, "dummy", 5);
        pgAssert(pgConfMgr::hasConfig(id1));
        pgAssert(pgConfMgr::getConfig(id1) == conf1);
        pgAssert(pgConfMgr::getFirstConfigN() == conf1 && pgConfMgr::getLastConfigN() == conf1);

        pgAssertThrow(pgConfMgr::newConfig(id1, "dummy", 5), pgConfMgr::ExceptionSameIDExists);

        pgID id2 = pgID::genID();
        pgConf* conf2 = pgConfMgr::newConfig(id2, "dummy", 5);
        pgAssert(pgConfMgr::hasConfig(id2));
        pgAssert(pgConfMgr::getConfig(id2) == conf2);
        pgAssert(pgConfMgr::getFirstConfigN() == conf1 && pgConfMgr::getLastConfigN() == conf2);

        pgConfMgr::deleteConfig(id1);
        pgAssert(!pgConfMgr::hasConfig(id1));
        pgAssert(pgConfMgr::getFirstConfigN() == conf2 && pgConfMgr::getLastConfigN() == conf2);

        pgAssertThrow(pgConfMgr::getConfig(id1), pgConfMgr::ExceptionNotFound);

        pgConfMgr::deleteConfig(id2);
        pgAssert(!pgConfMgr::hasConfig(id2));
        pgAssert(!pgConfMgr::getFirstConfigN() && !pgConfMgr::getLastConfigN());

        pgConfMgr::newConfig(id1, "dummy", 5);
        pgConfMgr::newConfig(id2, "dummy", 5);

        pgAssertThrow(pgConfMgr::hasConfig(pgID::ZERO), pgConfMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgConfMgr::getConfig(pgID::ZERO), pgConfMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgConfMgr::newConfig(pgID::ZERO, "dummy", 5), pgConfMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgConfMgr::newConfig(pgID::genID(), NULL, 5), pgConfMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgConfMgr::newConfig(pgID::genID(), "dummy", 0), pgConfMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgConfMgr::deleteConfig(pgID::ZERO), pgConfMgr::ExceptionInvalidArgument);

        pgConfMgr::destroyBeforeRes();

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);
    }
}
