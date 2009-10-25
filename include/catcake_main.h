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


#ifndef POGOLYN_MAIN_H_
#define POGOLYN_MAIN_H_


#ifdef WIN32
#include <windows.h>
#endif

#include "pogolyn.h"


#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "pogolynd.lib")
#else
#pragma comment(lib, "pogolyn.lib")
#endif
#endif


/*!
    @ingroup pgSys
    The entry function of Pogolyn.
*/
#ifdef WIN32
#define pgMain() \
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
    { \
        pgSysMgr::setInitialDirectoryForSystem(__argc, __argv); \
    \
        extern void pgMain_(); \
        pgMain_(); \
    \
        return 0; \
    } \
    void pgMain_()
#else
#define pgMain() \
    int main(int argc, char* argv[]) \
    { \
        pgSysMgr::setInitialDirectoryForSystem(argc, argv); \
    \
        extern void pgMain_(); \
        pgMain_(); \
    \
        return 0; \
    } \
    void pgMain_()
#endif


#endif // !POGOLYN_MAIN_H_
