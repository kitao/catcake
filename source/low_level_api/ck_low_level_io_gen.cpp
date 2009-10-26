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


#ifdef _MSC_VER
#include <windows.h>
#else
#include <wchar.h>
#endif

#include <stdio.h>
#include <stdarg.h>

#include "ck_low_level_api.h"


void ckLowLevelAPI::printf(const char* msg)
{
    ::printf("%s", msg);

#ifdef _MSC_VER
    OutputDebugString(msg);
#endif
}


void ckLowLevelAPI::wprintf(const wchar_t* msg)
{
    ::wprintf(msg);

#ifdef _MSC_VER
    OutputDebugStringW(msg);
#endif
}


void ckLowLevelAPI::vsprintf(char* buf, u32 buf_size, const char* format, void* arg)
{
#if _MSC_VER >= 1400
    ::vsprintf_s(buf, buf_size, format, *reinterpret_cast<va_list*>(arg));
#else
    ::vsnprintf(buf, buf_size, format, *reinterpret_cast<va_list*>(arg));
#endif
}


void ckLowLevelAPI::vswprintf(wchar_t* buf, u32 buf_size, const wchar_t* format, void* arg)
{
#ifdef CK_MINGW
    ::vswprintf(buf, format, *reinterpret_cast<va_list*>(arg));
#else
    ::vswprintf(buf, buf_size, format, *reinterpret_cast<va_list*>(arg));
#endif
}


void* ckLowLevelAPI::openFile(const char* filename, FileMode file_mode)
{
#if _MSC_VER >= 1400
    FILE* fp;
    return fopen_s(&fp, filename, (file_mode == FILE_MODE_READ) ? "rb" : "wb") ? NULL : fp;
#else
    return fopen(filename, (file_mode == FILE_MODE_READ) ? "rb" : "wb");
#endif
}


s32 ckLowLevelAPI::getFileSize(void* file_handler)
{
    FILE* fp = reinterpret_cast<FILE*>(file_handler);

    return (ftell(fp) >= 0 && !fseek(fp, 0, SEEK_END)) ? ftell(fp) : -1;
}


bool ckLowLevelAPI::readFile(void* buf, u32 offset, u32 size, void* file_handler)
{
    FILE* fp = reinterpret_cast<FILE*>(file_handler);

    return (!fseek(fp, offset, SEEK_SET) && (fread(buf, size, 1, fp) == 1));
}


bool ckLowLevelAPI::writeFile(u32 offset, const void* buf, u32 size, void* file_handler)
{
    FILE* fp = reinterpret_cast<FILE*>(file_handler);

    return (!fseek(fp, offset, SEEK_SET) && (fwrite(buf, size, 1, fp) == 1));
}


void ckLowLevelAPI::closeFile(void* file_handler)
{
    fclose(reinterpret_cast<FILE*>(file_handler));
}
