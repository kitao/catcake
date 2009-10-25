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


void pgMemMgrTest()
{
    /*
        static void memset(void* buf, u8 value, u32 size)
        static void memcpy(void* dest, const void* src, u32 size)
    */
    {
        u8 buf1[100];

        for (s32 i = 0; i < 100; i++)
        {
            buf1[i] = 0;
        }

        pgMemMgr::memset(buf1, 123, 50);
        pgMemMgr::memset(buf1 + 50, 234, 50);

        for (s32 i = 0; i < 100; i++)
        {
            pgAssert(buf1[i] == (i < 50) ? 123 : 234);
        }

        pgAssertThrow(pgMemMgr::memset(NULL, 123, 10), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::memset(buf1, 123, 0), pgMemMgr::ExceptionInvalidArgument);

        u8 buf2[100];

        pgMemMgr::memset(buf2, 123, 100);
        pgMemMgr::memcpy(buf2, buf1 + 50, 50);

        for (s32 i = 0; i < 100; i++)
        {
            pgAssert(buf2[i] == (i < 50) ? 234 : 123);
        }

        pgAssertThrow(pgMemMgr::memcpy(NULL, buf2, 10), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::memcpy(buf1, NULL, 10), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::memcpy(buf1, buf2, 0), pgMemMgr::ExceptionInvalidArgument);
    }

    /*
        static bool isCreated()
        static void createFirst(u32 pogolyn_version = POGOLYN_VERSION)
        static void destroyLast()
        static u32 getCurUsedMemorySize()
        static u32 getMaxUsedMemorySize()
        static const void* getFirstMemoryBlockN()
        static const void* getNextMemoryBlockN(const void* ptr)
        static const char* getMemoryBlockName(const void* ptr)
        static u32 getMemoryBlockSize(const void* ptr)
        static u32 getMemoryBlockArraySize(const void* ptr)
        static u32 getMemoryBlockHeaderSize()

        static void* mallocForSystem(u32 size, u32 array_size, const char* name)
        static void* reallocForSystem(void* ptr, u32 size, u32 array_size, const char* name)
        static void freeForSystem(void* ptr)
    */
    {
        pgAssertThrow(pgMemMgr::getCurUsedMemorySize(), pgMemMgr::ExceptionNotInitialized);
        pgAssertThrow(pgMemMgr::getMaxUsedMemorySize(), pgMemMgr::ExceptionNotInitialized);
        pgAssertThrow(pgMemMgr::getFirstMemoryBlockN(), pgMemMgr::ExceptionNotInitialized);
        pgAssertThrow(pgMemMgr::getNextMemoryBlockN(NULL), pgMemMgr::ExceptionNotInitialized);
        pgAssertThrow(pgMemMgr::mallocForSystem(100, 0, __FILE__), pgMemMgr::ExceptionNotInitialized);
        pgAssertThrow(pgMemMgr::reallocForSystem(NULL, 0, 0, __FILE__), pgMemMgr::ExceptionNotInitialized);

        pgMemMgr::freeForSystem(NULL);

        pgAssert(!pgMemMgr::isCreated());

        pgAssertThrow(pgMemMgr::createFirst(POGOLYN_VERSION + 1), pgMemMgr::ExceptionInvalidVersionOfHeader);

        pgMemMgr::createFirst();
        pgMemMgr::createFirst();

        pgAssert(pgMemMgr::isCreated());

        const void* mem0 = pgMemMgr::getFirstMemoryBlockN();
        pgAssert(mem0);

        u32 size0 = pgMemMgr::getCurUsedMemorySize();
        pgAssert(size0 > 0);

        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0);

        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem0));

        static const char name1[] = "mem1";
        void* mem1 = pgMemMgr::mallocForSystem(111, 0, name1);
        u32 size1 = 111 + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem1, 255, 111);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem1));
        pgAssert(pgMemMgr::getMemoryBlockName(mem1) == name1);
        pgAssert(pgMemMgr::getMemoryBlockSize(mem1) == size1);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem1) == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1);

        static const char name2[] = "mem2";
        void* mem2 = pgMemMgr::mallocForSystem(222, 123, name2);
        u32 size2 = 222 + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem2, 255, 222);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem1) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(pgMemMgr::getMemoryBlockName(mem2) == name2);
        pgAssert(pgMemMgr::getMemoryBlockSize(mem2) == size2);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem2) == 123);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        static const char name2_realloc[] = "mem2_realloc";
        mem2 = pgMemMgr::reallocForSystem(mem2, 300, 234, name2_realloc);
        u32 size2_realloc = 300 + pgMemMgr::getMemoryBlockHeaderSize();

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem1) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(pgMemMgr::getMemoryBlockName(mem2) == name2_realloc);
        pgAssert(pgMemMgr::getMemoryBlockSize(mem2) == size2_realloc);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem2) == 234);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1 + size2_realloc);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);
        pgAssert(*(static_cast<u8*>(mem2) + 221) == 255);

        mem2 = pgMemMgr::reallocForSystem(mem2, 222, 123, name2);

        pgMemMgr::freeForSystem(mem1);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        pgMemMgr::freeForSystem(mem2);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem0));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        pgAssertThrow(pgMemMgr::getNextMemoryBlockN(NULL), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::getMemoryBlockName(NULL), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::getMemoryBlockSize(NULL), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::getMemoryBlockArraySize(NULL), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::mallocForSystem(0, 100, __FILE__), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::mallocForSystem(100, 100, NULL), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::reallocForSystem(NULL, 100, 0, __FILE__), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::reallocForSystem(reinterpret_cast<void*>(100), 0, 0, __FILE__), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::reallocForSystem(reinterpret_cast<void*>(100), 100, 0, NULL), pgMemMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgMemMgr::freeForSystem(NULL), pgMemMgr::ExceptionInvalidArgument);

        pgMemMgr::mallocForSystem(123, 0, __FILE__);
        pgMemMgr::mallocForSystem(456, 78, __FILE__);

        pgMemMgr::destroyLast();
        pgMemMgr::destroyLast();

        pgAssert(!pgMemMgr::isCreated());
    }

    /*
        static void* allocTempBufferForSystem(u32 size)
        static u32 getTempBufferSizeForSystem()
    */
    {
        pgAssertThrow(pgMemMgr::allocTempBufferForSystem(0), pgMemMgr::ExceptionNotInitialized);
        pgAssertThrow(pgMemMgr::getTempBufferSizeForSystem(), pgMemMgr::ExceptionNotInitialized);

        pgMemMgr::createFirst();

        pgAssert(pgMemMgr::getTempBufferSizeForSystem() == 1024);

        pgAssertThrow(pgMemMgr::allocTempBufferForSystem(0), pgMemMgr::ExceptionInvalidArgument);

        void* buf1 = pgMemMgr::allocTempBufferForSystem(1000);
        pgMemMgr::memset(buf1, 255, 1000);

        pgAssert(buf1 != NULL);
        pgAssert(pgMemMgr::getTempBufferSizeForSystem() == 1024);

        void* buf2 = pgMemMgr::allocTempBufferForSystem(2000);
        pgMemMgr::memset(buf2, 255, 2000);

        pgAssert(buf2 != NULL);
        pgAssert(pgMemMgr::getTempBufferSizeForSystem() == 2048);

        void* buf3 = pgMemMgr::allocTempBufferForSystem(3000);
        pgMemMgr::memset(buf3, 255, 3000);

        pgAssert(buf3 != NULL);
        pgAssert(pgMemMgr::getTempBufferSizeForSystem() == 4096);

        void* buf4 = pgMemMgr::allocTempBufferForSystem(100);
        pgMemMgr::memset(buf4, 255, 100);

        pgAssert(buf4 == buf3);
        pgAssert(pgMemMgr::getTempBufferSizeForSystem() == 4096);

        pgMemMgr::destroyLast();
    }
}
