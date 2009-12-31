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


static u32 s_exinfo = 123;


static void initializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    *exinfo = &s_exinfo;
}


static void finalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    ckAssert(exinfo == &s_exinfo);
}


void ckResTest()
{
    /*
        ckRes()
        ckID getID() const
        ckStr<char, 3> getExtension() const
        template<class T> const T* getData() const
        u32 getDataSize() const
        template<class T> T* getExInfo() const
        bool isAutoFree() const
    */
    {
        ckRes res1;
        ckAssert(res1.getID() == ckID::ZERO);
        ckAssert(res1.getExtension() == "");
        ckAssert(!res1.getData<void>());
        ckAssert(res1.getDataSize() == 0);
        ckAssert(!res1.getExInfo<void>());
        ckAssert(!res1.isAutoFree());

        ckResMgr::createAfterTask();

        ckID id1 = ckID::genID();
        u32 data = 123;

        ckResMgr::addType("TST", initializer, finalizer);
        ckResMgr::addResource(id1, "TST", &data, sizeof(data), false);

        res1 = ckResMgr::getResource(id1);

        ckAssert(res1.getID() == id1);
        ckAssert(res1.getExtension() == "TST");
        ckAssert(res1.getData<u32>() == &data);
        ckAssert(res1.getDataSize() == sizeof(data));
        ckAssert(res1.getExInfo<u32>() == &s_exinfo);
        ckAssert(!res1.isAutoFree());

        ckID id2 = ckID::genID();
        ckResMgr::loadResourceAs(id2, TEST_DATA_DIR "test_data1.dat", true);
        ckRes res2 = ckResMgr::getResource(id2);

        ckAssert(res2.getID() == id2);
        ckAssert(res2.getExtension() == "DAT");
        ckAssert(res2.getData<u32>());
        ckAssert(res2.getDataSize() == 10);
        ckAssert(!res2.getExInfo<u32>());
        ckAssert(res2.isAutoFree());

        ckID id3 = ckID::genID();
        ckResMgr::loadResourceAs(id3, TEST_DATA_DIR "test_data1.dat", false);
        ckRes res3 = ckResMgr::getResource(id3);

        ckAssert(res3.getID() == id3);
        ckAssert(res3.getExtension() == "");
        ckAssert(res3.getData<u32>());
        ckAssert(res3.getDataSize() == 10);
        ckAssert(!res3.getExInfo<u32>());
        ckAssert(res3.isAutoFree());

        ckResMgr::destroyBeforeSys();
    }
}
