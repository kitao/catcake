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
class CK_API ckRend
{
    friend class ckPrim;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);

    virtual ~ckRend();

    bool isActive() const;
    void setActive(bool is_active);

protected:
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
