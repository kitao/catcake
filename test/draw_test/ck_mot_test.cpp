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


#include "test.h"


static void createTestModelData(pgID mdl_data_id)
{
    pgMdlData mdl_data;

    mdl_data.initAsWriter(2, 1, pgID::ZERO, false);

    mdl_data.setNodeLocal(0, pgMat::UNIT.translate(1.0f, 2.0f, 3.0f));
    mdl_data.setNodeLocal(1, pgMat::UNIT.translate(10.f, 20.f, 30.f));

    mdl_data.registerAsResource(mdl_data_id);
}


static void createTestMotionData(pgID mot_data_id)
{
    pgMotData mot_data;

    mot_data.initAsWriter(3, 2, 5);

    mot_data.setMotionInfo(0, 8, 0, 3);
    mot_data.setMotionInfo(1, 2, 3, 2);

    mot_data.setKeyFramePlayFrame(0, 0);
    mot_data.setKeyFramePlayFrame(1, 4);
    mot_data.setKeyFramePlayFrame(2, 6);
    mot_data.setKeyFramePlayFrame(3, 0);
    mot_data.setKeyFramePlayFrame(4, 2);

    mot_data.setNodeLocal(0, 0, pgMat::UNIT.translate(10.0f, 20.0f, 30.0f));
    mot_data.setNodeLocal(1, 0, pgMat::UNIT.translate(11.0f, 21.0f, 31.0f));
    mot_data.setNodeLocal(2, 0, pgMat::UNIT.translate(12.0f, 22.0f, 32.0f));

    mot_data.setNodeLocal(0, 1, pgMat::UNIT.translate(50.0f, 60.0f, 70.0f).rotateX_s32(40));
    mot_data.setNodeLocal(1, 1, pgMat::UNIT.translate(51.0f, 61.0f, 71.0f).rotateY_s32(40));
    mot_data.setNodeLocal(2, 1, pgMat::UNIT.translate(52.0f, 62.0f, 72.0f).rotateZ_s32(40));

    mot_data.setNodeLocal(0, 2, pgMat::UNIT.translate(0.0f, 10.0f, 20.0f).rotateX_s32(60));
    mot_data.setNodeLocal(1, 2, pgMat::UNIT.translate(1.0f, 11.0f, 21.0f).rotateY_s32(60));
    mot_data.setNodeLocal(2, 2, pgMat::UNIT.translate(2.0f, 12.0f, 22.0f).rotateZ_s32(60));

    mot_data.setNodeLocal(0, 3, pgMat::UNIT.translate(-100.0f, -200.0f, -300.0f).rotateX_s32(-90));
    mot_data.setNodeLocal(1, 3, pgMat::UNIT.translate(-110.0f, -210.0f, -310.0f).rotateY_s32(-90));
    mot_data.setNodeLocal(2, 3, pgMat::UNIT.translate(-120.0f, -220.0f, -320.0f).rotateZ_s32(-90));

    mot_data.setNodeLocal(0, 4, pgMat::UNIT.translate(-200.0f, -300.0f, -400.0f).rotateX_s32(-10));
    mot_data.setNodeLocal(1, 4, pgMat::UNIT.translate(-210.0f, -310.0f, -410.0f).rotateY_s32(-10));
    mot_data.setNodeLocal(2, 4, pgMat::UNIT.translate(-220.0f, -320.0f, -420.0f).rotateZ_s32(-10));

    mot_data.registerAsResource(mot_data_id);
}


void pgMotTest()
{
    /*
        pgMot()
        ~pgMot()
        void init(pgMdl* mdl, pgID mot_data_id)
        u16 getMotionIndex() const
        PlayMode getPlayMode() const
        r32 getPlaySpeed() const
        u16 getInterpFrame() const
        bool isPlaying() const
        void play(pgMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame)
        void stop()
        r32 getNextUpdateFrame() const
        void update(pgMdl* mdl)
    */
    {
        pgDrawMgr::createAfterRes();

        createTestModelData(pgID_("TEST_MODEL_DATA"));
        createTestMotionData(pgID_("TEST_MOTION_DATA"));

        pgMot mot;

        pgAssertThrow(mot.getMotionIndex(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.getPlayMode(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.getPlaySpeed(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.getInterpFrame(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.isPlaying(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.play(NULL, 0, pgMot::PLAY_NORMAL, 1.0f, 0), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.stop(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.getNextUpdateFrame(), pgMot::ExceptionNotInitialized);
        pgAssertThrow(mot.update(NULL), pgMot::ExceptionNotInitialized);

        {
            pgMdl mdl;
            mdl.init(pgID_("TEST_MODEL_DATA"), pgDrawMgr::DEFAULT_3D_SCREEN_ID);

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(10.0f, 20.0f, 30.0f)));

            mot.init(&mdl, pgID_("TEST_MOTION_DATA"));

            pgAssert(mot.getMotionIndex() == 0);
            pgAssert(mot.getPlayMode() == pgMot::PLAY_NORMAL);
            pgAssert(isEqual(mot.getPlaySpeed(), 1.0f));
            pgAssert(mot.getInterpFrame() == 0);
            pgAssert(!mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(10.0f, 20.0f, 30.0f)));

            mot.play(&mdl, 0, pgMot::PLAY_NORMAL, 1.0f, 0);

            pgAssert(mot.getMotionIndex() == 0);
            pgAssert(mot.getPlayMode() == pgMot::PLAY_NORMAL);
            pgAssert(isEqual(mot.getPlaySpeed(), 1.0f));
            pgAssert(mot.getInterpFrame() == 0);
            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(1.0f, 2.0f, 3.0f)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(10.0f, 20.0f, 30.0f)));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 1.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(11.0f, 22.0f, 33.0f)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(21.0f, 41.0f, 61.0f)));

            mot.update(&mdl);
            mot.update(&mdl);
            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 4.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(41.0f, 52.0f, 63.0f).rotateX_s32(30)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(51.0f, 71.0f, 91.0f).rotateY_s32(30)));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 5.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(51.0f, 62.0f, 73.0f).rotateX_s32(40)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(61.0f, 81.0f, 101.0f).rotateY_s32(40)));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 6.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(26.0f, 37.0f, 48.0f).rotateX_s32(50)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(36.0f, 56.0f, 76.0f).rotateY_s32(50)));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 7.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(1.0f, 12.0f, 23.0f).rotateX_s32(60)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(11.0f, 31.0f, 51.0f).rotateY_s32(60)));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 8.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(1.0f, 12.0f, 23.0f).rotateX_s32(60)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(11.0f, 31.0f, 51.0f).rotateY_s32(60)));

            mot.update(&mdl);

            pgAssert(!mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(1.0f, 12.0f, 23.0f).rotateX_s32(60)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(11.0f, 31.0f, 51.0f).rotateY_s32(60)));

            mot.update(&mdl);

            pgAssert(!mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            mot.play(&mdl, 1, pgMot::PLAY_LOOP, 0.5f, 2);

            pgAssert(mot.getMotionIndex() == 1);
            pgAssert(mot.getPlayMode() == pgMot::PLAY_LOOP);
            pgAssert(isEqual(mot.getPlaySpeed(), 0.5f));
            pgAssert(mot.getInterpFrame() == 2);
            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.5f));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 1.0f));

            mot.update(&mdl);
            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 2.0f));

            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.5f));

            mot.update(&mdl);
            mot.update(&mdl);

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 1.5f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(-149.0f, -248.0f, -347.0f).rotateX_s32(-50)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(-150.0f, -240.0f, -330.0f).rotateY_s32(-50)));

            mot.stop();
            pgAssert(!mot.isPlaying());

            mot.play(&mdl, 1, pgMot::PLAY_KEEP, 1.0f, 3);

            pgAssert(mot.getMotionIndex() == 1);
            pgAssert(mot.getPlayMode() == pgMot::PLAY_KEEP);
            pgAssert(isEqual(mot.getPlaySpeed(), 1.0f));
            pgAssert(mot.getInterpFrame() == 3);
            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 0.0f));

            for (s32 i = 0; i < 16; i++)
            {
                mot.update(&mdl);
            }

            pgAssert(mot.isPlaying());
            pgAssert(isEqual(mot.getNextUpdateFrame(), 2.0f));

            pgAssert(isEqual(mdl.getNodeDraw(0)->local(), pgMat::UNIT.translate(-199.0f, -298.0f, -397.0f).rotateX_s32(-10)));
            pgAssert(isEqual(mdl.getNodeDraw(1)->local(), pgMat::UNIT.translate(-200.0f, -290.0f, -380.0f).rotateY_s32(-10)));
        }

        pgAssertThrow(mot.play(NULL, 0, pgMot::PLAY_NORMAL, 1.0f, 0), pgMot::ExceptionInvalidArgument);
        pgAssertThrow(mot.play(reinterpret_cast<pgMdl*>(123), 2, pgMot::PLAY_NORMAL, 0.1f, 0), pgMot::ExceptionInvalidArgument);
        pgAssertThrow(mot.play(reinterpret_cast<pgMdl*>(123), 2, pgMot::PLAY_NORMAL, -0.1f, 0), pgMot::ExceptionInvalidArgument);
        pgAssertThrow(mot.update(NULL), pgMot::ExceptionInvalidArgument);
        pgAssertThrow(mot.init(NULL, pgID::genID()), pgMot::ExceptionInvalidArgument);
        pgAssertThrow(mot.init(reinterpret_cast<pgMdl*>(123), pgID::ZERO), pgMot::ExceptionInvalidArgument);

        pgResMgr::removeResource(pgID_("TEST_MODEL_DATA"));
        pgResMgr::removeResource(pgID_("TEST_MOTION_DATA"));

        pgDrawMgr::destroyBeforeRes();
    }
}
