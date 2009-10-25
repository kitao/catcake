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


static u32 s_exinfo = 123;


static void initializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    *exinfo = &s_exinfo;
}


static void finalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    pgAssert(exinfo == &s_exinfo);
}


void pgResTest()
{
    /*
        pgRes()
        pgID getID() const
        pgStr<char, 3> getExtension() const
        template<class T> const T* getData() const
        u32 getDataSize() const
        template<class T> T* getExInfo() const
        bool isAutoFree() const
    */
    {
        pgRes res1;
        pgAssert(res1.getID() == pgID::ZERO);
        pgAssert(res1.getExtension() == "");
        pgAssert(!res1.getData<void>());
        pgAssert(res1.getDataSize() == 0);
        pgAssert(!res1.getExInfo<void>());
        pgAssert(!res1.isAutoFree());

        pgResMgr::createAfterTask();

        pgID id1 = pgID::genID();
        u32 data = 123;

        pgResMgr::addType("TST", initializer, finalizer);
        pgResMgr::addResource(id1, "TST", &data, sizeof(data), false);

        res1 = pgResMgr::getResource(id1);

        pgAssert(res1.getID() == id1);
        pgAssert(res1.getExtension() == "TST");
        pgAssert(res1.getData<u32>() == &data);
        pgAssert(res1.getDataSize() == sizeof(data));
        pgAssert(res1.getExInfo<u32>() == &s_exinfo);
        pgAssert(!res1.isAutoFree());

        pgID id2 = pgID::genID();
        pgResMgr::loadResourceAs(id2, TEST_DATA_DIR "test_data1.dat", true);
        pgRes res2 = pgResMgr::getResource(id2);

        pgAssert(res2.getID() == id2);
        pgAssert(res2.getExtension() == "DAT");
        pgAssert(res2.getData<u32>());
        pgAssert(res2.getDataSize() == 10);
        pgAssert(!res2.getExInfo<u32>());
        pgAssert(res2.isAutoFree());

        pgID id3 = pgID::genID();
        pgResMgr::loadResourceAs(id3, TEST_DATA_DIR "test_data1.dat", false);
        pgRes res3 = pgResMgr::getResource(id3);

        pgAssert(res3.getID() == id3);
        pgAssert(res3.getExtension() == "");
        pgAssert(res3.getData<u32>());
        pgAssert(res3.getDataSize() == 10);
        pgAssert(!res3.getExInfo<u32>());
        pgAssert(res3.isAutoFree());

        pgResMgr::destroyBeforeSys();
    }
}
