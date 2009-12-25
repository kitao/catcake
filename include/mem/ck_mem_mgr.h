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
    @ingroup ckMem
    The memory manager singleton.
*/
class CK_API ckMemMgr
{
public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionInvalidSizeOfType);
    ckDefineException(ExceptionInvalidVersionOfHeader);
    ckDefineException(ExceptionNotInitialized);

    /*!
        Fills the memory by the specified value.
        @param[out] buf A memory.
        @param[in] value A value.
        @param[in] size The size of a memory.
    */
    static void memset(void* buf, u8 value, u32 size);

    /*!
        Copies the memory.
        @param[out] dest A destination memory.
        @param[in] src A source memory.
        @param[in] size The size of the memory.
    */
    static void memcpy(void* dest, const void* src, u32 size);

    /*!
        Returns whether the memory manager singleton is created.
        @return Whether the memory manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the memory manager singleton.
        @param[in] catcake_version The version of Catcake.
    */
    static void createFirst(u32 catcake_version = CATCAKE_VERSION);

    /*!
        Destroys the memory manager singleton.
    */
    static void destroyLast();

    /*!
        Returns the size of the used memory.
        @return The size of the used memory.
    */
    static u32 getCurUsedMemorySize();

    /*!
        Returns the maximum size of the used memory.
        @return The maximum size of the used memory.
    */
    static u32 getMaxUsedMemorySize();

    /*!
        Returns the first memory block. If the first memory block doesn't exist, return NULL.
        @return The first memory block.
    */
    static const void* getFirstMemoryBlockN();

    /*!
        Returns the next memory block of the specified memory. If the first memory block doesn't exist, return NULL.
        @param[in] ptr The pointer to the memory.
        @return The next memory block of the specified memory.
    */
    static const void* getNextMemoryBlockN(const void* ptr);

    /*!
        Returns the name of the specified memory.
        @param[in] ptr The pointer to the memory.
        @return The name of the specified memory.
    */
    static const char* getMemoryBlockName(const void* ptr);

    /*!
        Returns the size of the specified memory block.
        @param[in] ptr The pointer to a memory block.
        @return The size of the specified memory block.
    */
    static u32 getMemoryBlockSize(const void* ptr);

    /*!
        Returns the size of the specified memory block array.
        @param[in] ptr The pointer to a memory block array.
        @return The size of the specified memory block array.
    */
    static u32 getMemoryBlockArraySize(const void* ptr);

    /*!
        Returns the size of the memory block header.
        @return The size of the memory block header.
    */
    static u32 getMemoryBlockHeaderSize();

    /*!
        Allocates a memory. This method is only for system.
        @param[in] size The size of memory.
        @param[in] array_size The size of an array.
        @param[in] name The name of memory.
        @return An allocated memory.
    */
    static void* mallocForSystem(u32 size, u32 array_size, const char* name);

    /*!
        Resizes the specified memory. This method is only for system.
        @param[in,out] ptr The pointer to a memory.
        @param[in] size The size of a memory.
        @param[in] array_size The size of an array.
        @param[in] name The name of a memory.
    */
    static void* reallocForSystem(void* ptr, u32 size, u32 array_size, const char* name);

    /*!
        Releases the specified memory. This method is only for system.
        @param[in] ptr The pointer to a memory.
    */
    static void freeForSystem(void* ptr);

    /*!
        Allocates the temporary buffer. This method is only for system.
        @param[in] size The size of the temporary buffer.
        @return The temporary buffer.
    */
    static void* allocTempBufferForSystem(u32 size);

    /*!
        Returns the size of the temporary buffer. This method is only for system.
        @return The size of the temporary buffer.
    */
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

    ckMemMgr();
    ~ckMemMgr();
    void operator=(const ckMemMgr&);

    static ckMemMgr* instance();

    MemoryBlockHeader m_mbh_start;
    MemoryBlockHeader m_mbh_end;
    u32 m_cur_used_memory_size;
    u32 m_max_used_memory_size;
    void* m_temp_buf;
    u32 m_temp_buf_size;

    static ckMemMgr* m_instance;
};
