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
    ::vswprintf(buf, buf_size, format, *reinterpret_cast<va_list*>(arg));
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
