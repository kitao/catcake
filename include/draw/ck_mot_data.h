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


/*!
    @ingroup ckDraw
    TODO
*/
class CK_API ckMotData
{
public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionInvalidData);
    ckDefineException(ExceptionNotInitialized);

    static const u32 MOTION_DATA_VERSION = 70; // 0.7.0

    enum MotionDataMode
    {
        MODE_READ, //
        MODE_WRITE
    };

    ckMotData();
    ~ckMotData();

    void initAsReader(const void* data, u32 data_size);
    void initAsWriter(u16 node_num, u16 mot_num, u32 key_frame_num);

    MotionDataMode getMode() const;
    u16 getNodeNum() const;
    u16 getMotionNum() const;
    u32 getKeyFrameNum() const;

    u32 getMotionFrameNum(u16 mot_index) const;
    u32 getMotionKeyFrameIndex(u16 mot_index) const;
    u32 getMotionKeyFrameNum(u16 mot_index) const;
    void setMotionInfo(u16 mot_index, u32 frame_num, u32 key_frame_index, u32 key_frame_num);

    u32 getKeyFramePlayFrame(u32 key_frame_index) const;
    void setKeyFramePlayFrame(u32 key_frame_index, u32 play_frame);

    const ckQuat& getNodeRotate(u16 node_index, u32 key_frame_index) const;
    const ckVec& getNodeTrans(u16 node_index, u32 key_frame_index) const;
    void setNodeLocal(u16 node_index, u32 key_frame_index, const ckMat& local);

    void registerAsResource(ckID res_id);

private:
    struct MotionDataHeader
    {
        ckID format_id;
        u16 format_version;
        u16 node_num;
        u16 mot_num;
        u32 key_frame_num;
        u32 key_frame_info_offset;
        u32 node_info_offset;
    };

    struct MotionInfo
    {
        u32 frame_num;
        u32 key_frame_index;
        u32 key_frame_num;
    };

    struct KeyFrameInfo
    {
        u16 play_frame;
    };

    struct NodeInfo
    {
        ckQuat m_quat;
        ckVec m_trans;
        ckVec m_scale;
    };

    ckMotData(const ckMotData&);
    void operator=(const ckMotData&);

    ckType<u8, MotionDataMode> m_mode;
    u8* m_data;
    u32 m_data_size;
    MotionDataHeader* m_mot_data_header;
    MotionInfo* m_mot_info;
    KeyFrameInfo* m_key_frame_info;
    NodeInfo* m_node_info;
};
