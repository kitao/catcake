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


class ckLts;


/*!
    @ingroup ckDraw
    TODO
*/
class CK_API ckLit
{
    friend class ckLts;

public:
    ckDefineException(ExceptionInvalidArgument);

    ckLit* getPrevN() const;
    ckLit* getNextN() const;

    ckID getID() const;

    bool isEnabled() const;
    void setEnabled(bool is_enabled);

    const ckVec& getPos() const;
    void setPos(const ckVec& pos);

    r32 getInnerRadius() const;
    r32 getOuterRadius() const;
    void setRadius(r32 inner_rad, r32 outer_rad);

    ckCol getColor() const;
    void setColor(ckCol col);

private:
    ckLit(ckID lts_id);
    void operator=(const ckLit&);

    void updateBound();

    ckList<ckLit>::Item m_item;
    ckID m_id;
    ckType<u8, bool> m_is_enabled;
    ckVec m_pos;
    r32 m_inner_rad;
    r32 m_outer_rad;
    ckCol m_col;
    u16 m_col_int;
    ckVec m_min_bound;
    ckVec m_max_bound;
    r32 m_sq_inner_rad;
    r32 m_sq_outer_rad;
};
