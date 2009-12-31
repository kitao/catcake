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


#include <stdlib.h>
#include <string.h>

#include "ck_low_level_api.h"

#ifndef CK_NO_THROW_EXCEPTION
#include <new>
#endif


void* ckLowLevelAPI::malloc(u32 size)
{
    void* ptr = ::malloc(size);

    if (!ptr)
    {
#ifdef CK_NO_THROW_EXCEPTION
        error("bad_alloc");
#else
        throw std::bad_alloc();
#endif
    }

    return ptr;
}


void* ckLowLevelAPI::realloc(void* ptr, u32 size)
{
    ptr = ::realloc(ptr, size);

    if (!ptr)
    {
#ifdef CK_NO_THROW_EXCEPTION
        error("bad_alloc");
#else
        throw std::bad_alloc();
#endif
    }

    return ptr;
}


void ckLowLevelAPI::free(void* ptr)
{
    ::free(ptr);
}


void ckLowLevelAPI::memset(void* buf, u8 value, u32 size)
{
    ::memset(buf, value, size);
}


void ckLowLevelAPI::memcpy(void* dest, const void* src, u32 size)
{
    ::memcpy(dest, src, size);
}
