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
    A custom renderer for using programmable shaders.
*/
class CK_API ckRend_Shader : public ckRend
{
public:
    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] shd_id A shader ID.
    */
    void init(ckPrim* prim, ckID shd_id);

    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] rend_data Shared renderer data.
        @param[in] shd_id A shader ID.
    */
    void init(ckPrim* prim, r32* rend_data, ckID shd_id);

    /*!
        Returns the shader ID.
        @return The shader ID.
    */
    ckID getShaderID();

    /*!
        Returns the second texture ID.
        @return The second texture ID.
    */
    ckID get2ndTextureID();

    /*!
        Sets the second texture ID.
        @param[in] tex_id A texture ID.
    */
    void set2ndTextureID(ckID tex_id);

    /*!
        Returns the third texture ID.
        @return The third texture ID.
    */
    ckID get3rdTextureID();

    /*!
        Sets the third texture ID.
        @param[in] tex_id A texture ID.
    */
    void set3rdTextureID(ckID tex_id);

    /*!
        Returns the reference to the specified uniform.
        @param[in] uni_index The index of an uniform.
        @return The reference to the specified uniform.
    */
    r32& uniform(u8 uni_index);

    /*!
        Sets the specified uniforms as a vector.
        @param[in] uni_index the first index of uniforms.
        @param[in] vec A vector.
    */
    void setUniformVec(u8 uni_index, const ckVec& vec);

    /*!
        Sets the specified uniforms as a 3-element color.
        @param[in] uni_index the first index of uniforms.
        @param[in] col A color.
    */
    void setUniformCol3(u8 uni_index, ckCol col);

    /*!
        Sets the specified uniforms as a 4-element color.
        @param[in] uni_index The first index of uniforms.
        @param[in] col A color.
    */
    void setUniformCol4(u8 uni_index, ckCol col);

    /*!
        Returns the reference to the specified attribute.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The index of an attribute.
        @return The reference to the specified attribute.
    */
    r32& attrib(u16 data_index, u8 att_index);

    /*!
        Sets the specified attributes as a vector.
		@param[in] data_index The index of renderer data.
        @param[in] att_index The first index of attributes.
        @param[in] vec A vector.
    */
    void setAttribVec(u16 data_index, u8 att_index, const ckVec& vec);

    /*!
        Sets the specified attributes as a 3-element color.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The first index of attributes.
        @param[in] col A color.
    */
    void setAttribCol3(u16 data_index, u8 att_index, ckCol col);

    /*!
        Sets the specified attributes as a 4-element color.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The first index of attributes.
        @param[in] col A color.
    */
    void setAttribCol4(u16 data_index, u8 att_index, ckCol col);

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
