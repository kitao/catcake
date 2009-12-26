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


#include "catcake_main.h"


void newHelloCatcake();


ckMain()
{
    ckCreateCatcake("Hello Catcake", 320, 480, 30);

    /*
        The resource files need to be copied to the target by using 'adb push' command like this:
            adb push hello_catcake/data /data/data/catcake_application.hello_catcake/data

        And they must be specified by the absolute paths in ckResMgr::loadResource.
    */
    ckResMgr::loadResource("/data/data/catcake_application.hello_catcake/data/catcake_logo_71x14.png", true);
    ckResMgr::loadResource("/data/data/catcake_application.hello_catcake/data/stonsans.ttf", true);

    newHelloCatcake();

    /*
        ckStartCatcake doesn't block in Catcake for Android.
        So you must not write finalization process such as ckDestroyCatcake after it.
    */
    ckStartCatcake();
}
