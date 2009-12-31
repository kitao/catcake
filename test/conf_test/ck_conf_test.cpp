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


void ckConfTest1()
{
    /*
        ckConf* getPrevN() const
        ckConf* getNextN() const
        ckID getID() const
    */
    {
        ckConfMgr::createAfterRes();

        ckID id1 = ckID::genID();
        ckConf* conf1 = ckConfMgr::newConfig(id1, "dummy", 5);
        ckAssert(conf1->getID() == id1);
        ckAssert(ckConfMgr::getConfig(id1) == conf1);
        ckAssert(!conf1->getPrevN() && !conf1->getNextN());

        ckID id2 = ckID::genID();
        ckConf* conf2 = ckConfMgr::newConfig(id2, "dummy", 5);
        ckAssert(conf2->getID() == id2);
        ckAssert(ckConfMgr::getConfig(id2) == conf2);
        ckAssert(!conf1->getPrevN() && conf1->getNextN() == conf2);
        ckAssert(conf2->getPrevN() == conf1 && !conf2->getNextN());

        ckConfMgr::deleteConfig(id1);
        ckAssert(!conf2->getPrevN() && !conf2->getNextN());

        ckConfMgr::destroyBeforeRes();
    }

    /*
        bool isValid() const
        u16 getErrorLineNo() const
    */
    {
        ckConfMgr::createAfterRes();

        ckResMgr::loadResource(TEST_DATA_DIR "test_config1.ckl", true);
        ckResMgr::loadResource(TEST_DATA_DIR "test_config2.ckl", true);
        ckResMgr::loadResource(TEST_DATA_DIR "test_config3.ckl", true);
        ckResMgr::loadResource(TEST_DATA_DIR "test_config4.ckl", true);
        ckResMgr::loadResource(TEST_DATA_DIR "test_config5.ckl", true);

        ckConf* conf1 = ckConfMgr::getConfig(ckID_("test_config1.ckl"));
        ckAssert(conf1->isValid());

        ckConf* conf2 = ckConfMgr::getConfig(ckID_("test_config2.ckl"));
        ckAssert(conf2->isValid());

        ckAssertThrow(conf1->getErrorLineNo(), ckConf::ExceptionInvalidCall);
        ckAssertThrow(conf2->getErrorLineNo(), ckConf::ExceptionInvalidCall);

        ckConf* conf3 = ckConfMgr::getConfig(ckID_("test_config3.ckl"));
        ckAssert(!conf3->isValid() && conf3->getErrorLineNo() == 3);

        ckConf* conf4 = ckConfMgr::getConfig(ckID_("test_config4.ckl"));
        ckAssert(!conf4->isValid() && conf4->getErrorLineNo() == 4);

        ckConf* conf5 = ckConfMgr::getConfig(ckID_("test_config5.ckl"));
        ckAssert(!conf5->isValid() && conf5->getErrorLineNo() == 5);

        ckResMgr::removeResource(ckID_("test_config1.ckl"));
        ckResMgr::removeResource(ckID_("test_config2.ckl"));
        ckResMgr::removeResource(ckID_("test_config3.ckl"));
        ckResMgr::removeResource(ckID_("test_config4.ckl"));
        ckResMgr::removeResource(ckID_("test_config5.ckl"));

        ckConfMgr::destroyBeforeRes();
    }

    /*
        u16 getEntryNum(const char* ent_name) const
        ckEnt* getEntryFromFirstN(const char* ent_name) const
        ckEnt* getEntryFromLastN(const char* ent_name) const
        ckEnt* getEntryBeforeN(const char* ent_name, ckEnt* ent) const
        ckEnt* getEntryAfterN(const char* ent_name, ckEnt* ent) const
        ckEnt* getFirstEntryN() const
        ckEnt* getLastEntryN() const
    */
    {
        ckConfMgr::createAfterRes();

        ckResMgr::loadResource(TEST_DATA_DIR "test_config1.ckl", true);

        ckConf* conf = ckConfMgr::getConfig(ckID_("test_config1.ckl"));

        ckAssert(conf->getEntryNum("section") == 3 && conf->getEntryNum("A") == 2 && conf->getEntryNum("test") == 0);

        ckEnt* ent1 = conf->getEntryFromFirstN("section");
        ckAssert(ent1->getValue_s32(0) == 1);

        ckAssert(!conf->getEntryFromFirstN("sectio"));

        ckEnt* ent2 = conf->getEntryFromLastN("section");
        ckAssert(ent2->getValue_s32(0) == 3);

        ckAssert(!conf->getEntryFromLastN("sectio"));

        ckEnt* ent3 = conf->getEntryBeforeN("A", ent2);
        ckAssert(ent3->getValue_s32(0) == 1);

        ckAssert(!conf->getEntryBeforeN("A", ent1));

        ckEnt* ent4 = conf->getEntryAfterN("A", ent2);
        ckAssert(ent4->getValue_s32(0) == 2);

        ckAssert(!conf->getEntryAfterN("section", ent2));

        ckEnt* ent5 = conf->getFirstEntryN();
        ckAssert(ent5->getName() == "section" && ent5->getValue_s32(0) == 1);

        ckEnt* ent6 = conf->getLastEntryN();
        ckAssert(ent6->getName() == "A" && ent6->getValue_s32(0) == 2);

        ckAssertThrow(conf->getEntryFromFirstN(NULL), ckConf::ExceptionInvalidArgument);
        ckAssertThrow(conf->getEntryFromLastN(NULL), ckConf::ExceptionInvalidArgument);
        ckAssertThrow(conf->getEntryBeforeN(NULL, ent1), ckConf::ExceptionInvalidArgument);
        ckAssertThrow(conf->getEntryBeforeN("dummy", NULL), ckConf::ExceptionInvalidArgument);
        ckAssertThrow(conf->getEntryAfterN(NULL, ent1), ckConf::ExceptionInvalidArgument);
        ckAssertThrow(conf->getEntryAfterN("dummy", NULL), ckConf::ExceptionInvalidArgument);

        ckAssertThrow(ckConf::newPriorConfigBeforeInitialization(NULL), ckConf::ExceptionInvalidCall);
        ckAssertThrow(ckConf::deletePriorConfig(conf), ckConf::ExceptionInvalidCall);

        ckResMgr::removeResource(ckID_("test_config1.ckl"));

        ckConfMgr::destroyBeforeRes();
    }
}


void ckConfTest2()
{
    /*
        static ckConf* newPriorConfigBeforeInitialization(const char* filename)
        static void deletePriorConfig(ckConf* conf)
    */
    {
        ckConf* conf = ckConf::newPriorConfigBeforeInitialization(TEST_DATA_DIR "test_config1.ckl");

        ckAssert(conf->isValid() && conf->getEntryNum("section") == 3);

        ckAssertThrow(ckConf::newPriorConfigBeforeInitialization("dummy"), ckConf::ExceptionCannotOpenFile);
        ckAssertThrow(ckConf::newPriorConfigBeforeInitialization(TEST_DATA_DIR "test_data2.dat"), ckConf::ExceptionCannotReadFile);

        ckConf::deletePriorConfig(conf);

        ckAssertThrow(ckConf::newPriorConfigBeforeInitialization(NULL), ckConf::ExceptionInvalidArgument);
        ckAssertThrow(ckConf::deletePriorConfig(NULL), ckConf::ExceptionInvalidArgument);
    }
}
