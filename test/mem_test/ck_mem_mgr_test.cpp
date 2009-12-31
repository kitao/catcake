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


void ckMemMgrTest()
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

        ckMemMgr::memset(buf1, 123, 50);
        ckMemMgr::memset(buf1 + 50, 234, 50);

        for (s32 i = 0; i < 100; i++)
        {
            ckAssert(buf1[i] == (i < 50) ? 123 : 234);
        }

        ckAssertThrow(ckMemMgr::memset(NULL, 123, 10), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::memset(buf1, 123, 0), ckMemMgr::ExceptionInvalidArgument);

        u8 buf2[100];

        ckMemMgr::memset(buf2, 123, 100);
        ckMemMgr::memcpy(buf2, buf1 + 50, 50);

        for (s32 i = 0; i < 100; i++)
        {
            ckAssert(buf2[i] == (i < 50) ? 234 : 123);
        }

        ckAssertThrow(ckMemMgr::memcpy(NULL, buf2, 10), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::memcpy(buf1, NULL, 10), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::memcpy(buf1, buf2, 0), ckMemMgr::ExceptionInvalidArgument);
    }

    /*
        static bool isCreated()
        static void createFirst(u32 catcake_version = CATCAKE_VERSION)
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
        ckAssertThrow(ckMemMgr::getCurUsedMemorySize(), ckMemMgr::ExceptionNotInitialized);
        ckAssertThrow(ckMemMgr::getMaxUsedMemorySize(), ckMemMgr::ExceptionNotInitialized);
        ckAssertThrow(ckMemMgr::getFirstMemoryBlockN(), ckMemMgr::ExceptionNotInitialized);
        ckAssertThrow(ckMemMgr::getNextMemoryBlockN(NULL), ckMemMgr::ExceptionNotInitialized);
        ckAssertThrow(ckMemMgr::mallocForSystem(100, 0, __FILE__), ckMemMgr::ExceptionNotInitialized);
        ckAssertThrow(ckMemMgr::reallocForSystem(NULL, 0, 0, __FILE__), ckMemMgr::ExceptionNotInitialized);

        ckMemMgr::freeForSystem(NULL);

        ckAssert(!ckMemMgr::isCreated());

        ckAssertThrow(ckMemMgr::createFirst(CATCAKE_VERSION + 1), ckMemMgr::ExceptionInvalidVersionOfHeader);

        ckMemMgr::createFirst();
        ckMemMgr::createFirst();

        ckAssert(ckMemMgr::isCreated());

        const void* mem0 = ckMemMgr::getFirstMemoryBlockN();
        ckAssert(mem0);

        u32 size0 = ckMemMgr::getCurUsedMemorySize();
        ckAssert(size0 > 0);

        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0);

        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem0));

        static const char name1[] = "mem1";
        void* mem1 = ckMemMgr::mallocForSystem(111, 0, name1);
        u32 size1 = 111 + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem1, 255, 111);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem1));
        ckAssert(ckMemMgr::getMemoryBlockName(mem1) == name1);
        ckAssert(ckMemMgr::getMemoryBlockSize(mem1) == size1);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem1) == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1);

        static const char name2[] = "mem2";
        void* mem2 = ckMemMgr::mallocForSystem(222, 123, name2);
        u32 size2 = 222 + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem2, 255, 222);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem1) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(ckMemMgr::getMemoryBlockName(mem2) == name2);
        ckAssert(ckMemMgr::getMemoryBlockSize(mem2) == size2);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem2) == 123);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        static const char name2_realloc[] = "mem2_realloc";
        mem2 = ckMemMgr::reallocForSystem(mem2, 300, 234, name2_realloc);
        u32 size2_realloc = 300 + ckMemMgr::getMemoryBlockHeaderSize();

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem1) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(ckMemMgr::getMemoryBlockName(mem2) == name2_realloc);
        ckAssert(ckMemMgr::getMemoryBlockSize(mem2) == size2_realloc);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem2) == 234);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1 + size2_realloc);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);
        ckAssert(*(static_cast<u8*>(mem2) + 221) == 255);

        mem2 = ckMemMgr::reallocForSystem(mem2, 222, 123, name2);

        ckMemMgr::freeForSystem(mem1);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        ckMemMgr::freeForSystem(mem2);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem0));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        ckAssertThrow(ckMemMgr::getNextMemoryBlockN(NULL), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::getMemoryBlockName(NULL), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::getMemoryBlockSize(NULL), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::getMemoryBlockArraySize(NULL), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::mallocForSystem(0, 100, __FILE__), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::mallocForSystem(100, 100, NULL), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::reallocForSystem(NULL, 100, 0, __FILE__), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::reallocForSystem(reinterpret_cast<void*>(100), 0, 0, __FILE__), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::reallocForSystem(reinterpret_cast<void*>(100), 100, 0, NULL), ckMemMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckMemMgr::freeForSystem(NULL), ckMemMgr::ExceptionInvalidArgument);

        ckMemMgr::mallocForSystem(123, 0, __FILE__);
        ckMemMgr::mallocForSystem(456, 78, __FILE__);

        ckMemMgr::destroyLast();
        ckMemMgr::destroyLast();

        ckAssert(!ckMemMgr::isCreated());
    }

    /*
        static void* allocTempBufferForSystem(u32 size)
        static u32 getTempBufferSizeForSystem()
    */
    {
        ckAssertThrow(ckMemMgr::allocTempBufferForSystem(0), ckMemMgr::ExceptionNotInitialized);
        ckAssertThrow(ckMemMgr::getTempBufferSizeForSystem(), ckMemMgr::ExceptionNotInitialized);

        ckMemMgr::createFirst();

        ckAssert(ckMemMgr::getTempBufferSizeForSystem() == 1024);

        ckAssertThrow(ckMemMgr::allocTempBufferForSystem(0), ckMemMgr::ExceptionInvalidArgument);

        void* buf1 = ckMemMgr::allocTempBufferForSystem(1000);
        ckMemMgr::memset(buf1, 255, 1000);

        ckAssert(buf1 != NULL);
        ckAssert(ckMemMgr::getTempBufferSizeForSystem() == 1024);

        void* buf2 = ckMemMgr::allocTempBufferForSystem(2000);
        ckMemMgr::memset(buf2, 255, 2000);

        ckAssert(buf2 != NULL);
        ckAssert(ckMemMgr::getTempBufferSizeForSystem() == 2048);

        void* buf3 = ckMemMgr::allocTempBufferForSystem(3000);
        ckMemMgr::memset(buf3, 255, 3000);

        ckAssert(buf3 != NULL);
        ckAssert(ckMemMgr::getTempBufferSizeForSystem() == 4096);

        void* buf4 = ckMemMgr::allocTempBufferForSystem(100);
        ckMemMgr::memset(buf4, 255, 100);

        ckAssert(buf4 == buf3);
        ckAssert(ckMemMgr::getTempBufferSizeForSystem() == 4096);

        ckMemMgr::destroyLast();
    }
}
