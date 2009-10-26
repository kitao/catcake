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
class CK_API ckRend_Shader : public ckRend
{
public:
    void init(ckPrim* prim, ckID shd_id);
    void init(ckPrim* prim, r32* rend_data, ckID shd_id);

    ckID getShaderID();

    ckID get2ndTextureID();
    void set2ndTextureID(ckID tex_id);
    ckID get3rdTextureID();
    void set3rdTextureID(ckID tex_id);

    r32& uniform(u8 uni_index);
    void setUniformVec(u8 uni_index, const ckVec& vec);
    void setUniformCol3x255(u8 uni_index, ckCol col);
    void setUniformCol4x255(u8 uni_index, ckCol col);

    r32& attrib(u16 data_index, u8 att_index);
    void setAttribVec(u16 data_index, u8 att_index, const ckVec& vec);
    void setAttribCol3x255(u16 data_index, u8 att_index, ckCol col);
    void setAttribCol4x255(u16 data_index, u8 att_index, ckCol col);

private:
    struct RendBody
    {
        ckShd* shd;
        ckTex* tex2;
        ckTex* tex3;
        r32* uni;
        r32* att;
        u32 rend_data_size;
    };

    virtual ckID getClassID();
    virtual void initData(void* data, u16 data_num);
    virtual void render(const ckMat& view);
};
