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


#ifndef CATCAKE_MAIN_H_
#define CATCAKE_MAIN_H_


#ifdef WIN32
#include <windows.h>
#endif

#include "catcake.h"


#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "catcaked.lib")
#else
#pragma comment(lib, "catcake.lib")
#endif
#endif


/*!
    @ingroup ckSys
    The entry function of the application.
*/
#ifdef WIN32
#define ckMain() \
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
    { \
        ckSysMgr::setInitialDirectoryForSystem(__argc, __argv); \
    \
        extern void ckMain_(); \
        ckMain_(); \
    \
        return 0; \
    } \
    void ckMain_()
#else
#define ckMain() \
    int main(int argc, char* argv[]) \
    { \
        ckSysMgr::setInitialDirectoryForSystem(argc, argv); \
    \
        extern void ckMain_(); \
        ckMain_(); \
    \
        return 0; \
    } \
    void ckMain_()
#endif


#endif // !CATCAKE_MAIN_H_
