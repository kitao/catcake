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


static u32 s_init_count1, s_final_count1;
static u32 s_init_count2, s_final_count2;

static u32 s_exinfo = 123;


static void initializer1(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    s_init_count1++;

    *exinfo = &s_exinfo;
}


static void finalizer1(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    s_final_count1++;

    pgAssert(exinfo == &s_exinfo);
}


static void initializer2(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    s_init_count2++;

    *exinfo = &s_exinfo;
}


static void finalizer2(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    s_final_count2++;

    pgAssert(exinfo == &s_exinfo);
}


void pgResMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterTask()
        static void destroyBeforeSys()
    */
    {
        pgAssert(!pgResMgr::isCreated());

        pgResMgr::createAfterTask();
        pgResMgr::createAfterTask();

        pgAssert(pgSysMgr::isCreated());

        pgResMgr::destroyBeforeSys();
        pgResMgr::destroyBeforeSys();

        pgAssert(!pgResMgr::isCreated());
    }

    /*
        static u16 getResourceNum()
        static bool hasResource(pgID id)
        static pgRes getResource(pgID id)
        static void addResource(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free)
        static void removeResource(pgID id)
        static const pgRes* getFirstResourceN()
        static const pgRes* getNextResourceN(pgID id)
    */
    {
        pgAssertThrow(pgResMgr::getResourceNum(), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::hasResource(pgID::ZERO), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::getResource(pgID::ZERO), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::addResource(pgID::ZERO, "", NULL, 0, false), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::removeResource(pgID::ZERO), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::getFirstResourceN(), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::getNextResourceN(pgID::ZERO), pgResMgr::ExceptionNotInitialized);

        pgResMgr::createAfterTask();
        pgAssert(pgResMgr::getResourceNum() == 0);

        pgAssert(!pgResMgr::getFirstResourceN());

        pgID id1 = pgID::genID();
        u32 data1 = 123;

        pgResMgr::addResource(id1, "ABC", &data1, sizeof(data1), false);
        pgAssert(pgResMgr::getResourceNum() == 1);
        pgAssert(pgResMgr::hasResource(id1));
        pgAssert(pgResMgr::getResource(id1).getID() == id1);
        pgAssert(pgResMgr::getFirstResourceN()->getID() == id1);
        pgAssert(!pgResMgr::getNextResourceN(id1));

        pgAssertThrow(pgResMgr::getResource(pgID::genID()), pgResMgr::ExceptionNotFound);
        pgAssertThrow(pgResMgr::addResource(id1, "", &data1, sizeof(data1), false), pgResMgr::ExceptionSameIDExists);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgID id2 = pgID::genID();
        u16* data2 = reinterpret_cast<u16*>(pgMalloc(sizeof(u16)));
        *data2 = 456;

        pgAssert(pgMemMgr::getCurUsedMemorySize() > used_memory_size);

        pgResMgr::addResource(id2, "", data2, sizeof(u16), true);
        pgAssert(pgResMgr::getResourceNum() == 2);
        pgAssert(pgResMgr::hasResource(id2));
        pgAssert(pgResMgr::getResource(id2).getID() == id2);
        pgAssert(pgResMgr::getFirstResourceN()->getID() == id1);
        pgAssert(pgResMgr::getNextResourceN(id1)->getID() == id2);
        pgAssert(!pgResMgr::getNextResourceN(id2));

        pgResMgr::removeResource(id2);
        pgAssert(pgResMgr::getResourceNum() == 1);
        pgAssert(!pgResMgr::hasResource(id2));
        pgAssert(pgResMgr::getFirstResourceN()->getID() == id1);
        pgAssert(!pgResMgr::getNextResourceN(id1));

        pgAssertThrow(pgResMgr::getResource(id2), pgResMgr::ExceptionNotFound);

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgAssertThrow(pgResMgr::hasResource(pgID::ZERO), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::getResource(id2), pgResMgr::ExceptionNotFound);
        pgAssertThrow(pgResMgr::removeResource(id2), pgResMgr::ExceptionNotFound);
        pgAssertThrow(pgResMgr::getResource(pgID::ZERO), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::addResource(pgID::ZERO, "", &data1, sizeof(data1), false), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::addResource(id2, "", NULL, sizeof(data1), false), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::addResource(id2, "", &data1, 0, false), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::removeResource(pgID::ZERO), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::getNextResourceN(pgID::ZERO), pgResMgr::ExceptionInvalidArgument);

        pgResMgr::destroyBeforeSys();
    }

    /*
        static u16 getTypeNum()
        static void addType(pgStr<char, 3> ext, Initializer init, Finalizer final)
        static void removeType(pgStr<char, 3> ext)
        static const pgStr<char, 3>* getFirstTypeN()
        static const pgStr<char, 3>* getNextTypeN(pgStr<char, 3> ext)
    */
    {
        s_init_count1 = s_final_count1 = 0;
        s_init_count2 = s_final_count2 = 0;

        pgAssertThrow(pgResMgr::getTypeNum(), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::addType("", NULL, NULL), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::removeType(""), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::getFirstTypeN(), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::getNextTypeN(""), pgResMgr::ExceptionNotInitialized);

        pgResMgr::createAfterTask();
        pgAssert(pgResMgr::getTypeNum() == 0);

        pgResMgr::addType("aBc", initializer1, finalizer1);
        pgAssert(pgResMgr::getTypeNum() == 1);
        pgAssert(*pgResMgr::getFirstTypeN() == "ABC");
        pgAssert(!pgResMgr::getNextTypeN("ABC"));

        pgAssertThrow(pgResMgr::addType("abC", initializer1, finalizer1), pgResMgr::ExceptionSameExtensionExists);

        pgID id1 = pgID::genID();
        u32 data1 = 123;

        pgResMgr::addResource(id1, "abC", &data1, sizeof(data1), false);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 0);

        pgRes res1 = pgResMgr::getResource(id1);
        pgAssert(res1.getData<u32>() == &data1);

        pgResMgr::removeResource(id1);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 1);

        pgResMgr::addType("Def", initializer2, finalizer2);
        pgAssert(pgResMgr::getTypeNum() == 2);
        pgAssert(*pgResMgr::getFirstTypeN() == "ABC");
        pgAssert(*pgResMgr::getNextTypeN("ABC") == "DEF");
        pgAssert(!pgResMgr::getNextTypeN("DEF"));

        pgID id2 = pgID::genID();
        u32 data2 = 456;

        pgResMgr::addResource(id2, "DEF", &data2, sizeof(data2), false);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 1);
        pgAssert(s_init_count2 == 1 && s_final_count2 == 0);

        pgRes res2 = pgResMgr::getResource(id2);
        pgAssert(res2.getData<u32>() == &data2);

        pgResMgr::removeResource(id2);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 1);
        pgAssert(s_init_count2 == 1 && s_final_count2 == 1);

        pgResMgr::removeType("ABC");
        pgAssert(pgResMgr::getTypeNum() == 1);
        pgAssert(*pgResMgr::getFirstTypeN() == "DEF");
        pgAssert(!pgResMgr::getNextTypeN("DEF"));

        pgResMgr::addResource(id1, "ABC", &data1, sizeof(data1), false);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 1);
        pgAssert(s_init_count2 == 1 && s_final_count2 == 1);

        pgAssertThrow(pgResMgr::removeType("123"), pgResMgr::ExceptionNotFound);
        pgAssertThrow(pgResMgr::addType("", initializer1, finalizer1), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::addType("123", NULL, finalizer1), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::addType("123", initializer1, NULL), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::removeType(""), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::getNextTypeN(""), pgResMgr::ExceptionInvalidArgument);

        pgResMgr::destroyBeforeSys();
    }

    /*
        static void loadResource(const char* filename, bool is_type_detect)
        static void loadResourceAs(pgID id, const char* filename, bool is_type_detect)
    */
    {
        pgAssertThrow(pgResMgr::loadResource(NULL, false), pgResMgr::ExceptionNotInitialized);
        pgAssertThrow(pgResMgr::loadResourceAs(pgID::ZERO, NULL, false), pgResMgr::ExceptionNotInitialized);

        s_init_count1 = s_final_count1 = 0;

        pgResMgr::createAfterTask();

        pgResMgr::addType("DAT", initializer1, finalizer1);

        pgResMgr::loadResource(TEST_DATA_DIR "test_data1.dat", true);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 0);
        pgAssert(pgResMgr::getResourceNum() == 1);

        pgRes res1 = pgResMgr::getResource(pgID_("test_data1.dat"));
        pgAssert(res1.getDataSize() == 10);

        pgAssertThrow(pgResMgr::loadResource(TEST_DATA_DIR "test_data1.dat", true), pgResMgr::ExceptionSameIDExists);
        pgAssertThrow(pgResMgr::loadResource("dummy", true), pgResMgr::ExceptionCannotOpenFile);
        pgAssertThrow(pgResMgr::loadResource(TEST_DATA_DIR "test_data2.dat", true), pgResMgr::ExceptionCannotReadFile);

        pgResMgr::removeResource(pgID_("test_data1.dat"));
        pgAssert(s_init_count1 == 1 && s_final_count1 == 1);
        pgAssert(pgResMgr::getResourceNum() == 0);

        pgResMgr::loadResource(TEST_DATA_DIR "test_data1.dat", false);
        pgAssert(s_init_count1 == 1 && s_final_count1 == 1);
        pgAssert(pgResMgr::getResourceNum() == 1);

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgResMgr::loadResourceAs(pgID_("test1"), TEST_DATA_DIR "test_data1.dat", true);
        pgAssert(s_init_count1 == 2 && s_final_count1 == 1);
        pgAssert(pgResMgr::getResourceNum() == 2);

        pgRes res2 = pgResMgr::getResource(pgID_("test1"));
        pgAssert(res2.getDataSize() == 10);

        pgResMgr::loadResourceAs(pgID_("test2"), TEST_DATA_DIR "test_data1.dat", false);
        pgAssert(s_init_count1 == 2 && s_final_count1 == 1);
        pgAssert(pgResMgr::getResourceNum() == 3);

        pgResMgr::removeResource(pgID_("test1"));
        pgAssert(s_init_count1 == 2 && s_final_count1 == 2);
        pgAssert(pgResMgr::getResourceNum() == 2);

        pgResMgr::removeResource(pgID_("test2"));
        pgAssert(s_init_count1 == 2 && s_final_count1 == 2);
        pgAssert(pgResMgr::getResourceNum() == 1);

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgAssertThrow(pgResMgr::loadResource(NULL, true), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::loadResourceAs(pgID::ZERO, "test", true), pgResMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgResMgr::loadResourceAs(pgID::genID(), NULL, true), pgResMgr::ExceptionInvalidArgument);

        pgResMgr::destroyBeforeSys();
    }
}
