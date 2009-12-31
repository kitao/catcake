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
    An user custom renderer for drawing element primitives.
*/
class CK_API ckRend
{
    friend class ckPrim;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        Destructs this renderer.
    */
    virtual ~ckRend();

    /*!
        Returns whether this renderer is active.
        @return Whether this renderer is active.
    */
    bool isActive() const;

    /*!
        Determines whether this renderer is active.
        @param[in] is_active Whether this renderer is active.
    */
    void setActive(bool is_active);

protected:
    //! @cond
    ckRend();
    void init(ckPrim* prim, u32 body_size, u32 data_size);
    void init(ckPrim* prim, u32 body_size, u32 data_size, void* rend_data);

    ckPrim* getPrim() const;

    template<class T> T* getRendBody()
    {
        if (!m_prim)
        {
            ckThrow(ExceptionNotInitialized);
        }

        return static_cast<T*>(m_rend_body);
    }

    template<class T> T* getRendData(u16 index)
    {
        if (!m_prim)
        {
            ckThrow(ExceptionNotInitialized);
        }

        if (index >= getMaxDataNum())
        {
            ckThrow(ExceptionInvalidArgument);
        }

        return static_cast<T*>(m_rend_data) + index;
    }

    static void renderAllocBuffer(ckVec** pos_buf, ckCol** col_buf, r32** uv_buf, ckVec** normal_buf, //
        const ckPrim* prim, bool has_pos_buf, bool has_col_buf, bool has_uv_buf, bool has_normal_buf);

    static const ckMat& renderGetPrimWorld(const ckPrim* prim);
    static ckTex* renderGetPrimTextureN(const ckPrim* prim);
    static ckCol renderGetPrimFinalColor(const ckPrim* prim);

    static bool renderIsTextureUVAdjustNeeded(ckTex* tex);
    static void renderGetTextureParam(r32* u_param_a, r32* u_param_b, r32* v_param_a, r32* v_param_b, const ckTex* tex);

    static void renderCalcColorBuffer(ckCol* col_buf, const ckPrim* prim);
    static void renderCalcUVBuffer(r32* uv_buf, const ckPrim* prim);

    static void renderSetTexture(ckTex* tex1, ckTex* tex2, ckTex* tex3, bool is_bilinear);
    static void renderSetVertexPointer(u32 stride, const r32* vert);
    static void renderSetColorPointer(u32 stride, const u8* color);
    static void renderSetTexCoordPointer(u32 stride, const r32* uv);

    static void renderSetTexture(const ckPrim* prim);
    static void renderSetVertexPointer(const ckPrim* prim);
    static void renderSetColorPointer(const ckPrim* prim);
    static void renderSetTexCoordPointer(const ckPrim* prim);

    static void renderDrawArrays(u8 prim_mode, u16 first, u16 count);

    static u32 renderGetShaderUniformLocation(const ckShd* shd, u8 index);
    static u32 renderGetShaderAttribLocation(const ckShd* shd, u8 index);
    static u32 renderGetShaderTextureLocation(const ckShd* shd, u8 index);
    static void renderSetShader(ckShd* shd);
    static void renderSetUniform_s32(u32 location, s32 uniform);
    static void renderSetUniform_r32(u32 location, r32 uniform);
    static void renderSetUniform_localToScreen(const ckShd* shd);
    static void renderSetAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib);
    static void renderSetAttribPointer_vertex(const ckShd* shd, u32 stride, const r32* vert);
    static void renderSetAttribPointer_color(const ckShd* shd, u32 stride, const u8* color);
    static void renderSetAttribPointer_texCoord(const ckShd* shd, u32 stride, const r32* uv);
    static void renderSetAttribPointer_vertex(const ckShd* shd, const ckPrim* prim);
    static void renderSetAttribPointer_color(const ckShd* shd, const ckPrim* prim);
    static void renderSetAttribPointer_texCoord(const ckShd* shd, const ckPrim* prim);
    static void renderDisableAttribPointers(const ckShd* shd);

    static void renderCallPrimRenderWithDestroyingBuffer(ckPrim* prim, const ckMat& view);
    //! @endcond

private:
    ckRend(const ckRend&);
    void operator=(const ckRend&);

    void init2(ckPrim* prim, u32 body_size, u32 data_size, bool is_share_data, void* rend_data);
    void uninit();

    u16 getMaxDataNum() const;

    void reallocData(u16 max_data_num);
    void copyData(u16 dest_index, const ckPrim* src_prim, u16 src_index);

    virtual ckID getClassID() = 0;
    virtual void initData(void* data, u16 data_num) = 0;
    virtual void render(const ckMat& view) = 0;

    ckType<u8, bool> m_is_active;
    u32 m_rend_body_size;
    u32 m_rend_data_size;
    ckPrim* m_prim;
    void* m_rend_body;
    void* m_rend_data;
};
