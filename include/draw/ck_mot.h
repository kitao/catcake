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
    A motion for 3D model, which consists of motion nodes.
*/
class CK_API ckMot
{
    friend class ckJoin;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);

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
