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
void newStage();
void newZombie(u16 zombie_no, r32 x, r32 y, r32 z);


static const char* s_cmd[] =
{
    "ARROW: ROTATE CAMERA", //
    "CLICK ZOMBIE: SHOT ZOMBIE", //
    "F: TOGGLE FULLSCREEN", //
    "Q: QUIT"
};


static void initMotData()
{
    pgMotData mot_data;
    pgMat local;

    mot_data.initAsWriter(6, 3, 1 + 3 + 5);

    /*
        motion 0: normal pose
    */
    mot_data.setMotionInfo(0, 1, 0, 1);
    mot_data.setKeyFramePlayFrame(0, 0);

    /*
        motion 1: walk
    */
    mot_data.setMotionInfo(1, 180, 1, 3);

    mot_data.setKeyFramePlayFrame(1, 0);
    mot_data.setKeyFramePlayFrame(2, 90);
    mot_data.setKeyFramePlayFrame(3, 180);

    mot_data.setNodeLocal(0, 1, pgMat::UNIT.rotateZ_s32(5));
    mot_data.setNodeLocal(0, 2, pgMat::UNIT.rotateZ_s32(-5));
    mot_data.setNodeLocal(0, 3, pgMat::UNIT.rotateZ_s32(5));

    mot_data.setNodeLocal(1, 1, pgMat::UNIT.rotateX_s32(-10));
    mot_data.setNodeLocal(1, 2, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(1, 3, pgMat::UNIT.rotateX_s32(-10));

    mot_data.setNodeLocal(2, 1, pgMat::UNIT.rotateX_s32(3));
    mot_data.setNodeLocal(2, 2, pgMat::UNIT.rotateX_s32(-3));
    mot_data.setNodeLocal(2, 3, pgMat::UNIT.rotateX_s32(3));

    mot_data.setNodeLocal(3, 1, pgMat::UNIT.rotateX_s32(8));
    mot_data.setNodeLocal(3, 2, pgMat::UNIT.rotateX_s32(-5));
    mot_data.setNodeLocal(3, 3, pgMat::UNIT.rotateX_s32(8));

    mot_data.setNodeLocal(4, 1, pgMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));
    mot_data.setNodeLocal(4, 2, pgMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));
    mot_data.setNodeLocal(4, 3, pgMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));

    mot_data.setNodeLocal(5, 1, pgMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));
    mot_data.setNodeLocal(5, 2, pgMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));
    mot_data.setNodeLocal(5, 3, pgMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));

    /*
        motion 2: fall down
    */
    mot_data.setMotionInfo(2, 40, 4, 5);

    mot_data.setKeyFramePlayFrame(4, 0);
    mot_data.setKeyFramePlayFrame(5, 10);
    mot_data.setKeyFramePlayFrame(6, 20);
    mot_data.setKeyFramePlayFrame(7, 30);
    mot_data.setKeyFramePlayFrame(8, 40);

    mot_data.setNodeLocal(0, 4, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(0, 5, pgMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(0, 6, pgMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(0, 7, pgMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(0, 8, pgMat::UNIT.rotateX_s32(10));

    mot_data.setNodeLocal(1, 4, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(1, 5, pgMat::UNIT.translate(0.0f, 16.0f, 56.0f).rotateX_s32(30));
    mot_data.setNodeLocal(1, 6, pgMat::UNIT.translate(0.0f, 8.0f, 88.0f).rotateX_s32(40));
    mot_data.setNodeLocal(1, 7, pgMat::UNIT.translate(0.0f, -16.0f, 108.0f).rotateX_s32(60));
    mot_data.setNodeLocal(1, 8, pgMat::UNIT.translate(0.0f, -32.0f, 116.0f).rotateX_s32(90));

    mot_data.setNodeLocal(2, 4, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(2, 5, pgMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(2, 6, pgMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(2, 7, pgMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(2, 8, pgMat::UNIT.rotateX_s32(0));

    mot_data.setNodeLocal(3, 4, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(3, 5, pgMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(3, 6, pgMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(3, 7, pgMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(3, 8, pgMat::UNIT.rotateX_s32(0));

    mot_data.setNodeLocal(4, 4, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(4, 5, pgMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 6, pgMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 7, pgMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 8, pgMat::UNIT.rotateX_s32(-20));

    mot_data.setNodeLocal(5, 4, pgMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(5, 5, pgMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 6, pgMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 7, pgMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 8, pgMat::UNIT.rotateX_s32(-20));

    /*
        register motion data
    */
    mot_data.registerAsResource(pgID_("zombie.mot"));
}


pgMain()
{
    pgCreatePogolyn("Sample12 - Motion", 640, 480, 60);

    pgResMgr::loadResource("../data/pogolyn_logo_71x14.png", true);
    pgUtil::loadPixelArtModel("../data/pogolyn_icon.pxm", "../data/pogolyn_icon.png", 4.0f);
    pgUtil::loadPixelArtModel("../data/zombie.pxm", "../data/zombie.png", 8.0f);
    pgUtil::loadPixelArtModel("../data/blood.pxm", "../data/blood.png", 4.0f);

    initMotData();

    newPogolynLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newStage();
    newZombie(0, -20.0f, 64.0f, -150.0f);
    newZombie(1, 100.0f, 64.0f, 50.0f);
    newZombie(2, -120.0f, 64.0f, 150.0f);

    pgStartPogolyn();
    pgDestroyPogolyn();
}
