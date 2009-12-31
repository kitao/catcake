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
void newScenarioPlayer();


static const char* s_cmd[] =
{
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


ckMain()
{
    // special method to load a config file before initializing Catcake
    ckConf* conf = ckConf::newPriorConfigBeforeInitialization("../data/setting.ckl");

    ckEnt* ent1 = conf->getEntryFromFirstN("title");
    ckEnt* ent2 = conf->getEntryFromFirstN("framebuffer_size");
    ckEnt* ent3 = conf->getEntryFromFirstN("frame_rate");
    ckEnt* ent4 = conf->getEntryFromFirstN("resources");

    ckCreateCatcake(ent1->getValue_string(0), ent2->getValue_s32(0), ent2->getValue_s32(1), ent3->getValue_s32(0));
    ckKeyMgr::setMouseVisible(false);

    for (s32 i = 0; i < ent4->getValueNum(); i++)
    {
        ckResMgr::loadResource(ent4->getValue_string(i), true);
    }

    // normal way to load config file
    ckResMgr::loadResource("../data/scenario.ckl", true);

    newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newScenarioPlayer();

    ckStartCatcake();
    ckDestroyCatcake();

    ckConf::deletePriorConfig(conf);
}
