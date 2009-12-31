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
