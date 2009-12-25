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
    The operator new.
    @param[in] type The type of a class.
    @return An instance of a class.
*/
#define ckNew(type) new(ckMemMgr::mallocForSystem(sizeof(type), 0, __FILE__), NULL) type

/*!
    @ingroup ckMem
    The operator delete.
    @param[in] ptr An instance to be deleted.
    @param[in] type The type of a class.
*/
#define ckDelete(ptr, type) \
    do \
    { \
        if (!ckMemMgr::isCreated()) \
        { \
            break; \
        } \
    \
        void* ptr_ = ptr; \
    \
        if (ckMemMgr::getMemoryBlockArraySize(ptr_) > 0) \
        { \
            ckThrow(ckMemMgr::ExceptionInvalidCall); \
        } \
    \
        reinterpret_cast<type*>(ptr_)->~type(); \
        ckMemMgr::freeForSystem(ptr_); \
    } \
    while (false)

/*!
    @ingroup ckMem
    The operator new for an array.
    @param[out] var The created instance.
    @param[in] type The type of a class.
    @param[in] array_size The size of an array.
*/
#define ckNewArray(var, type, array_size) \
    do \
    { \
        var = reinterpret_cast<type*>(ckMemMgr::mallocForSystem(sizeof(type) * array_size, array_size, __FILE__)); \
    \
        for (u32 i = 0; i < array_size; i++) \
        { \
            new(&var[i], NULL) type; \
        } \
    } \
    while (false)

/*!
    @ingroup ckMem
    The operator delete for an array.
    @param[in] ptr The instance to be deleted.
    @param[in] type The type of a class.
*/
#define ckDeleteArray(ptr, type) \
    do \
    { \
        if (!ckMemMgr::isCreated()) \
        { \
            break; \
        } \
    \
        void* ptr_ = ptr; \
        u32 array_size = ckMemMgr::getMemoryBlockArraySize(ptr_); \
    \
        if (array_size == 0) \
        { \
            ckThrow(ckMemMgr::ExceptionInvalidCall); \
        } \
    \
        for (u32 i = 0; i < array_size; i++) \
        { \
            reinterpret_cast<type*>(ptr_)[i].~type(); \
        } \
    \
        ckMemMgr::freeForSystem(ptr_); \
    } \
    while (false)

/*!
    @ingroup ckMem
    Allocates a memory.
    @param[in] size The size of memory.
    @return The memory.
*/
#define ckMalloc(size) ckMemMgr::mallocForSystem(size, 0, __FILE__)

/*!
    @ingroup ckMem
    Resizes a memory.
    @param[in] ptr The memory to be resized.
    @param[in] size The new size of the memory.
    @return The memory.
*/
#define ckRealloc(ptr, size) ckMemMgr::reallocForSystem(ptr, size, 0, __FILE__)

/*!
    @ingroup ckMem
    Releases a memory.
    @param[in] ptr The memory to be released.
*/
#define ckFree(ptr) ckMemMgr::freeForSystem(ptr)

/*!
    @ingroup ckMem
    The dummy operator new for system.
    @param[in] ptr The pointer to be allocated.
    @return The memory.
*/
CK_API void* operator new(size_t, void* ptr, ckException*);

/*!
    @ingroup ckMem
    The dummy operator delete for system.
*/
CK_API void operator delete(void*, void*, ckException*);
