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
#define pgNew(type) new(pgMemMgr::mallocForSystem(sizeof(type), 0, __FILE__), NULL) type

/*!
    @ingroup pgMem
    TODO
*/
#define pgDelete(ptr, type) \
    do \
    { \
        if (!pgMemMgr::isCreated()) \
        { \
            break; \
        } \
    \
        void* ptr_ = ptr; \
    \
        if (pgMemMgr::getMemoryBlockArraySize(ptr_) > 0) \
        { \
            pgThrow(pgMemMgr::ExceptionInvalidCall); \
        } \
    \
        reinterpret_cast<type*>(ptr_)->~type(); \
        pgMemMgr::freeForSystem(ptr_); \
    } \
    while (false)

/*!
    @ingroup pgMem
    TODO
*/
#define pgNewArray(var, type, array_size) \
    do \
    { \
        var = reinterpret_cast<type*>(pgMemMgr::mallocForSystem(sizeof(type) * array_size, array_size, __FILE__)); \
    \
        for (u32 i = 0; i < array_size; i++) \
        { \
            new(&var[i], NULL) type; \
        } \
    } \
    while (false)

/*!
    @ingroup pgMem
    TODO
*/
#define pgDeleteArray(ptr, type) \
    do \
    { \
        if (!pgMemMgr::isCreated()) \
        { \
            break; \
        } \
    \
        void* ptr_ = ptr; \
        u32 array_size = pgMemMgr::getMemoryBlockArraySize(ptr_); \
    \
        if (array_size == 0) \
        { \
            pgThrow(pgMemMgr::ExceptionInvalidCall); \
        } \
    \
        for (u32 i = 0; i < array_size; i++) \
        { \
            reinterpret_cast<type*>(ptr_)[i].~type(); \
        } \
    \
        pgMemMgr::freeForSystem(ptr_); \
    } \
    while (false)

/*!
    @ingroup pgMem
    TODO
*/
#define pgMalloc(size) pgMemMgr::mallocForSystem(size, 0, __FILE__)

/*!
    @ingroup pgMem
    TODO
*/
#define pgRealloc(ptr, size) pgMemMgr::reallocForSystem(ptr, size, 0, __FILE__)

/*!
    @ingroup pgMem
    TODO
*/
#define pgFree(ptr) pgMemMgr::freeForSystem(ptr)


PG_API void* operator new(size_t, void* ptr, pgException*);
PG_API void operator delete(void*, void*, pgException*);
