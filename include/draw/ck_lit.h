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


class pgLts;


/*!
    @ingroup pgDraw
    TODO
*/
class PG_API pgLit
{
    friend class pgLts;

public:
    pgDefineException(ExceptionInvalidArgument);

    pgLit* getPrevN() const;
    pgLit* getNextN() const;

    pgID getID() const;

    bool isActive() const;
    void setActive(bool is_active);

    const pgVec& getPos() const;
    void setPos(const pgVec& pos);

    r32 getInnerRadius() const;
    r32 getOuterRadius() const;
    void setRadius(r32 inner_rad, r32 outer_rad);

    pgCol getColor() const;
    void setColor(pgCol col);

private:
    pgLit(pgID lts_id);
    void operator=(const pgLit&);

    void updateBound();

    pgList<pgLit>::Item m_item;
    pgID m_id;
    pgType<u8, bool> m_is_active;
    pgVec m_pos;
    r32 m_inner_rad;
    r32 m_outer_rad;
    pgCol m_col;
    u16 m_col_int;
    pgVec m_min_bound;
    pgVec m_max_bound;
    r32 m_sq_inner_rad;
    r32 m_sq_outer_rad;
};
