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


/*!
    @ingroup ckMem
    The memory manager singleton.
*/
class CK_API ckMemMgr
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionInvalidSizeOfType);
    ckDefineException(ExceptionInvalidVersionOfHeader);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

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
