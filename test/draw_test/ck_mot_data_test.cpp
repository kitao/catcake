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


void pgMotDataTest()
{
    /*
        static const u32 MOTION_DATA_VERSION
    */
    {
        pgAssert(pgMotData::MOTION_DATA_VERSION == 70);
    }

    /*
        pgMotData()
        ~pgMotData()
        void initAsReader(const void* data, u32 data_size)
        void initAsWriter(u16 node_num, u16 mot_num, u32 key_frame_num)
        MotionDataMode getMode() const
        u16 getNodeNum() const
        u16 getMotionNum() const
        u32 getKeyFrameNum() const
        u32 getMotionFrameNum(u16 mot_index) const
        u32 getMotionKeyFrameIndex(u16 mot_index) const
        u32 getMotionKeyFrameNum(u16 mot_index) const
        void setMotionInfo(u16 mot_index, u32 frame_num, u32 key_frame_index, u32 key_frame_num)
        u32 getKeyFramePlayFrame(u32 key_frame_index) const
        void setKeyFramePlayFrame(u32 key_frame_index, u32 play_frame)
        const pgQuat& getNodeRotate(u16 node_index, u32 key_frame_index) const
        const pgVec& getNodeTrans(u16 node_index, u32 key_frame_index) const
        void setNodeLocal(u16 node_index, u32 key_frame_index, const pgMat& local)
        void registerAsResource(pgID res_id)
    */
    {
        pgMotData mot_data;

        pgAssertThrow(mot_data.getMode(), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getNodeNum(), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getMotionNum(), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getKeyFrameNum(), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getMotionFrameNum(0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getMotionKeyFrameIndex(0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getMotionKeyFrameNum(0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.setMotionInfo(0, 0, 0, 0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getKeyFramePlayFrame(0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.setKeyFramePlayFrame(0, 0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getNodeRotate(0, 0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.getNodeTrans(0, 0), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.setNodeLocal(0, 0, pgMat::UNIT), pgMotData::ExceptionNotInitialized);
        pgAssertThrow(mot_data.registerAsResource(pgID::ZERO), pgMotData::ExceptionNotInitialized);

        mot_data.initAsWriter(2, 3, 4);

        pgAssert(mot_data.getMode() == pgMotData::MODE_WRITE);
        pgAssert(mot_data.getNodeNum() == 2);
        pgAssert(mot_data.getMotionNum() == 3);
        pgAssert(mot_data.getKeyFrameNum() == 4);

        for (s32 i = 0; i < 3; i++)
        {
            pgAssert(mot_data.getMotionFrameNum(i) == 1);
            pgAssert(mot_data.getMotionKeyFrameIndex(i) == 0);
            pgAssert(mot_data.getMotionKeyFrameNum(i) == 1);
        }

        for (s32 i = 0; i < 4; i++)
        {
            pgAssert(mot_data.getKeyFramePlayFrame(i) == 0);
        }

        for (s32 i = 0; i < 2; i++)
        {
            for (s32 j = 0; j < 4; j++)
            {
                pgAssert(isEqual(mot_data.getNodeRotate(i, j).toMat(pgVec::ZERO), pgMat::UNIT));
                pgAssert(isEqual(mot_data.getNodeTrans(i, j), pgVec::ZERO));
            }
        }

        mot_data.setMotionInfo(2, 2, 1, 2);
        pgAssert(mot_data.getMotionFrameNum(2) == 2);
        pgAssert(mot_data.getMotionKeyFrameIndex(2) == 1);
        pgAssert(mot_data.getMotionKeyFrameNum(2) == 2);

        mot_data.setKeyFramePlayFrame(1, 10);
        mot_data.setKeyFramePlayFrame(2, 20);
        mot_data.setKeyFramePlayFrame(3, 30);
        pgAssert(mot_data.getKeyFramePlayFrame(1) == 10 && mot_data.getKeyFramePlayFrame(2) == 20 && mot_data.getKeyFramePlayFrame(3) == 30);

        mot_data.setNodeLocal(1, 1, pgMat::UNIT.rotateY_s32(90).translate(0.0f, 0.0f, 100.0f));
        pgAssert(isEqual(mot_data.getNodeRotate(1, 1).toMat(pgVec::ZERO), pgMat::UNIT.rotateY_s32(90)));
        pgAssert(isEqual(mot_data.getNodeTrans(1, 1), pgVec(100.0f, 0.0f, 0.0f)));

        pgAssertThrow(mot_data.initAsWriter(0, 1, 1), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.initAsWriter(1, 0, 1), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.initAsWriter(1, 1, 0), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getMotionFrameNum(3), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getMotionKeyFrameIndex(3), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getMotionKeyFrameNum(3), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setMotionInfo(3, 1, 0, 1), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setMotionInfo(0, 0, 0, 1), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setMotionInfo(0, 1, 4, 1), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setMotionInfo(0, 1, 0, 0), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getKeyFramePlayFrame(4), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setKeyFramePlayFrame(4, 0), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getNodeRotate(3, 0), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getNodeRotate(0, 4), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getNodeTrans(3, 0), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getNodeTrans(0, 4), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setNodeLocal(3, 0, pgMat::UNIT), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setNodeLocal(0, 4, pgMat::UNIT), pgMotData::ExceptionInvalidArgument);

        mot_data.registerAsResource(pgID_("test_mot_data"));

        pgAssert(pgResMgr::hasResource(pgID_("test_mot_data")));
        pgAssert(mot_data.getMode() == pgMotData::MODE_READ);

        mot_data.initAsWriter(1, 1, 1);

        pgAssertThrow(mot_data.getMotionFrameNum(2), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getMotionKeyFrameIndex(2), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.getMotionKeyFrameNum(2), pgMotData::ExceptionInvalidArgument);

        pgRes res = pgResMgr::getResource(pgID_("test_mot_data"));

        mot_data.initAsReader(res.getData<void>(), res.getDataSize());

        pgAssert(mot_data.getMode() == pgMotData::MODE_READ);
        pgAssert(mot_data.getNodeNum() == 2);
        pgAssert(mot_data.getMotionNum() == 3);
        pgAssert(mot_data.getKeyFrameNum() == 4);
        pgAssert(mot_data.getMotionFrameNum(2) == 2);
        pgAssert(mot_data.getMotionKeyFrameIndex(2) == 1);
        pgAssert(mot_data.getMotionKeyFrameNum(2) == 2);
        pgAssert(mot_data.getKeyFramePlayFrame(1) == 10 && mot_data.getKeyFramePlayFrame(2) == 20 && mot_data.getKeyFramePlayFrame(3) == 30);
        pgAssert(isEqual(mot_data.getNodeRotate(1, 1).toMat(pgVec::ZERO), pgMat::UNIT.rotateY_s32(90)));
        pgAssert(isEqual(mot_data.getNodeTrans(1, 1), pgVec(100.0f, 0.0f, 0.0f)));

        pgAssertThrow(mot_data.initAsReader(NULL, 100), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.initAsReader("dummy", 0), pgMotData::ExceptionInvalidArgument);
        pgAssertThrow(mot_data.setMotionInfo(0, 1, 0, 1), pgMotData::ExceptionInvalidCall);
        pgAssertThrow(mot_data.setKeyFramePlayFrame(0, 0), pgMotData::ExceptionInvalidCall);
        pgAssertThrow(mot_data.setNodeLocal(0, 0, pgMat::UNIT), pgMotData::ExceptionInvalidCall);

        pgResMgr::removeResource(pgID_("test_mot_data"));
    }
}
