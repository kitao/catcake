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


/*!
    @ingroup ckDraw
    A motion data, which is used by motions.
*/
class CK_API ckMotData
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionInvalidData);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The version number of the motion data format.
    */
    static const u32 MOTION_DATA_VERSION = 70; // 0.7.0

    /*!
        The modes of the model data.
    */
    enum MotionDataMode
    {
        MODE_READ, //!< Read-only mode.
        MODE_WRITE //!< Read and write mode.
    };

    /*!
        Constructs a motion data.
    */
    ckMotData();

    /*!
        Destructs this motion data.
    */
    ~ckMotData();

    /*!
        Initializes this motion data as read-only mode.
        @param[in] data Data.
        @param[in] data_size The size of data.
    */
    void initAsReader(const void* data, u32 data_size);

    /*!
        Initializes this motion data as read-and-write mode.
        @param[in] node_num The number of the nodes.
        @param[in] mot_num The number of the motions.
        @param[in] key_frame_num The number of the key frames.
    */
    void initAsWriter(u16 node_num, u16 mot_num, u32 key_frame_num);

    /*!
        Returns the motion data mode.
        @return The motion data mode.
    */
    MotionDataMode getMode() const;

    /*!
        Returns the number of the nodes.
        @return The number of the nodes.
    */
    u16 getNodeNum() const;

    /*!
        Returns the number of the motions.
        @return The number of the motions.
    */
    u16 getMotionNum() const;

    /*!
        Returns the number of the key frames.
        @return The number of the key frames.
    */
    u32 getKeyFrameNum() const;

    /*!
        Returns the number of the specified motion.
        @param[in] mot_index The index of a motion.
        @return The number of the specified motion.
    */
    u32 getMotionFrameNum(u16 mot_index) const;

    /*!
        Returns the first index of the key frames of the specified motion.
        @param[in] mot_index The index of a motion.
        @return The first index of the key frames of the specified motion.
    */
    u32 getMotionKeyFrameIndex(u16 mot_index) const;

    /*!
        Returns the number of the key frames of the specified motion.
        @param[in] mot_index The index of a motion.
        @return The number of the key frames of the specified motion.
    */
    u32 getMotionKeyFrameNum(u16 mot_index) const;

    /*!
        Sets the motion information of the specified motion.
        @param[in] mot_index The index of a motion.
        @param[in] frame_num The number of the frames.
        @param[in] key_frame_index The first index of a motion.
        @param[in] key_frame_num The number of the key frames.
    */
    void setMotionInfo(u16 mot_index, u32 frame_num, u32 key_frame_index, u32 key_frame_num);

    /*!
        Returns the play frame of the specified key frame.
        @param[in] key_frame_index The index of a key frame.
        @return The play frame of the specified key frame.
    */
    u32 getKeyFramePlayFrame(u32 key_frame_index) const;

    /*!
        Sets the play frame of the specified key frame.
        @param[in] key_frame_index The index of a key frame.
        @param[in] play_frame A play frame.
    */
    void setKeyFramePlayFrame(u32 key_frame_index, u32 play_frame);

    /*!
        Returns the rotation of the specified key frame.
        @param[in] node_index The index of a node.
        @param[in] key_frame_index The index of a key frame.
        @return The rotation of the specified key frame.
    */
    const ckQuat& getNodeRotate(u16 node_index, u32 key_frame_index) const;

    /*!
        Returns the translation of the specified key frame.
        @param[in] node_index The index of a node.
        @param[in] key_frame_index The index of a key frame.
        @return The translation of the specified key frame.
    */
    const ckVec& getNodeTrans(u16 node_index, u32 key_frame_index) const;

    /*!
        Sets the local matrix of the specified key frame.
        @param[in] node_index The index of a node.
        @param[in] key_frame_index The index of a key frame.
        @param[in] local A local matrix.
    */
    void setNodeLocal(u16 node_index, u32 key_frame_index, const ckMat& local);

    /*!
        Registers this motion data as a resource.
        @param[in] res_id The ID of the resource.
    */
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
