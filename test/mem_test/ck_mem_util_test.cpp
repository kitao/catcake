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


static u32 s_ctor_count;
static u32 s_dtor_count;


class MemUtilTestClass
{
public:
    MemUtilTestClass()
    {
        s_ctor_count++;
    }

    MemUtilTestClass(u8 dummy)
    {
        s_ctor_count++;
    }

    ~MemUtilTestClass()
    {
        s_dtor_count++;
    }

private:
    u8 m_buf[100];
};


void ckMemUtilTest()
{
    /*
        ckNew(type)
        ckDelete(ptr, type)
    */
    {
        s_ctor_count = 0;
        s_dtor_count = 0;

        ckMemMgr::createFirst();

        const void* mem0 = ckMemMgr::getFirstMemoryBlockN();
        u32 size0 = ckMemMgr::getCurUsedMemorySize();

        MemUtilTestClass* mem1 = ckNew(MemUtilTestClass);
        u32 size1 = sizeof(MemUtilTestClass) + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem1, 255, sizeof(MemUtilTestClass));

        ckAssert(s_ctor_count == 1 && s_dtor_count == 0);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem1));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem1), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem1) == size1);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem1) == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1);

        MemUtilTestClass* mem2 = ckNew(MemUtilTestClass)(123);
        u32 size2 = sizeof(MemUtilTestClass) + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem2, 255, sizeof(MemUtilTestClass));

        ckAssert(s_ctor_count == 2 && s_dtor_count == 0);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem1) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem2), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem2) == size2);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem2) == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        ckAssertThrow(ckDeleteArray(mem1, MemUtilTestClass), ckMemMgr::ExceptionInvalidCall);

        ckDelete(mem1, MemUtilTestClass);

        ckAssert(s_ctor_count == 2 && s_dtor_count == 1);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        ckDelete(mem2, MemUtilTestClass);

        ckAssert(s_ctor_count == 2 && s_dtor_count == 2);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem0));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        ckAssertThrow(ckDelete(NULL, u32), ckMemMgr::ExceptionInvalidArgument);

        ckMemMgr::destroyLast();

        ckAssertThrow(ckNew(u32), ckMemMgr::ExceptionNotInitialized);
    }

    /*
        ckNewArray(var, type, array_size)
        ckDeleteArray(ptr, type)
    */
    {
        s_ctor_count = 0;
        s_dtor_count = 0;

        ckMemMgr::createFirst();

        const void* mem0 = ckMemMgr::getFirstMemoryBlockN();
        u32 size0 = ckMemMgr::getCurUsedMemorySize();

        MemUtilTestClass* mem1;
        ckNewArray(mem1, MemUtilTestClass, 100);
        u32 size1 = sizeof(MemUtilTestClass) * 100 + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem1, 255, sizeof(MemUtilTestClass) * 100);

        ckAssert(s_ctor_count == 100 && s_dtor_count == 0);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem1));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem1), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem1) == size1);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem1) == 100);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1);

        MemUtilTestClass* mem2;
        ckNewArray(mem2, MemUtilTestClass, 200);
        u32 size2 = sizeof(MemUtilTestClass) * 200 + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem2, 255, sizeof(MemUtilTestClass) * 200);

        ckAssert(s_ctor_count == 300 && s_dtor_count == 0);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem1) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem2), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem2) == size2);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem2) == 200);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        ckAssertThrow(ckDelete(mem1, MemUtilTestClass), ckMemMgr::ExceptionInvalidCall);

        ckDeleteArray(mem1, MemUtilTestClass);

        ckAssert(s_ctor_count == 300 && s_dtor_count == 100);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        ckDeleteArray(mem2, MemUtilTestClass);

        ckAssert(s_ctor_count == 300 && s_dtor_count == 300);
        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem0));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        u32* mem3;
        ckAssertThrow(ckNewArray(mem3, u32, 0), ckMemMgr::ExceptionInvalidArgument);

        ckAssertThrow(ckDeleteArray(NULL, u32), ckMemMgr::ExceptionInvalidArgument);

        ckMemMgr::destroyLast();

        ckAssertThrow(ckNewArray(mem3, u32, 10), ckMemMgr::ExceptionNotInitialized);
    }

    /*
        ckMalloc(size)
        ckRealloc(ptr, size)
        ckFree(ptr)
    */
    {
        ckMemMgr::createFirst();

        const void* mem0 = ckMemMgr::getFirstMemoryBlockN();
        u32 size0 = ckMemMgr::getCurUsedMemorySize();

        void* mem1 = ckMalloc(111);
        u32 size1 = 111 + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem1, 255, 111);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem1));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem1), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem1) == size1);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem1) == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1);

        void* mem2 = ckMalloc(222);
        u32 size2 = 222 + ckMemMgr::getMemoryBlockHeaderSize();
        ckMemMgr::memset(mem2, 255, 222);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem1) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem2), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem2) == size2);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem2) == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        mem2 = ckRealloc(mem2, 333);
        u32 size2_realloc = 333 + ckMemMgr::getMemoryBlockHeaderSize();

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem1);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem1) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(isEqual(ckMemMgr::getMemoryBlockName(mem2), __FILE__));
        ckAssert(ckMemMgr::getMemoryBlockSize(mem2) == size2_realloc);
        ckAssert(ckMemMgr::getMemoryBlockArraySize(mem2) == 0);
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size1 + size2_realloc);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);
        ckAssert(*(static_cast<u8*>(mem2) + 221) == 255);

        mem2 = ckRealloc(mem2, 222);

        ckFree(mem1);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(ckMemMgr::getNextMemoryBlockN(mem0) == mem2);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem2));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0 + size2);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        ckFree(mem2);

        ckAssert(ckMemMgr::getFirstMemoryBlockN() == mem0);
        ckAssert(!ckMemMgr::getNextMemoryBlockN(mem0));
        ckAssert(ckMemMgr::getCurUsedMemorySize() == size0);
        ckAssert(ckMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        ckAssertThrow(ckFree(NULL), ckMemMgr::ExceptionInvalidArgument);

        ckMemMgr::destroyLast();

        ckAssertThrow(ckMalloc(100), ckMemMgr::ExceptionNotInitialized);
    }
}
