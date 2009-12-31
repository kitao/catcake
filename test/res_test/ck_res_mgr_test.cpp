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


static u32 s_init_count1, s_final_count1;
static u32 s_init_count2, s_final_count2;

static u32 s_exinfo = 123;


static void initializer1(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    s_init_count1++;

    *exinfo = &s_exinfo;
}


static void finalizer1(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    s_final_count1++;

    ckAssert(exinfo == &s_exinfo);
}


static void initializer2(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    s_init_count2++;

    *exinfo = &s_exinfo;
}


static void finalizer2(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    s_final_count2++;

    ckAssert(exinfo == &s_exinfo);
}


void ckResMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterTask()
        static void destroyBeforeSys()
    */
    {
        ckAssert(!ckResMgr::isCreated());

        ckResMgr::createAfterTask();
        ckResMgr::createAfterTask();

        ckAssert(ckSysMgr::isCreated());

        ckResMgr::destroyBeforeSys();
        ckResMgr::destroyBeforeSys();

        ckAssert(!ckResMgr::isCreated());
    }

    /*
        static u16 getResourceNum()
        static bool hasResource(ckID id)
        static ckRes getResource(ckID id)
        static void addResource(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free)
        static void removeResource(ckID id)
        static const ckRes* getFirstResourceN()
        static const ckRes* getNextResourceN(ckID id)
    */
    {
        ckAssertThrow(ckResMgr::getResourceNum(), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::hasResource(ckID::ZERO), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::getResource(ckID::ZERO), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::addResource(ckID::ZERO, "", NULL, 0, false), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::removeResource(ckID::ZERO), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::getFirstResourceN(), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::getNextResourceN(ckID::ZERO), ckResMgr::ExceptionNotInitialized);

        ckResMgr::createAfterTask();
        ckAssert(ckResMgr::getResourceNum() == 0);

        ckAssert(!ckResMgr::getFirstResourceN());

        ckID id1 = ckID::genID();
        u32 data1 = 123;

        ckResMgr::addResource(id1, "ABC", &data1, sizeof(data1), false);
        ckAssert(ckResMgr::getResourceNum() == 1);
        ckAssert(ckResMgr::hasResource(id1));
        ckAssert(ckResMgr::getResource(id1).getID() == id1);
        ckAssert(ckResMgr::getFirstResourceN()->getID() == id1);
        ckAssert(!ckResMgr::getNextResourceN(id1));

        ckAssertThrow(ckResMgr::getResource(ckID::genID()), ckResMgr::ExceptionNotFound);
        ckAssertThrow(ckResMgr::addResource(id1, "", &data1, sizeof(data1), false), ckResMgr::ExceptionSameIDExists);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckID id2 = ckID::genID();
        u16* data2 = reinterpret_cast<u16*>(ckMalloc(sizeof(u16)));
        *data2 = 456;

        ckAssert(ckMemMgr::getCurUsedMemorySize() > used_memory_size);

        ckResMgr::addResource(id2, "", data2, sizeof(u16), true);
        ckAssert(ckResMgr::getResourceNum() == 2);
        ckAssert(ckResMgr::hasResource(id2));
        ckAssert(ckResMgr::getResource(id2).getID() == id2);
        ckAssert(ckResMgr::getFirstResourceN()->getID() == id1);
        ckAssert(ckResMgr::getNextResourceN(id1)->getID() == id2);
        ckAssert(!ckResMgr::getNextResourceN(id2));

        ckResMgr::removeResource(id2);
        ckAssert(ckResMgr::getResourceNum() == 1);
        ckAssert(!ckResMgr::hasResource(id2));
        ckAssert(ckResMgr::getFirstResourceN()->getID() == id1);
        ckAssert(!ckResMgr::getNextResourceN(id1));

        ckAssertThrow(ckResMgr::getResource(id2), ckResMgr::ExceptionNotFound);

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckAssertThrow(ckResMgr::hasResource(ckID::ZERO), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::getResource(id2), ckResMgr::ExceptionNotFound);
        ckAssertThrow(ckResMgr::removeResource(id2), ckResMgr::ExceptionNotFound);
        ckAssertThrow(ckResMgr::getResource(ckID::ZERO), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::addResource(ckID::ZERO, "", &data1, sizeof(data1), false), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::addResource(id2, "", NULL, sizeof(data1), false), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::addResource(id2, "", &data1, 0, false), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::removeResource(ckID::ZERO), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::getNextResourceN(ckID::ZERO), ckResMgr::ExceptionInvalidArgument);

        ckResMgr::destroyBeforeSys();
    }

    /*
        static u16 getTypeNum()
        static void addType(ckStr<char, 3> ext, Initializer init, Finalizer final)
        static void removeType(ckStr<char, 3> ext)
        static const ckStr<char, 3>* getFirstTypeN()
        static const ckStr<char, 3>* getNextTypeN(ckStr<char, 3> ext)
    */
    {
        s_init_count1 = s_final_count1 = 0;
        s_init_count2 = s_final_count2 = 0;

        ckAssertThrow(ckResMgr::getTypeNum(), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::addType("", NULL, NULL), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::removeType(""), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::getFirstTypeN(), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::getNextTypeN(""), ckResMgr::ExceptionNotInitialized);

        ckResMgr::createAfterTask();
        ckAssert(ckResMgr::getTypeNum() == 0);

        ckResMgr::addType("aBc", initializer1, finalizer1);
        ckAssert(ckResMgr::getTypeNum() == 1);
        ckAssert(*ckResMgr::getFirstTypeN() == "ABC");
        ckAssert(!ckResMgr::getNextTypeN("ABC"));

        ckAssertThrow(ckResMgr::addType("abC", initializer1, finalizer1), ckResMgr::ExceptionSameExtensionExists);

        ckID id1 = ckID::genID();
        u32 data1 = 123;

        ckResMgr::addResource(id1, "abC", &data1, sizeof(data1), false);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 0);

        ckRes res1 = ckResMgr::getResource(id1);
        ckAssert(res1.getData<u32>() == &data1);

        ckResMgr::removeResource(id1);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 1);

        ckResMgr::addType("Def", initializer2, finalizer2);
        ckAssert(ckResMgr::getTypeNum() == 2);
        ckAssert(*ckResMgr::getFirstTypeN() == "ABC");
        ckAssert(*ckResMgr::getNextTypeN("ABC") == "DEF");
        ckAssert(!ckResMgr::getNextTypeN("DEF"));

        ckID id2 = ckID::genID();
        u32 data2 = 456;

        ckResMgr::addResource(id2, "DEF", &data2, sizeof(data2), false);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 1);
        ckAssert(s_init_count2 == 1 && s_final_count2 == 0);

        ckRes res2 = ckResMgr::getResource(id2);
        ckAssert(res2.getData<u32>() == &data2);

        ckResMgr::removeResource(id2);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 1);
        ckAssert(s_init_count2 == 1 && s_final_count2 == 1);

        ckResMgr::removeType("ABC");
        ckAssert(ckResMgr::getTypeNum() == 1);
        ckAssert(*ckResMgr::getFirstTypeN() == "DEF");
        ckAssert(!ckResMgr::getNextTypeN("DEF"));

        ckResMgr::addResource(id1, "ABC", &data1, sizeof(data1), false);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 1);
        ckAssert(s_init_count2 == 1 && s_final_count2 == 1);

        ckAssertThrow(ckResMgr::removeType("123"), ckResMgr::ExceptionNotFound);
        ckAssertThrow(ckResMgr::addType("", initializer1, finalizer1), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::addType("123", NULL, finalizer1), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::addType("123", initializer1, NULL), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::removeType(""), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::getNextTypeN(""), ckResMgr::ExceptionInvalidArgument);

        ckResMgr::destroyBeforeSys();
    }

    /*
        static void loadResource(const char* filename, bool is_type_detect)
        static void loadResourceAs(ckID id, const char* filename, bool is_type_detect)
    */
    {
        ckAssertThrow(ckResMgr::loadResource(NULL, false), ckResMgr::ExceptionNotInitialized);
        ckAssertThrow(ckResMgr::loadResourceAs(ckID::ZERO, NULL, false), ckResMgr::ExceptionNotInitialized);

        s_init_count1 = s_final_count1 = 0;

        ckResMgr::createAfterTask();

        ckResMgr::addType("DAT", initializer1, finalizer1);

        ckResMgr::loadResource(TEST_DATA_DIR "test_data1.dat", true);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 0);
        ckAssert(ckResMgr::getResourceNum() == 1);

        ckRes res1 = ckResMgr::getResource(ckID_("test_data1.dat"));
        ckAssert(res1.getDataSize() == 10);

        ckAssertThrow(ckResMgr::loadResource(TEST_DATA_DIR "test_data1.dat", true), ckResMgr::ExceptionSameIDExists);
        ckAssertThrow(ckResMgr::loadResource("dummy", true), ckResMgr::ExceptionCannotOpenFile);
        ckAssertThrow(ckResMgr::loadResource(TEST_DATA_DIR "test_data2.dat", true), ckResMgr::ExceptionCannotReadFile);

        ckResMgr::removeResource(ckID_("test_data1.dat"));
        ckAssert(s_init_count1 == 1 && s_final_count1 == 1);
        ckAssert(ckResMgr::getResourceNum() == 0);

        ckResMgr::loadResource(TEST_DATA_DIR "test_data1.dat", false);
        ckAssert(s_init_count1 == 1 && s_final_count1 == 1);
        ckAssert(ckResMgr::getResourceNum() == 1);

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckResMgr::loadResourceAs(ckID_("test1"), TEST_DATA_DIR "test_data1.dat", true);
        ckAssert(s_init_count1 == 2 && s_final_count1 == 1);
        ckAssert(ckResMgr::getResourceNum() == 2);

        ckRes res2 = ckResMgr::getResource(ckID_("test1"));
        ckAssert(res2.getDataSize() == 10);

        ckResMgr::loadResourceAs(ckID_("test2"), TEST_DATA_DIR "test_data1.dat", false);
        ckAssert(s_init_count1 == 2 && s_final_count1 == 1);
        ckAssert(ckResMgr::getResourceNum() == 3);

        ckResMgr::removeResource(ckID_("test1"));
        ckAssert(s_init_count1 == 2 && s_final_count1 == 2);
        ckAssert(ckResMgr::getResourceNum() == 2);

        ckResMgr::removeResource(ckID_("test2"));
        ckAssert(s_init_count1 == 2 && s_final_count1 == 2);
        ckAssert(ckResMgr::getResourceNum() == 1);

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckAssertThrow(ckResMgr::loadResource(NULL, true), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::loadResourceAs(ckID::ZERO, "test", true), ckResMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckResMgr::loadResourceAs(ckID::genID(), NULL, true), ckResMgr::ExceptionInvalidArgument);

        ckResMgr::destroyBeforeSys();
    }
}
