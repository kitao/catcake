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


void pgMemUtilTest()
{
    /*
        pgNew(type)
        pgDelete(ptr, type)
    */
    {
        s_ctor_count = 0;
        s_dtor_count = 0;

        pgMemMgr::createFirst();

        const void* mem0 = pgMemMgr::getFirstMemoryBlockN();
        u32 size0 = pgMemMgr::getCurUsedMemorySize();

        MemUtilTestClass* mem1 = pgNew(MemUtilTestClass);
        u32 size1 = sizeof(MemUtilTestClass) + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem1, 255, sizeof(MemUtilTestClass));

        pgAssert(s_ctor_count == 1 && s_dtor_count == 0);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem1));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem1), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem1) == size1);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem1) == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1);

        MemUtilTestClass* mem2 = pgNew(MemUtilTestClass)(123);
        u32 size2 = sizeof(MemUtilTestClass) + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem2, 255, sizeof(MemUtilTestClass));

        pgAssert(s_ctor_count == 2 && s_dtor_count == 0);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem1) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem2), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem2) == size2);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem2) == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        pgAssertThrow(pgDeleteArray(mem1, MemUtilTestClass), pgMemMgr::ExceptionInvalidCall);

        pgDelete(mem1, MemUtilTestClass);

        pgAssert(s_ctor_count == 2 && s_dtor_count == 1);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        pgDelete(mem2, MemUtilTestClass);

        pgAssert(s_ctor_count == 2 && s_dtor_count == 2);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem0));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        pgAssertThrow(pgDelete(NULL, u32), pgMemMgr::ExceptionInvalidArgument);

        pgMemMgr::destroyLast();

        pgAssertThrow(pgNew(u32), pgMemMgr::ExceptionNotInitialized);
    }

    /*
        pgNewArray(var, type, array_size)
        pgDeleteArray(ptr, type)
    */
    {
        s_ctor_count = 0;
        s_dtor_count = 0;

        pgMemMgr::createFirst();

        const void* mem0 = pgMemMgr::getFirstMemoryBlockN();
        u32 size0 = pgMemMgr::getCurUsedMemorySize();

        MemUtilTestClass* mem1;
        pgNewArray(mem1, MemUtilTestClass, 100);
        u32 size1 = sizeof(MemUtilTestClass) * 100 + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem1, 255, sizeof(MemUtilTestClass) * 100);

        pgAssert(s_ctor_count == 100 && s_dtor_count == 0);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem1));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem1), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem1) == size1);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem1) == 100);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1);

        MemUtilTestClass* mem2;
        pgNewArray(mem2, MemUtilTestClass, 200);
        u32 size2 = sizeof(MemUtilTestClass) * 200 + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem2, 255, sizeof(MemUtilTestClass) * 200);

        pgAssert(s_ctor_count == 300 && s_dtor_count == 0);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem1) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem2), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem2) == size2);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem2) == 200);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        pgAssertThrow(pgDelete(mem1, MemUtilTestClass), pgMemMgr::ExceptionInvalidCall);

        pgDeleteArray(mem1, MemUtilTestClass);

        pgAssert(s_ctor_count == 300 && s_dtor_count == 100);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        pgDeleteArray(mem2, MemUtilTestClass);

        pgAssert(s_ctor_count == 300 && s_dtor_count == 300);
        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem0));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        u32* mem3;
        pgAssertThrow(pgNewArray(mem3, u32, 0), pgMemMgr::ExceptionInvalidArgument);

        pgAssertThrow(pgDeleteArray(NULL, u32), pgMemMgr::ExceptionInvalidArgument);

        pgMemMgr::destroyLast();

        pgAssertThrow(pgNewArray(mem3, u32, 10), pgMemMgr::ExceptionNotInitialized);
    }

    /*
        pgMalloc(size)
        pgRealloc(ptr, size)
        pgFree(ptr)
    */
    {
        pgMemMgr::createFirst();

        const void* mem0 = pgMemMgr::getFirstMemoryBlockN();
        u32 size0 = pgMemMgr::getCurUsedMemorySize();

        void* mem1 = pgMalloc(111);
        u32 size1 = 111 + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem1, 255, 111);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem1));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem1), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem1) == size1);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem1) == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1);

        void* mem2 = pgMalloc(222);
        u32 size2 = 222 + pgMemMgr::getMemoryBlockHeaderSize();
        pgMemMgr::memset(mem2, 255, 222);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem1) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem2), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem2) == size2);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem2) == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2);

        mem2 = pgRealloc(mem2, 333);
        u32 size2_realloc = 333 + pgMemMgr::getMemoryBlockHeaderSize();

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem1);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem1) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(isEqual(pgMemMgr::getMemoryBlockName(mem2), __FILE__));
        pgAssert(pgMemMgr::getMemoryBlockSize(mem2) == size2_realloc);
        pgAssert(pgMemMgr::getMemoryBlockArraySize(mem2) == 0);
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size1 + size2_realloc);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);
        pgAssert(*(static_cast<u8*>(mem2) + 221) == 255);

        mem2 = pgRealloc(mem2, 222);

        pgFree(mem1);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(pgMemMgr::getNextMemoryBlockN(mem0) == mem2);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem2));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0 + size2);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        pgFree(mem2);

        pgAssert(pgMemMgr::getFirstMemoryBlockN() == mem0);
        pgAssert(!pgMemMgr::getNextMemoryBlockN(mem0));
        pgAssert(pgMemMgr::getCurUsedMemorySize() == size0);
        pgAssert(pgMemMgr::getMaxUsedMemorySize() == size0 + size1 + size2_realloc);

        pgAssertThrow(pgFree(NULL), pgMemMgr::ExceptionInvalidArgument);

        pgMemMgr::destroyLast();

        pgAssertThrow(pgMalloc(100), pgMemMgr::ExceptionNotInitialized);
    }
}
