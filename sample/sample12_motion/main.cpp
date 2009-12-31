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
    ckMotData mot_data;
    ckMat local;

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

    mot_data.setNodeLocal(0, 1, ckMat::UNIT.rotateZ_s32(5));
    mot_data.setNodeLocal(0, 2, ckMat::UNIT.rotateZ_s32(-5));
    mot_data.setNodeLocal(0, 3, ckMat::UNIT.rotateZ_s32(5));

    mot_data.setNodeLocal(1, 1, ckMat::UNIT.rotateX_s32(-10));
    mot_data.setNodeLocal(1, 2, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(1, 3, ckMat::UNIT.rotateX_s32(-10));

    mot_data.setNodeLocal(2, 1, ckMat::UNIT.rotateX_s32(3));
    mot_data.setNodeLocal(2, 2, ckMat::UNIT.rotateX_s32(-3));
    mot_data.setNodeLocal(2, 3, ckMat::UNIT.rotateX_s32(3));

    mot_data.setNodeLocal(3, 1, ckMat::UNIT.rotateX_s32(8));
    mot_data.setNodeLocal(3, 2, ckMat::UNIT.rotateX_s32(-5));
    mot_data.setNodeLocal(3, 3, ckMat::UNIT.rotateX_s32(8));

    mot_data.setNodeLocal(4, 1, ckMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));
    mot_data.setNodeLocal(4, 2, ckMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));
    mot_data.setNodeLocal(4, 3, ckMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));

    mot_data.setNodeLocal(5, 1, ckMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));
    mot_data.setNodeLocal(5, 2, ckMat::UNIT.rotateX_s32(10).translate(0.0f, 0.0f, -8.0f));
    mot_data.setNodeLocal(5, 3, ckMat::UNIT.rotateX_s32(-10).translate(0.0f, 0.0f, 8.0f));

    /*
        motion 2: fall down
    */
    mot_data.setMotionInfo(2, 40, 4, 5);

    mot_data.setKeyFramePlayFrame(4, 0);
    mot_data.setKeyFramePlayFrame(5, 10);
    mot_data.setKeyFramePlayFrame(6, 20);
    mot_data.setKeyFramePlayFrame(7, 30);
    mot_data.setKeyFramePlayFrame(8, 40);

    mot_data.setNodeLocal(0, 4, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(0, 5, ckMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(0, 6, ckMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(0, 7, ckMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(0, 8, ckMat::UNIT.rotateX_s32(10));

    mot_data.setNodeLocal(1, 4, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(1, 5, ckMat::UNIT.translate(0.0f, 16.0f, 56.0f).rotateX_s32(30));
    mot_data.setNodeLocal(1, 6, ckMat::UNIT.translate(0.0f, 8.0f, 88.0f).rotateX_s32(40));
    mot_data.setNodeLocal(1, 7, ckMat::UNIT.translate(0.0f, -16.0f, 108.0f).rotateX_s32(60));
    mot_data.setNodeLocal(1, 8, ckMat::UNIT.translate(0.0f, -32.0f, 116.0f).rotateX_s32(90));

    mot_data.setNodeLocal(2, 4, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(2, 5, ckMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(2, 6, ckMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(2, 7, ckMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(2, 8, ckMat::UNIT.rotateX_s32(0));

    mot_data.setNodeLocal(3, 4, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(3, 5, ckMat::UNIT.rotateX_s32(-30));
    mot_data.setNodeLocal(3, 6, ckMat::UNIT.rotateX_s32(-25));
    mot_data.setNodeLocal(3, 7, ckMat::UNIT.rotateX_s32(-20));
    mot_data.setNodeLocal(3, 8, ckMat::UNIT.rotateX_s32(0));

    mot_data.setNodeLocal(4, 4, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(4, 5, ckMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 6, ckMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 7, ckMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(4, 8, ckMat::UNIT.rotateX_s32(-20));

    mot_data.setNodeLocal(5, 4, ckMat::UNIT.rotateX_s32(0));
    mot_data.setNodeLocal(5, 5, ckMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 6, ckMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 7, ckMat::UNIT.rotateX_s32(30));
    mot_data.setNodeLocal(5, 8, ckMat::UNIT.rotateX_s32(-20));

    /*
        register motion data
    */
    mot_data.registerAsResource(ckID_("zombie.mot"));
}


ckMain()
{
    ckCreateCatcake("Sample12 - Motion", 640, 480, 60);

    ckResMgr::loadResource("../data/catcake_logo_71x14.png", true);
    ckUtil::loadPixelArtModelAs(ckID_("catcake_icon.mdl"), "../data/catcake_icon_model.ckl", "../data/catcake_icon_model.png", 4.0f);
    ckUtil::loadPixelArtModelAs(ckID_("zombie.mdl"), "../data/zombie_model.ckl", "../data/zombie_model.png", 8.0f);
    ckUtil::loadPixelArtModelAs(ckID_("blood.mdl"), "../data/blood_model.ckl", "../data/blood_model.png", 4.0f);

    initMotData();

    newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newStage();
    newZombie(0, -20.0f, 64.0f, -150.0f);
    newZombie(1, 100.0f, 64.0f, 50.0f);
    newZombie(2, -120.0f, 64.0f, 150.0f);

    ckStartCatcake();
    ckDestroyCatcake();
}
