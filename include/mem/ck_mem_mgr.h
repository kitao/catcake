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


/*!
    @ingroup pgMem
    TODO
*/
class PG_API pgMemMgr
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionInvalidSizeOfType);
    pgDefineException(ExceptionInvalidVersionOfHeader);
    pgDefineException(ExceptionNotInitialized);

    static void memset(void* buf, u8 value, u32 size);
    static void memcpy(void* dest, const void* src, u32 size);

    static bool isCreated();
    static void createFirst(u32 pogolyn_version = POGOLYN_VERSION);
    static void destroyLast();

    static u32 getCurUsedMemorySize();
    static u32 getMaxUsedMemorySize();

    static const void* getFirstMemoryBlockN();
    static const void* getNextMemoryBlockN(const void* ptr);
    static const char* getMemoryBlockName(const void* ptr);
    static u32 getMemoryBlockSize(const void* ptr);
    static u32 getMemoryBlockArraySize(const void* ptr);
    static u32 getMemoryBlockHeaderSize();

    static void* mallocForSystem(u32 size, u32 array_size, const char* name);
    static void* reallocForSystem(void* ptr, u32 size, u32 array_size, const char* name);
    static void freeForSystem(void* ptr);
    static void* allocTempBufferForSystem(u32 size);
    static u32 getTempBufferSizeForSystem();

private:
    static const u32 INITIAL_TEMP_BUFFER_SIZE = 1024;

    struct MemoryBlockHeader
    {
        MemoryBlockHeader* prev;
        MemoryBlockHeader* next;
        const char* name;
        u32 size;
        u32 array_size;
    };

    pgMemMgr();
    ~pgMemMgr();
    void operator=(const pgMemMgr&);

    static pgMemMgr* instance();

    MemoryBlockHeader m_mbh_start;
    MemoryBlockHeader m_mbh_end;
    u32 m_cur_used_memory_size;
    u32 m_max_used_memory_size;
    void* m_temp_buf;
    u32 m_temp_buf_size;

    static pgMemMgr* m_instance;
};
