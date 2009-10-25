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


void pgConfTest1()
{
    /*
        pgConf* getPrevN() const
        pgConf* getNextN() const
        pgID getID() const
    */
    {
        pgConfMgr::createAfterRes();

        pgID id1 = pgID::genID();
        pgConf* conf1 = pgConfMgr::newConfig(id1, "dummy", 5);
        pgAssert(conf1->getID() == id1);
        pgAssert(pgConfMgr::getConfig(id1) == conf1);
        pgAssert(!conf1->getPrevN() && !conf1->getNextN());

        pgID id2 = pgID::genID();
        pgConf* conf2 = pgConfMgr::newConfig(id2, "dummy", 5);
        pgAssert(conf2->getID() == id2);
        pgAssert(pgConfMgr::getConfig(id2) == conf2);
        pgAssert(!conf1->getPrevN() && conf1->getNextN() == conf2);
        pgAssert(conf2->getPrevN() == conf1 && !conf2->getNextN());

        pgConfMgr::deleteConfig(id1);
        pgAssert(!conf2->getPrevN() && !conf2->getNextN());

        pgConfMgr::destroyBeforeRes();
    }

    /*
        bool isValid() const
        u16 getErrorLineNo() const
    */
    {
        pgConfMgr::createAfterRes();

        pgResMgr::loadResource(TEST_DATA_DIR "test_config1.pgc", true);
        pgResMgr::loadResource(TEST_DATA_DIR "test_config2.pgc", true);
        pgResMgr::loadResource(TEST_DATA_DIR "test_config3.pgc", true);
        pgResMgr::loadResource(TEST_DATA_DIR "test_config4.pgc", true);
        pgResMgr::loadResource(TEST_DATA_DIR "test_config5.pgc", true);

        pgConf* conf1 = pgConfMgr::getConfig(pgID_("test_config1.pgc"));
        pgAssert(conf1->isValid());

        pgConf* conf2 = pgConfMgr::getConfig(pgID_("test_config2.pgc"));
        pgAssert(conf2->isValid());

        pgAssertThrow(conf1->getErrorLineNo(), pgConf::ExceptionInvalidCall);
        pgAssertThrow(conf2->getErrorLineNo(), pgConf::ExceptionInvalidCall);

        pgConf* conf3 = pgConfMgr::getConfig(pgID_("test_config3.pgc"));
        pgAssert(!conf3->isValid() && conf3->getErrorLineNo() == 3);

        pgConf* conf4 = pgConfMgr::getConfig(pgID_("test_config4.pgc"));
        pgAssert(!conf4->isValid() && conf4->getErrorLineNo() == 4);

        pgConf* conf5 = pgConfMgr::getConfig(pgID_("test_config5.pgc"));
        pgAssert(!conf5->isValid() && conf5->getErrorLineNo() == 5);

        pgResMgr::removeResource(pgID_("test_config1.pgc"));
        pgResMgr::removeResource(pgID_("test_config2.pgc"));
        pgResMgr::removeResource(pgID_("test_config3.pgc"));
        pgResMgr::removeResource(pgID_("test_config4.pgc"));
        pgResMgr::removeResource(pgID_("test_config5.pgc"));

        pgConfMgr::destroyBeforeRes();
    }

    /*
        u16 getEntryNum(const char* ent_name) const
        pgEnt* getEntryFromFirstN(const char* ent_name) const
        pgEnt* getEntryFromLastN(const char* ent_name) const
        pgEnt* getEntryBeforeN(const char* ent_name, pgEnt* ent) const
        pgEnt* getEntryAfterN(const char* ent_name, pgEnt* ent) const
        pgEnt* getFirstEntryN() const
        pgEnt* getLastEntryN() const
    */
    {
        pgConfMgr::createAfterRes();

        pgResMgr::loadResource(TEST_DATA_DIR "test_config1.pgc", true);

        pgConf* conf = pgConfMgr::getConfig(pgID_("test_config1.pgc"));

        pgAssert(conf->getEntryNum("section") == 3 && conf->getEntryNum("A") == 2 && conf->getEntryNum("test") == 0);

        pgEnt* ent1 = conf->getEntryFromFirstN("section");
        pgAssert(ent1->getValue_s32(0) == 1);

        pgAssert(!conf->getEntryFromFirstN("sectio"));

        pgEnt* ent2 = conf->getEntryFromLastN("section");
        pgAssert(ent2->getValue_s32(0) == 3);

        pgAssert(!conf->getEntryFromLastN("sectio"));

        pgEnt* ent3 = conf->getEntryBeforeN("A", ent2);
        pgAssert(ent3->getValue_s32(0) == 1);

        pgAssert(!conf->getEntryBeforeN("A", ent1));

        pgEnt* ent4 = conf->getEntryAfterN("A", ent2);
        pgAssert(ent4->getValue_s32(0) == 2);

        pgAssert(!conf->getEntryAfterN("section", ent2));

        pgEnt* ent5 = conf->getFirstEntryN();
        pgAssert(ent5->getName() == "section" && ent5->getValue_s32(0) == 1);

        pgEnt* ent6 = conf->getLastEntryN();
        pgAssert(ent6->getName() == "A" && ent6->getValue_s32(0) == 2);

        pgAssertThrow(conf->getEntryFromFirstN(NULL), pgConf::ExceptionInvalidArgument);
        pgAssertThrow(conf->getEntryFromLastN(NULL), pgConf::ExceptionInvalidArgument);
        pgAssertThrow(conf->getEntryBeforeN(NULL, ent1), pgConf::ExceptionInvalidArgument);
        pgAssertThrow(conf->getEntryBeforeN("dummy", NULL), pgConf::ExceptionInvalidArgument);
        pgAssertThrow(conf->getEntryAfterN(NULL, ent1), pgConf::ExceptionInvalidArgument);
        pgAssertThrow(conf->getEntryAfterN("dummy", NULL), pgConf::ExceptionInvalidArgument);

        pgAssertThrow(pgConf::newPriorConfigBeforeInitialization(NULL), pgConf::ExceptionInvalidCall);
        pgAssertThrow(pgConf::deletePriorConfig(conf), pgConf::ExceptionInvalidCall);

        pgResMgr::removeResource(pgID_("test_config1.pgc"));

        pgConfMgr::destroyBeforeRes();
    }
}


void pgConfTest2()
{
    /*
        static pgConf* newPriorConfigBeforeInitialization(const char* filename)
        static void deletePriorConfig(pgConf* conf)
    */
    {
        pgConf* conf = pgConf::newPriorConfigBeforeInitialization(TEST_DATA_DIR "test_config1.pgc");

        pgAssert(conf->isValid() && conf->getEntryNum("section") == 3);

        pgAssertThrow(pgConf::newPriorConfigBeforeInitialization("dummy"), pgConf::ExceptionCannotOpenFile);
        pgAssertThrow(pgConf::newPriorConfigBeforeInitialization(TEST_DATA_DIR "test_data2.dat"), pgConf::ExceptionCannotReadFile);

        pgConf::deletePriorConfig(conf);

        pgAssertThrow(pgConf::newPriorConfigBeforeInitialization(NULL), pgConf::ExceptionInvalidArgument);
        pgAssertThrow(pgConf::deletePriorConfig(NULL), pgConf::ExceptionInvalidArgument);
    }
}
