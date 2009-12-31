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
void newEmbossFont();
void newWaterdrop(r32 x, r32 y);


static const char* s_cmd[] =
{
    "U: ROTATE/STOP UV OFFSET", //
    "R: CHANGE REFRACTION INDEX", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


ckMain()
{
    ckCreateCatcake("Sample07 - Shader", 640, 480, 60);

    ckResMgr::loadResource("../data/catcake_logo_71x14.png", true);
    ckResMgr::loadResource("../data/bg_512x512.png", true);
    ckResMgr::loadResource("../data/stonsans.ttf", true);
    ckResMgr::loadResource("../data/waterdrop_64x64.png", true);
    ckUtil::loadShader(ckID_("refract.shd"), "../data/refract.vert", "../data/refract.frag", 1, 2, 2);

    ckMath::srand(static_cast<u32>(ckSysMgr::getUsecTime()));

    newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newEmbossFont();

    for (s32 i = 0; i < 64; i++)
    {
        newWaterdrop(ckMath::rand(-320.0f, 320.0f, 1.0f), ckMath::rand(-320.0f, 320.0f, 1.0f));
    }

    ckStartCatcake();
    ckDestroyCatcake();
}
