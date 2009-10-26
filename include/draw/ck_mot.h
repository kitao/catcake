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
class CK_API ckMot
{
    friend class ckJoin;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);

    enum PlayMode
    {
        PLAY_NORMAL, //
        PLAY_LOOP, //
        PLAY_KEEP
    };

    ckMot();
    ~ckMot();

    void init(ckMdl* mdl, ckID mot_data_id);

    u16 getMotionIndex() const;
    PlayMode getPlayMode() const;
    r32 getPlaySpeed() const;
    u16 getInterpFrame() const;

    bool isPlaying() const;
    void play(ckMdl* mdl, u16 mot_index, PlayMode play_mode, r32 play_speed, u16 interp_frame);
    void stop();

    r32 getNextUpdateFrame() const;
    void update(ckMdl* mdl);

private:
    /*!
        TODO
    */
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
