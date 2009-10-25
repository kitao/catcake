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
class PG_API pgRend
{
    friend class pgPrim;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionNotInitialized);

    virtual ~pgRend();

    bool isActive() const;
    void setActive(bool is_active);

protected:
    pgRend();
    void init(pgPrim* prim, u32 body_size, u32 data_size);
    void init(pgPrim* prim, u32 body_size, u32 data_size, void* rend_data);

    pgPrim* getPrim() const;

    template<class T> T* getRendBody()
    {
        if (!m_prim)
        {
            pgThrow(ExceptionNotInitialized);
        }

        return static_cast<T*>(m_rend_body);
    }

    template<class T> T* getRendData(u16 index)
    {
        if (!m_prim)
        {
            pgThrow(ExceptionNotInitialized);
        }

        if (index >= getMaxDataNum())
        {
            pgThrow(ExceptionInvalidArgument);
        }

        return static_cast<T*>(m_rend_data) + index;
    }

    static void renderAllocBuffer(pgVec** pos_buf, pgCol** col_buf, r32** uv_buf, pgVec** normal_buf, //
        const pgPrim* prim, bool has_pos_buf, bool has_col_buf, bool has_uv_buf, bool has_normal_buf);

    static const pgMat& renderGetPrimWorld(const pgPrim* prim);
    static pgTex* renderGetPrimTextureN(const pgPrim* prim);
    static pgCol renderGetPrimFinalColor(const pgPrim* prim);

    static bool renderIsTextureUVAdjustNeeded(pgTex* tex);
    static void renderGetTextureParam(r32* u_param_a, r32* u_param_b, r32* v_param_a, r32* v_param_b, const pgTex* tex);

    static void renderCalcColorBuffer(pgCol* col_buf, const pgPrim* prim);
    static void renderCalcUVBuffer(r32* uv_buf, const pgPrim* prim);

    static void renderSetTexture(pgTex* tex1, pgTex* tex2, pgTex* tex3, bool is_bilinear);
    static void renderSetVertexPointer(u32 stride, const r32* vert);
    static void renderSetColorPointer(u32 stride, const u8* color);
    static void renderSetTexCoordPointer(u32 stride, const r32* uv);

    static void renderSetTexture(const pgPrim* prim);
    static void renderSetVertexPointer(const pgPrim* prim);
    static void renderSetColorPointer(const pgPrim* prim);
    static void renderSetTexCoordPointer(const pgPrim* prim);

    static void renderDrawArrays(u8 prim_mode, u16 first, u16 count);

    static u32 renderGetShaderUniformLocation(const pgShd* shd, u8 index);
    static u32 renderGetShaderAttribLocation(const pgShd* shd, u8 index);
    static u32 renderGetShaderTextureLocation(const pgShd* shd, u8 index);
    static void renderSetShader(pgShd* shd);
    static void renderSetUniform_s32(u32 location, s32 uniform);
    static void renderSetUniform_r32(u32 location, r32 uniform);
    static void renderSetUniform_localToScreen(const pgShd* shd);
    static void renderSetAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib);
    static void renderSetAttribPointer_vertex(const pgShd* shd, u32 stride, const r32* vert);
    static void renderSetAttribPointer_color(const pgShd* shd, u32 stride, const u8* color);
    static void renderSetAttribPointer_texCoord(const pgShd* shd, u32 stride, const r32* uv);
    static void renderSetAttribPointer_vertex(const pgShd* shd, const pgPrim* prim);
    static void renderSetAttribPointer_color(const pgShd* shd, const pgPrim* prim);
    static void renderSetAttribPointer_texCoord(const pgShd* shd, const pgPrim* prim);
    static void renderDisableAttribPointers(const pgShd* shd);

    static void renderCallPrimRenderWithDestroyingBuffer(pgPrim* prim, const pgMat& view);

private:
    pgRend(const pgRend&);
    void operator=(const pgRend&);

    void init2(pgPrim* prim, u32 body_size, u32 data_size, bool is_share_data, void* rend_data);
    void uninit();

    u16 getMaxDataNum() const;

    void reallocData(u16 max_data_num);
    void copyData(u16 dest_index, const pgPrim* src_prim, u16 src_index);

    virtual pgID getClassID() = 0;
    virtual void initData(void* data, u16 data_num) = 0;
    virtual void render(const pgMat& view) = 0;

    pgType<u8, bool> m_is_active;
    u32 m_rend_body_size;
    u32 m_rend_data_size;
    pgPrim* m_prim;
    void* m_rend_body;
    void* m_rend_data;
};
