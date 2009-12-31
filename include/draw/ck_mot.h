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
    A motion for 3D model, which consists of motion nodes.
*/
class CK_API ckMot
{
    friend class ckJoin;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The motion play modes.
    */
    enum PlayMode
    {
        PLAY_NORMAL, //!< Play once.
        PLAY_LOOP, //!< Play repeatedly.
        PLAY_KEEP //!< Play and keep the last frame.
    };

    /*!
        Constructs a motion.
    */
    ckMot();

    /*!
        Destructs this motion.
    */
    ~ckMot();

    /*!
        Initializes this motion.
        @param[in] mdl A target model.
        @param[in] mot_data_id The ID of motion data.
    */
    void init(ckMdl* mdl, ckID mot_data_id);

    /*!
        Returns the index of the current motion.
        @return The index of the current motion.
    */
    u16 getMotionIndex() const;

    /*!
        Returns the play mode.
        @return The play mode.
    */
    PlayMode getPlayMode() const;

    /*!
        Returns the play speed.
        @return The play speed.
    */
    r32 getPlaySpeed() const;

    /*!
        Returns the number of the frames for interporation.
        @return The number of the frames for interporation.
    */
    u16 getInterpFrame() const;

    /*!
        Returns whether playing motion.
        @return Whether playing motion.
    */
    bool isPlaying() const;

    /*!
        Plays the specified motion.
        @param[in] mdl A target model.
        @param[in] mot_index The index of a motion.
        @param[in] play_mode A play mode.
        @param[in] play_speed A play speed.
        @param[in] interp_frame The number of the frames for interporation.
    */
    void play(ckMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame);

    /*!
        Stops playing motion.
    */
    void stop();

    /*!
        Returns the frame number of next updating time.
    */
    r32 getNextUpdateFrame() const;

    /*!
        Updates this motion.
    */
    void update(ckMdl* mdl);

private:
    struct InterpInfo
    {
        ckQuat quat;
        ckVec trans;
    };

    ckMot(const ckMot&);
    void operator=(const ckMot&);

    void uninit();

    ckMotData m_mot_data;
    InterpInfo* m_interp_info;
    ckType<u8, PlayMode> m_play_mode;
    u16 m_mot_index;
    u16 m_interp_frame;
    r32 m_play_speed;
    r32 m_playing_frame;
    r32 m_interp_speed;
    r32 m_interp_ratio;
};
