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
