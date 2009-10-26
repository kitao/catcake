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
