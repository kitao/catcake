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
class CK_API ckShd
{
    friend class ckRend;
    friend class ckPrim;
    friend class ckSprt;
    friend class ckDrawMgr;

public:
    ckShd* getPrevN() const;
    ckShd* getNextN() const;

    ckID getID() const;
    u8 getUniformNum() const;
    u8 getAttribNum() const;
    u8 getTextureNum() const;
    bool isValid() const;

private:
    enum ShdFlag
    {
        FLAG_UPLOAD, //
        FLAG_COMPILE_ERROR
    };

    ckShd(ckID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);
    ~ckShd();
    void operator=(const ckShd&);

    u32 getShdObj();

    ckID m_id;
    char* m_vert_code;
    char* m_frag_code;
    u8 m_uni_num;
    u8 m_att_num;
    u8 m_tex_num;
    u32 m_shd_obj;
    u32 m_local_to_screen_loc;
    u32 m_vertex_loc;
    u32 m_color_loc;
    u32 m_texcoord_loc;
    u32* m_uni_loc_tbl;
    u32* m_att_loc_tbl;
    u32 m_tex_loc_tbl[3];
    ckFlag<u8, ShdFlag> m_flag;
};
