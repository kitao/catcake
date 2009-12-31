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
