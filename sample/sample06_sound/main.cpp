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


#include "pogolyn_main.h"


void newPogolynLogo(const char** cmd, u8 cmd_num);
void newSoundMonitor();
void newSoundMixer();


static const char* s_cmd[] =
{
    "CLICK & DRAG: CONTROL SOUND", //
    "1-6: CHANGE SOUND QUALITY", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


pgMain()
{
    pgCreatePogolyn("Sample06 - Sound", 640, 480, 30);

    pgResMgr::loadResource("../data/pogolyn_logo_71x14.png", true);
    pgResMgr::loadResource("../data/mono_11khz.wav", true);
    pgResMgr::loadResource("../data/mono_22khz.wav", true);
    pgResMgr::loadResource("../data/mono_44khz.wav", true);
    pgResMgr::loadResource("../data/stereo_11khz.wav", true);
    pgResMgr::loadResource("../data/stereo_22khz.wav", true);
    pgResMgr::loadResource("../data/stereo_44khz.wav", true);

    newPogolynLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newSoundMonitor();
    newSoundMixer();

    pgStartPogolyn();
    pgDestroyPogolyn();
}
