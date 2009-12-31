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


#include "test.h"


static void createTestModelData(ckID mdl_data_id)
{
    ckMdlData mdl_data;

    mdl_data.initAsWriter(2, 1, ckID::ZERO, false);

    mdl_data.setNodeLocal(0, ckMat::UNIT.translate(1.0f, 2.0f, 3.0f));
    mdl_data.setNodeLocal(1, ckMat::UNIT.translate(10.f, 20.f, 30.f));

    mdl_data.registerAsResource(mdl_data_id);
}


static void createTestMotionData(ckID mot_data_id)
{
    ckMotData mot_data;

    mot_data.initAsWriter(3, 2, 5);

    mot_data.setMotionInfo(0, 8, 0, 3);
    mot_data.setMotionInfo(1, 2, 3, 2);

    mot_data.setKeyFramePlayFrame(0, 0);
    mot_data.setKeyFramePlayFrame(1, 4);
    mot_data.setKeyFramePlayFrame(2, 6);
    mot_data.setKeyFramePlayFrame(3, 0);
    mot_data.setKeyFramePlayFrame(4, 2);

    mot_data.setNodeLocal(0, 0, ckMat::UNIT.translate(10.0f, 20.0f, 30.0f));
    mot_data.setNodeLocal(1, 0, ckMat::UNIT.translate(11.0f, 21.0f, 31.0f));
    mot_data.setNodeLocal(2, 0, ckMat::UNIT.translate(12.0f, 22.0f, 32.0f));

    mot_data.setNodeLocal(0, 1, ckMat::UNIT.translate(50.0f, 60.0f, 70.0f).rotateX_s32(40));
    mot_data.setNodeLocal(1, 1, ckMat::UNIT.translate(51.0f, 61.0f, 71.0f).rotateY_s32(40));
    mot_data.setNodeLocal(2, 1, ckMat::UNIT.translate(52.0f, 62.0f, 72.0f).rotateZ_s32(40));

    mot_data.setNodeLocal(0, 2, ckMat::UNIT.translate(0.0f, 10.0f, 20.0f).rotateX_s32(60));
    mot_data.setNodeLocal(1, 2, ckMat::UNIT.translate(1.0f, 11.0f, 21.0f).rotateY_s32(60));
    mot_data.setNodeLocal(2, 2, ckMat::UNIT.translate(2.0f, 12.0f, 22.0f).rotateZ_s32(60));

    mot_data.setNodeLocal(0, 3, ckMat::UNIT.translate(-100.0f, -200.0f, -300.0f).rotateX_s32(-90));
    mot_data.setNodeLocal(1, 3, ckMat::UNIT.translate(-110.0f, -210.0f, -310.0f).rotateY_s32(-90));
    mot_data.setNodeLocal(2, 3, ckMat::UNIT.translate(-120.0f, -220.0f, -320.0f).rotateZ_s32(-90));

    mot_data.setNodeLocal(0, 4, ckMat::UNIT.translate(-200.0f, -300.0f, -400.0f).rotateX_s32(-10));
    mot_data.setNodeLocal(1, 4, ckMat::UNIT.translate(-210.0f, -310.0f, -410.0f).rotateY_s32(-10));
    mot_data.setNodeLocal(2, 4, ckMat::UNIT.translate(-220.0f, -320.0f, -420.0f).rotateZ_s32(-10));

    mot_data.registerAsResource(mot_data_id);
}


void ckMotTest()
{
    /*
        ckMot()
        ~ckMot()
        void init(ckMdl* mdl, ckID mot_data_id)
        u16 getMotionIndex() const
        PlayMode getPlayMode() const
        r32 getPlaySpeed() const
        u16 getInterpFrame() const
        bool isPlaying() const
        void play(ckMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame)
        void stop()
        r32 getNextUpdateFrame() const
        void update(ckMdl* mdl)
    */
    {
        ckDrawMgr::createAfterRes();

        createTestModelData(ckID_("TEST_MODEL_DATA"));
        createTestMotionData(ckID_("TEST_MOTION_DATA"));

        ckMot mot;

        ckAssertThrow(mot.getMotionIndex(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.getPlayMode(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.getPlaySpeed(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.getInterpFrame(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.isPlaying(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.play(NULL, 0, ckMot::PLAY_NORMAL, 1.0f, 0), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.stop(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.getNextUpdateFrame(), ckMot::ExceptionNotInitialized);
        ckAssertThrow(mot.update(NULL), ckMot::ExceptionNotInitialized);

        {
            ckMdl mdl;
            mdl.init(ckID_("TEST_MODEL_DATA"), ckDrawMgr::DEFAULT_3D_SCREEN_ID);

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(10.0f, 20.0f, 30.0f)));

            mot.init(&mdl, ckID_("TEST_MOTION_DATA"));

            ckAssert(mot.getMotionIndex() == 0);
            ckAssert(mot.getPlayMode() == ckMot::PLAY_NORMAL);
            ckAssert(isEqual(mot.getPlaySpeed(), 1.0f));
            ckAssert(mot.getInterpFrame() == 0);
            ckAssert(!mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(10.0f, 20.0f, 30.0f)));

            mot.play(&mdl, 0, ckMot::PLAY_NORMAL, 1.0f, 0);

            ckAssert(mot.getMotionIndex() == 0);
            ckAssert(mot.getPlayMode() == ckMot::PLAY_NORMAL);
            ckAssert(isEqual(mot.getPlaySpeed(), 1.0f));
            ckAssert(mot.getInterpFrame() == 0);
            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(10.0f, 20.0f, 30.0f)));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 1.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(11.0f, 22.0f, 33.0f)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(21.0f, 41.0f, 61.0f)));

            mot.update(&mdl);
            mot.update(&mdl);
            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 4.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(41.0f, 52.0f, 63.0f).rotateX_s32(30)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(51.0f, 71.0f, 91.0f).rotateY_s32(30)));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 5.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(51.0f, 62.0f, 73.0f).rotateX_s32(40)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(61.0f, 81.0f, 101.0f).rotateY_s32(40)));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 6.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(26.0f, 37.0f, 48.0f).rotateX_s32(50)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(36.0f, 56.0f, 76.0f).rotateY_s32(50)));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 7.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(1.0f, 12.0f, 23.0f).rotateX_s32(60)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(11.0f, 31.0f, 51.0f).rotateY_s32(60)));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 8.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(1.0f, 12.0f, 23.0f).rotateX_s32(60)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(11.0f, 31.0f, 51.0f).rotateY_s32(60)));

            mot.update(&mdl);

            ckAssert(!mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(1.0f, 12.0f, 23.0f).rotateX_s32(60)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(11.0f, 31.0f, 51.0f).rotateY_s32(60)));

            mot.update(&mdl);

            ckAssert(!mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            mot.play(&mdl, 1, ckMot::PLAY_LOOP, 0.5f, 2);

            ckAssert(mot.getMotionIndex() == 1);
            ckAssert(mot.getPlayMode() == ckMot::PLAY_LOOP);
            ckAssert(isEqual(mot.getPlaySpeed(), 0.5f));
            ckAssert(mot.getInterpFrame() == 2);
            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.5f));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 1.0f));

            mot.update(&mdl);
            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 2.0f));

            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.5f));

            mot.update(&mdl);
            mot.update(&mdl);

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 1.5f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(-149.0f, -248.0f, -347.0f).rotateX_s32(-50)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(-150.0f, -240.0f, -330.0f).rotateY_s32(-50)));

            mot.stop();
            ckAssert(!mot.isPlaying());

            mot.play(&mdl, 1, ckMot::PLAY_KEEP, 1.0f, 3);

            ckAssert(mot.getMotionIndex() == 1);
            ckAssert(mot.getPlayMode() == ckMot::PLAY_KEEP);
            ckAssert(isEqual(mot.getPlaySpeed(), 1.0f));
            ckAssert(mot.getInterpFrame() == 3);
            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            for (s32 i = 0; i < 16; i++)
            {
                mot.update(&mdl);
            }

            ckAssert(mot.isPlaying());
            ckAssert(isEqual(mot.getNextUpdateFrame(), 2.0f));

            ckAssert(isEqual(mdl.getNodeDraw(0)->local(), ckMat::UNIT.translate(-199.0f, -298.0f, -397.0f).rotateX_s32(-10)));
            ckAssert(isEqual(mdl.getNodeDraw(1)->local(), ckMat::UNIT.translate(-200.0f, -290.0f, -380.0f).rotateY_s32(-10)));
        }

        ckAssertThrow(mot.play(NULL, 0, ckMot::PLAY_NORMAL, 1.0f, 0), ckMot::ExceptionInvalidArgument);
        ckAssertThrow(mot.play(reinterpret_cast<ckMdl*>(123), 2, ckMot::PLAY_NORMAL, 0.1f, 0), ckMot::ExceptionInvalidArgument);
        ckAssertThrow(mot.play(reinterpret_cast<ckMdl*>(123), 2, ckMot::PLAY_NORMAL, -0.1f, 0), ckMot::ExceptionInvalidArgument);
        ckAssertThrow(mot.update(NULL), ckMot::ExceptionInvalidArgument);
        ckAssertThrow(mot.init(NULL, ckID::genID()), ckMot::ExceptionInvalidArgument);
        ckAssertThrow(mot.init(reinterpret_cast<ckMdl*>(123), ckID::ZERO), ckMot::ExceptionInvalidArgument);

        ckResMgr::removeResource(ckID_("TEST_MODEL_DATA"));
        ckResMgr::removeResource(ckID_("TEST_MOTION_DATA"));

        ckDrawMgr::destroyBeforeRes();
    }
}
