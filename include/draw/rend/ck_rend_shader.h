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
