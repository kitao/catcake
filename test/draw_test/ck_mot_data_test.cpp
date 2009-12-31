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


void ckMotDataTest()
{
    /*
        static const u32 MOTION_DATA_VERSION
    */
    {
        ckAssert(ckMotData::MOTION_DATA_VERSION == 70);
    }

    /*
        ckMotData()
        ~ckMotData()
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
        const ckQuat& getNodeRotate(u16 node_index, u32 key_frame_index) const
        const ckVec& getNodeTrans(u16 node_index, u32 key_frame_index) const
        void setNodeLocal(u16 node_index, u32 key_frame_index, const ckMat& local)
        void registerAsResource(ckID res_id)
    */
    {
        ckMotData mot_data;

        ckAssertThrow(mot_data.getMode(), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getNodeNum(), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getMotionNum(), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getKeyFrameNum(), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getMotionFrameNum(0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getMotionKeyFrameIndex(0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getMotionKeyFrameNum(0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.setMotionInfo(0, 0, 0, 0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getKeyFramePlayFrame(0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.setKeyFramePlayFrame(0, 0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getNodeRotate(0, 0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.getNodeTrans(0, 0), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.setNodeLocal(0, 0, ckMat::UNIT), ckMotData::ExceptionNotInitialized);
        ckAssertThrow(mot_data.registerAsResource(ckID::ZERO), ckMotData::ExceptionNotInitialized);

        mot_data.initAsWriter(2, 3, 4);

        ckAssert(mot_data.getMode() == ckMotData::MODE_WRITE);
        ckAssert(mot_data.getNodeNum() == 2);
        ckAssert(mot_data.getMotionNum() == 3);
        ckAssert(mot_data.getKeyFrameNum() == 4);

        for (s32 i = 0; i < 3; i++)
        {
            ckAssert(mot_data.getMotionFrameNum(i) == 1);
            ckAssert(mot_data.getMotionKeyFrameIndex(i) == 0);
            ckAssert(mot_data.getMotionKeyFrameNum(i) == 1);
        }

        for (s32 i = 0; i < 4; i++)
        {
            ckAssert(mot_data.getKeyFramePlayFrame(i) == 0);
        }

        for (s32 i = 0; i < 2; i++)
        {
            for (s32 j = 0; j < 4; j++)
            {
                ckAssert(isEqual(mot_data.getNodeRotate(i, j).toMat(ckVec::ZERO), ckMat::UNIT));
                ckAssert(isEqual(mot_data.getNodeTrans(i, j), ckVec::ZERO));
            }
        }

        mot_data.setMotionInfo(2, 2, 1, 2);
        ckAssert(mot_data.getMotionFrameNum(2) == 2);
        ckAssert(mot_data.getMotionKeyFrameIndex(2) == 1);
        ckAssert(mot_data.getMotionKeyFrameNum(2) == 2);

        mot_data.setKeyFramePlayFrame(1, 10);
        mot_data.setKeyFramePlayFrame(2, 20);
        mot_data.setKeyFramePlayFrame(3, 30);
        ckAssert(mot_data.getKeyFramePlayFrame(1) == 10 && mot_data.getKeyFramePlayFrame(2) == 20 && mot_data.getKeyFramePlayFrame(3) == 30);

        mot_data.setNodeLocal(1, 1, ckMat::UNIT.rotateY_s32(90).translate(0.0f, 0.0f, 100.0f));
        ckAssert(isEqual(mot_data.getNodeRotate(1, 1).toMat(ckVec::ZERO), ckMat::UNIT.rotateY_s32(90)));
        ckAssert(isEqual(mot_data.getNodeTrans(1, 1), ckVec(100.0f, 0.0f, 0.0f)));

        ckAssertThrow(mot_data.initAsWriter(0, 1, 1), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.initAsWriter(1, 0, 1), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.initAsWriter(1, 1, 0), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getMotionFrameNum(3), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getMotionKeyFrameIndex(3), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getMotionKeyFrameNum(3), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setMotionInfo(3, 1, 0, 1), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setMotionInfo(0, 0, 0, 1), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setMotionInfo(0, 1, 4, 1), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setMotionInfo(0, 1, 0, 0), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getKeyFramePlayFrame(4), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setKeyFramePlayFrame(4, 0), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getNodeRotate(3, 0), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getNodeRotate(0, 4), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getNodeTrans(3, 0), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getNodeTrans(0, 4), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setNodeLocal(3, 0, ckMat::UNIT), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setNodeLocal(0, 4, ckMat::UNIT), ckMotData::ExceptionInvalidArgument);

        mot_data.registerAsResource(ckID_("test_mot_data"));

        ckAssert(ckResMgr::hasResource(ckID_("test_mot_data")));
        ckAssert(mot_data.getMode() == ckMotData::MODE_READ);

        mot_data.initAsWriter(1, 1, 1);

        ckAssertThrow(mot_data.getMotionFrameNum(2), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getMotionKeyFrameIndex(2), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.getMotionKeyFrameNum(2), ckMotData::ExceptionInvalidArgument);

        ckRes res = ckResMgr::getResource(ckID_("test_mot_data"));

        mot_data.initAsReader(res.getData<void>(), res.getDataSize());

        ckAssert(mot_data.getMode() == ckMotData::MODE_READ);
        ckAssert(mot_data.getNodeNum() == 2);
        ckAssert(mot_data.getMotionNum() == 3);
        ckAssert(mot_data.getKeyFrameNum() == 4);
        ckAssert(mot_data.getMotionFrameNum(2) == 2);
        ckAssert(mot_data.getMotionKeyFrameIndex(2) == 1);
        ckAssert(mot_data.getMotionKeyFrameNum(2) == 2);
        ckAssert(mot_data.getKeyFramePlayFrame(1) == 10 && mot_data.getKeyFramePlayFrame(2) == 20 && mot_data.getKeyFramePlayFrame(3) == 30);
        ckAssert(isEqual(mot_data.getNodeRotate(1, 1).toMat(ckVec::ZERO), ckMat::UNIT.rotateY_s32(90)));
        ckAssert(isEqual(mot_data.getNodeTrans(1, 1), ckVec(100.0f, 0.0f, 0.0f)));

        ckAssertThrow(mot_data.initAsReader(NULL, 100), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.initAsReader("dummy", 0), ckMotData::ExceptionInvalidArgument);
        ckAssertThrow(mot_data.setMotionInfo(0, 1, 0, 1), ckMotData::ExceptionInvalidCall);
        ckAssertThrow(mot_data.setKeyFramePlayFrame(0, 0), ckMotData::ExceptionInvalidCall);
        ckAssertThrow(mot_data.setNodeLocal(0, 0, ckMat::UNIT), ckMotData::ExceptionInvalidCall);

        ckResMgr::removeResource(ckID_("test_mot_data"));
    }
}
