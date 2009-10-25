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
    @ingroup pgDraw
    TODO
*/
class PG_API pgSprt : public pgDraw
{
public:
    struct PG_API SprtData
    {
        pgVec pos;
        pgCol col;
        r32 w, h;
        r32 u1, v1, u2, v2;
        s32 ang;

        SprtData();
    };

    pgSprt();
    virtual ~pgSprt();

    void init(u16 max_data_num, pgID scr_id);
    void init(u16 max_data_num, pgDraw* parent);

    void init(SprtData* sprt_data, u16 max_data_num, pgID scr_id);
    void init(SprtData* sprt_data, u16 max_data_num, pgDraw* parent);

    bool isShareData() const;

    u16 getCurDataNum() const;
    void setCurDataNum(u16 cur_data_num);
    u16 getMaxDataNum() const;
    void reallocData(u16 max_data_num);
    void copyData(u16 dest_index, const pgSprt* src_sprt, u16 src_index);

    pgVec& dataPos(u16 index);
    pgCol& dataCol(u16 index);
    r32& dataW(u16 index);
    r32& dataH(u16 index);
    r32& dataU1(u16 index);
    r32& dataV1(u16 index);
    r32& dataU2(u16 index);
    r32& dataV2(u16 index);
    s32& dataAng(u16 index);
    void setDataSize(u16 index, r32 width, r32 height);
    void setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2);

private:
    void init2(bool is_share_data, SprtData* sprt_data, u16 max_data_num, pgID scr_id, pgDraw* parent);

    virtual void render(const pgMat& view);
    void render_soft(const pgMat& view);
    void render_shader(const pgMat& view);

    pgType<u8, bool> m_is_share_data;
    SprtData* m_sprt_data;
    u16 m_cur_data_num;
    u16 m_max_data_num;
};
