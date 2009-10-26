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


#include "ck_mem_all.h"

#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckMemMgr* ckMemMgr::m_instance = NULL;


void ckMemMgr::memset(void* buf, u8 value, u32 size)
{
    if (!buf || size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::memset(buf, value, size);
}


void ckMemMgr::memcpy(void* dest, const void* src, u32 size)
{
    if (!dest || !src || size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::memcpy(dest, src, size);
}


CK_DEFINE_MANAGER_IS_CREATED(ckMemMgr)


void ckMemMgr::createFirst(u32 catcake_version)
{
    if (catcake_version != CATCAKE_VERSION)
    {
        ckThrow(ExceptionInvalidVersionOfHeader);
    }

    if (!(sizeof(s8) == 1 && sizeof(s16) == 2 && sizeof(s32) == 4 && sizeof(s64) == 8 && //
        sizeof(u8) == 1 && sizeof(u16) == 2 && sizeof(u32) == 4 && sizeof(u64) == 8 && //
        sizeof(r32) == 4 && sizeof(r64) == 8))
    {
        ckThrow(ExceptionInvalidSizeOfType);
    }

    destroyLast();

    m_instance = new(ckLowLevelAPI::malloc(sizeof(ckMemMgr)), NULL) ckMemMgr;

    m_instance->m_temp_buf_size = INITIAL_TEMP_BUFFER_SIZE;
    m_instance->m_temp_buf = ckMalloc(m_instance->m_temp_buf_size);
}


void ckMemMgr::destroyLast()
{
    if (m_instance)
    {
        m_instance->~ckMemMgr();
        ckLowLevelAPI::free(m_instance);
    }
}


u32 ckMemMgr::getCurUsedMemorySize()
{
    return instance()->m_cur_used_memory_size;
}


u32 ckMemMgr::getMaxUsedMemorySize()
{
    return instance()->m_max_used_memory_size;
}


const void* ckMemMgr::getFirstMemoryBlockN()
{
    ckMemMgr* ins = instance();

    return (ins->m_mbh_start.next != &ins->m_mbh_end) ? ins->m_mbh_start.next + 1 : NULL;
}


const void* ckMemMgr::getNextMemoryBlockN(const void* ptr)
{
    ckMemMgr* ins = instance();

    if (!ptr)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    const MemoryBlockHeader* mbh = reinterpret_cast<const MemoryBlockHeader*>(ptr) - 1;

    return (mbh->next != &ins->m_mbh_end) ? mbh->next + 1 : NULL;
}


const char* ckMemMgr::getMemoryBlockName(const void* ptr)
{
    if (!ptr)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return (reinterpret_cast<const MemoryBlockHeader*>(ptr) - 1)->name;
}


u32 ckMemMgr::getMemoryBlockSize(const void* ptr)
{
    if (!ptr)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return (reinterpret_cast<const MemoryBlockHeader*>(ptr) - 1)->size;
}


u32 ckMemMgr::getMemoryBlockArraySize(const void* ptr)
{
    if (!ptr)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return (reinterpret_cast<const MemoryBlockHeader*>(ptr) - 1)->array_size;
}


u32 ckMemMgr::getMemoryBlockHeaderSize()
{
    return sizeof(MemoryBlockHeader);
}


void* ckMemMgr::mallocForSystem(u32 size, u32 array_size, const char* name)
{
    ckMemMgr* ins = instance();

    if (size == 0 || !name)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    size += sizeof(MemoryBlockHeader);

    MemoryBlockHeader* mbh = reinterpret_cast<MemoryBlockHeader*>(ckLowLevelAPI::malloc(size));

    mbh->prev = ins->m_mbh_end.prev;
    mbh->next = &ins->m_mbh_end;
    mbh->name = name;
    mbh->size = size;
    mbh->array_size = array_size;

    mbh->prev->next = mbh;
    mbh->next->prev = mbh;

    ins->m_cur_used_memory_size += size;

    if (ins->m_cur_used_memory_size > ins->m_max_used_memory_size)
    {
        ins->m_max_used_memory_size = ins->m_cur_used_memory_size;
    }

    return mbh + 1;
}


void* ckMemMgr::reallocForSystem(void* ptr, u32 size, u32 array_size, const char* name)
{
    ckMemMgr* ins = instance();

    if (!ptr || size == 0 || !name)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    size += sizeof(MemoryBlockHeader);

    MemoryBlockHeader* mbh = reinterpret_cast<MemoryBlockHeader*>(ptr) - 1;

    ins->m_cur_used_memory_size -= mbh->size;

    mbh = reinterpret_cast<MemoryBlockHeader*>(ckLowLevelAPI::realloc(mbh, size));

    mbh->name = name;
    mbh->size = size;
    mbh->array_size = array_size;

    mbh->prev->next = mbh;
    mbh->next->prev = mbh;

    ins->m_cur_used_memory_size += size;

    if (ins->m_cur_used_memory_size > ins->m_max_used_memory_size)
    {
        ins->m_max_used_memory_size = ins->m_cur_used_memory_size;
    }

    return mbh + 1;
}


void ckMemMgr::freeForSystem(void* ptr)
{
    if (!isCreated())
    {
        return;
    }

    ckMemMgr* ins = instance();

    if (!ptr)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    MemoryBlockHeader* mbh = reinterpret_cast<MemoryBlockHeader*>(ptr) - 1;

    ins->m_cur_used_memory_size -= mbh->size;

    mbh->prev->next = mbh->next;
    mbh->next->prev = mbh->prev;

    ckLowLevelAPI::free(mbh);
}


void* ckMemMgr::allocTempBufferForSystem(u32 size)
{
    ckMemMgr* ins = instance();

    if (size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (size > ins->m_temp_buf_size)
    {
        while (size > ins->m_temp_buf_size)
        {
            ins->m_temp_buf_size *= 2;
        }

        ckFree(ins->m_temp_buf);

        ins->m_temp_buf = ckMalloc(ins->m_temp_buf_size);
    }

    return ins->m_temp_buf;
}


u32 ckMemMgr::getTempBufferSizeForSystem()
{
    return instance()->m_temp_buf_size;
}


ckMemMgr::ckMemMgr()
{
    m_mbh_start.prev = NULL;
    m_mbh_start.next = &m_mbh_end;
    m_mbh_start.name = __FILE__;
    m_mbh_start.size = 0;
    m_mbh_start.array_size = 0;

    m_mbh_end.prev = &m_mbh_start;
    m_mbh_end.next = NULL;
    m_mbh_end.name = __FILE__;
    m_mbh_end.size = 0;
    m_mbh_end.array_size = 0;

    m_temp_buf = NULL;
    m_temp_buf_size = 0;

    m_cur_used_memory_size = m_max_used_memory_size = sizeof(ckMemMgr);
}


ckMemMgr::~ckMemMgr()
{
    while (void* ptr = const_cast<void*>(getFirstMemoryBlockN()))
    {
        ckFree(ptr);
    }

    m_instance = NULL;
}


CK_DEFINE_OPERATOR_EQUAL(ckMemMgr)


CK_DEFINE_MANAGER_INSTANCE(ckMemMgr)
