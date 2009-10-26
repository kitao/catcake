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
