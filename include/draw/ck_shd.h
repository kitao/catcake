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
    A programmable shader for drawing element primitives.
*/
class CK_API ckShd
{
    friend class ckRend;
    friend class ckPrim;
    friend class ckSprt;
    friend class ckDrawMgr;

public:
    /*!
        Returns the previous shader. If the previous shader doesn't exist, returns NULL.
        @return The previous shader.
    */
    ckShd* getPrevN() const;
    
    /*!
        Returns the next shader. If the next shader doesn't exist, returns NULL.
        @return The next shader.
    */
    ckShd* getNextN() const;

    /*!
        Returns the shader ID.
        @return The shader ID.
    */
    ckID getID() const;

    /*!
        Returns the number of the uniforms.
        @return The number of the uniforms.
    */
    u8 getUniformNum() const;

    /*!
        Returns the number of the attributes.
        @return The number of the attributes.
    */
    u8 getAttribNum() const;

    /*!
        Returns the number of the textures.
        @return The number of the textures.
    */
    u8 getTextureNum() const;

    /*!
        Returns whether the shader code is valid.
        @return Whether the shader code is valid.
    */
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
