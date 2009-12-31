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


void newCatcakeLogo(const char** cmd, u8 cmd_num);
void newTriangle();
void newAmeba(r32 x, r32 y, ckCol color);


static const char* s_cmd[] =
{
    "P: CHANGE PRIM MODE", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


ckMain()
{
    ckCreateCatcake("Sample02 - Primitive", 640, 480, 60);

    ckResMgr::loadResource("../data/catcake_logo_71x14.png", true);
    ckResMgr::loadResource("../data/coin_400x200.png", true);

    newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newTriangle();
    newAmeba(0.0f, 0.0f, ckCol(128, 128, 128));
    newAmeba(-200.0f, 120.0f, ckCol(255, 128, 64));
    newAmeba(0.0f, 240.0f, ckCol(64, 255, 128));
    newAmeba(200.0f, 120.0f, ckCol(128, 64, 255));
    newAmeba(-200.0f, -120.0f, ckCol(255, 64, 128));
    newAmeba(0.0f, -240.0f, ckCol(128, 255, 64));
    newAmeba(200.0f, -120.0f, ckCol(64, 128, 255));

    ckStartCatcake();
    ckDestroyCatcake();
}
