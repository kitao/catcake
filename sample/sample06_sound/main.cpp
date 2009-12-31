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
void newSoundMonitor();
void newSoundMixer();


static const char* s_cmd[] =
{
    "CLICK & DRAG: CONTROL SOUND", //
    "1-6: CHANGE SOUND QUALITY", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


ckMain()
{
    ckCreateCatcake("Sample06 - Sound", 640, 480, 30);

    ckResMgr::loadResource("../data/catcake_logo_71x14.png", true);
    ckResMgr::loadResource("../data/mono_11khz.wav", true);
    ckResMgr::loadResource("../data/mono_22khz.wav", true);
    ckResMgr::loadResource("../data/mono_44khz.wav", true);
    ckResMgr::loadResource("../data/stereo_11khz.wav", true);
    ckResMgr::loadResource("../data/stereo_22khz.wav", true);
    ckResMgr::loadResource("../data/stereo_44khz.wav", true);

    newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newSoundMonitor();
    newSoundMixer();

    ckStartCatcake();
    ckDestroyCatcake();
}
