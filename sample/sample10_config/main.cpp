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
