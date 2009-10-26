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
class CK_API ckPrim : public ckDraw
{
    friend class ckRend;

public:
    struct CK_API PrimData
    {
        ckVec pos;
        ckCol col;
        r32 u, v;

        PrimData();
    };

    enum PrimMode
    {
        MODE_POINTS, //
        MODE_LINES, //
        MODE_LINE_LOOP, //
        MODE_LINE_STRIP, //
        MODE_TRIANGLES, //
        MODE_TRIANGLE_STRIP, //
        MODE_TRIANGLE_FAN
    };

    ckPrim();
    virtual ~ckPrim();

    void init(PrimMode prim_mode, u16 max_data_num, ckID scr_id);
    void init(PrimMode prim_mode, u16 max_data_num, ckDraw* parent);

    void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, ckID scr_id);
    void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, ckDraw* parent);

    PrimMode getPrimMode() const;
    void setPrimMode(PrimMode prim_mode);

    bool isShareData() const;

    u16 getCurDataNum() const;
    void setCurDataNum(u16 cur_data_num);
    u16 getMaxDataNum() const;
    void reallocData(u16 max_data_num);
    void copyData(u16 dest_index, const ckPrim* src_prim, u16 src_index);

    ckVec& dataPos(u16 index);
    ckCol& dataCol(u16 index);
    r32& dataU(u16 index);
    r32& dataV(u16 index);
    void setDataUV(u16 index, r32 u, r32 v);
    void setDataRect(u16 index, const ckVec& center, r32 width, r32 height, ckCol col, r32 u1, r32 v1, r32 u2, r32 v2);

private:
    void init2(PrimMode prim_mode, bool is_share_data, PrimData* prim_data, u16 max_data_num, ckID scr_id, ckDraw* parent);

    virtual void render(const ckMat& view);
    void render_soft(const ckMat& view);
    void render_shader(const ckMat& view);

    ckType<u8, PrimMode> m_prim_mode;
    ckType<u8, bool> m_is_share_data;
    PrimData* m_prim_data;
    u16 m_cur_data_num;
    u16 m_max_data_num;
    ckRend* m_rend;
};
