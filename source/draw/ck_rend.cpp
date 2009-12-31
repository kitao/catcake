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


#include "ck_draw_all.h"

#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckRend::~ckRend()
{
    uninit();
}


bool ckRend::isActive() const
{
    if (!m_prim)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_is_active.getType();
}


void ckRend::setActive(bool is_active)
{
    if (!m_prim)
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_is_active = is_active;
}


ckRend::ckRend()
{
    m_prim = NULL;
    m_rend_body = NULL;
    m_rend_data = NULL;
}


void ckRend::init(ckPrim* prim, u32 body_size, u32 data_size)
{
    init2(prim, body_size, data_size, false, NULL);
}


void ckRend::init(ckPrim* prim, u32 body_size, u32 data_size, void* rend_data)
{
    init2(prim, body_size, data_size, true, rend_data);
}


ckPrim* ckRend::getPrim() const
{
    if (!m_prim)
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_prim;
}


void ckRend::renderAllocBuffer(ckVec** pos_buf, ckCol** col_buf, r32** uv_buf, ckVec** normal_buf, //
    const ckPrim* prim, bool has_pos_buf, bool has_col_buf, bool has_uv_buf, bool has_normal_buf)
{
    u32 data_size = 0;

    if (has_pos_buf)
    {
        data_size += sizeof(ckVec);
    }

    if (has_col_buf)
    {
        data_size += sizeof(ckCol);
    }

    if (has_uv_buf)
    {
        data_size += sizeof(r32) * 2;
    }

    if (has_normal_buf)
    {
        data_size += sizeof(ckVec);
    }

    if (data_size == 0)
    {
        *pos_buf = NULL;
        *col_buf = NULL;
        *uv_buf = NULL;
        *normal_buf = NULL;

        return;
    }

    u8* buf = static_cast<u8*>(ckMemMgr::allocTempBufferForSystem(data_size * prim->m_cur_data_num));

    if (has_pos_buf)
    {
        *pos_buf = reinterpret_cast<ckVec*>(buf);
        buf += sizeof(ckVec) * prim->m_cur_data_num;
    }
    else
    {
        *pos_buf = NULL;
    }

    if (has_col_buf)
    {
        *col_buf = reinterpret_cast<ckCol*>(buf);
        buf += sizeof(ckCol) * prim->m_cur_data_num;
    }
    else
    {
        *col_buf = NULL;
    }

    if (has_uv_buf)
    {
        *uv_buf = reinterpret_cast<r32*>(buf);
        buf += sizeof(r32) * 2 * prim->m_cur_data_num;
    }
    else
    {
        *uv_buf = NULL;
    }

    if (has_normal_buf)
    {
        *normal_buf = reinterpret_cast<ckVec*>(buf);
        buf += sizeof(ckVec) * prim->m_cur_data_num;
    }
    else
    {
        *normal_buf = NULL;
    }
}


const ckMat& ckRend::renderGetPrimWorld(const ckPrim* prim)
{
    return prim->m_world;
}


ckTex* ckRend::renderGetPrimTextureN(const ckPrim* prim)
{
    return prim->m_tex ? (prim->m_tex->m_proxy_tex ? prim->m_tex->m_proxy_tex : prim->m_tex) : NULL;
}


ckCol ckRend::renderGetPrimFinalColor(const ckPrim* prim)
{
    return prim->m_final_col;
}


bool ckRend::renderIsTextureUVAdjustNeeded(ckTex* tex)
{
    return tex->m_flag.isOn(ckTex::FLAG_UV_ADJUST);
}


void ckRend::renderGetTextureParam(r32* u_param_a, r32* u_param_b, r32* v_param_a, r32* v_param_b, const ckTex* tex)
{
    *u_param_a = tex->m_u_param_a;
    *u_param_b = tex->m_u_param_b;
    *v_param_a = tex->m_v_param_a;
    *v_param_b = tex->m_v_param_b;
}


void ckRend::renderCalcColorBuffer(ckCol* col_buf, const ckPrim* prim)
{
    ckPrim::PrimData* prim_data = prim->m_prim_data;

    for (s32 i = 0; i < prim->m_cur_data_num; i++)
    {
        col_buf[i] = prim->m_final_col * prim_data[i].col;
    }
}


void ckRend::renderCalcUVBuffer(r32* uv_buf, const ckPrim* prim)
{
    ckTex* tex = prim->m_tex->m_proxy_tex ? prim->m_tex->m_proxy_tex : prim->m_tex;
    r32 u_param_a = tex->m_u_param_a;
    r32 u_param_b = tex->m_u_param_b;
    r32 v_param_a = tex->m_v_param_a;
    r32 v_param_b = tex->m_v_param_b;

    for (u32 i = 0; i < prim->m_cur_data_num; i++)
    {
        ckPrim::PrimData* prim_data = &prim->m_prim_data[i];

        *uv_buf = prim_data->u * u_param_a + u_param_b;
        uv_buf++;

        *uv_buf = prim_data->v * v_param_a + v_param_b;
        uv_buf++;
    }
}


void ckRend::renderSetTexture(ckTex* tex1, ckTex* tex2, ckTex* tex3, bool is_bilinear)
{
    ckLowLevelAPI::setTexture(tex1 ? tex1->getTexObj() : 0, tex2 ? tex2->getTexObj() : 0, tex3 ? tex3->getTexObj() : 0, is_bilinear);
}


void ckRend::renderSetVertexPointer(u32 stride, const r32* vert)
{
    ckLowLevelAPI::setVertexPointer(stride, vert);
}


void ckRend::renderSetColorPointer(u32 stride, const u8* color)
{
    ckLowLevelAPI::setColorPointer(stride, color);
}


void ckRend::renderSetTexCoordPointer(u32 stride, const r32* uv)
{
    ckLowLevelAPI::setTexCoordPointer(stride, uv);
}


void ckRend::renderSetTexture(const ckPrim* prim)
{
    ckTex* tex = prim->m_tex ? (prim->m_tex->m_proxy_tex ? prim->m_tex->m_proxy_tex : prim->m_tex) : NULL;

    if (tex)
    {
        ckLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, prim->m_draw_flag.isOn(ckDraw::FLAG_BILINEAR));
    }
    else
    {
        ckLowLevelAPI::setTexture(0, 0, 0, false);
    }
}


void ckRend::renderSetVertexPointer(const ckPrim* prim)
{
    ckLowLevelAPI::setVertexPointer(sizeof(ckPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->pos));
}


void ckRend::renderSetColorPointer(const ckPrim* prim)
{
    ckLowLevelAPI::setColorPointer(sizeof(ckPrim::PrimData), reinterpret_cast<const u8*>(&prim->m_prim_data->col));
}


void ckRend::renderSetTexCoordPointer(const ckPrim* prim)
{
    ckLowLevelAPI::setTexCoordPointer(sizeof(ckPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->u));
}


void ckRend::renderDrawArrays(u8 prim_mode, u16 first, u16 count)
{
    ckLowLevelAPI::drawArrays(static_cast<ckLowLevelAPI::DrawMode>(prim_mode), first, count);
}


u32 ckRend::renderGetShaderUniformLocation(const ckShd* shd, u8 index)
{
    return shd->m_uni_loc_tbl[index];
}


u32 ckRend::renderGetShaderAttribLocation(const ckShd* shd, u8 index)
{
    return shd->m_att_loc_tbl[index];
}


u32 ckRend::renderGetShaderTextureLocation(const ckShd* shd, u8 index)
{
    return shd->m_tex_loc_tbl[index];
}


void ckRend::renderSetShader(ckShd* shd)
{
    if (shd)
    {
        ckLowLevelAPI::setShader(shd->getShdObj());
    }
    else
    {
        ckLowLevelAPI::setShader(0);
    }
}


void ckRend::renderSetUniform_s32(u32 location, s32 uniform)
{
    ckLowLevelAPI::setUniform_s32(location, uniform);
}


void ckRend::renderSetUniform_r32(u32 location, r32 uniform)
{
    ckLowLevelAPI::setUniform_r32(location, uniform);
}


void ckRend::renderSetUniform_localToScreen(const ckShd* shd)
{
    ckLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);
}


void ckRend::renderSetAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib)
{
    ckLowLevelAPI::setAttribPointer_r32(location, size, stride, attrib);
}


void ckRend::renderSetAttribPointer_vertex(const ckShd* shd, u32 stride, const r32* vert)
{
    ckLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, stride, vert);
}


void ckRend::renderSetAttribPointer_color(const ckShd* shd, u32 stride, const u8* color)
{
    ckLowLevelAPI::setAttribPointer_color(shd->m_color_loc, stride, color);
}


void ckRend::renderSetAttribPointer_texCoord(const ckShd* shd, u32 stride, const r32* uv)
{
    ckLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, stride, uv);
}


void ckRend::renderSetAttribPointer_vertex(const ckShd* shd, const ckPrim* prim)
{
    ckLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(ckPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->pos));
}


void ckRend::renderSetAttribPointer_color(const ckShd* shd, const ckPrim* prim)
{
    ckLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(ckPrim::PrimData), reinterpret_cast<const u8*>(&prim->m_prim_data->col));
}


void ckRend::renderSetAttribPointer_texCoord(const ckShd* shd, const ckPrim* prim)
{
    ckLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(ckPrim::PrimData), &prim->m_prim_data->u);
}


void ckRend::renderDisableAttribPointers(const ckShd* shd)
{
    ckLowLevelAPI::disableAttribPointer(shd->m_vertex_loc);
    ckLowLevelAPI::disableAttribPointer(shd->m_color_loc);
    ckLowLevelAPI::disableAttribPointer(shd->m_texcoord_loc);

    for (s32 i = 0; i < shd->m_att_num; i++)
    {
        ckLowLevelAPI::disableAttribPointer(shd->m_att_loc_tbl[i]);
    }
}


void ckRend::renderCallPrimRenderWithDestroyingBuffer(ckPrim* prim, const ckMat& view)
{
    if (ckDrawMgr::isShaderAvailable())
    {
        prim->render_shader(view);
    }
    else
    {
        prim->render_soft(view);
    }
}


CK_DEFINE_COPY_CONSTRUCTOR(ckRend)


CK_DEFINE_OPERATOR_EQUAL(ckRend)


void ckRend::init2(ckPrim* prim, u32 body_size, u32 data_size, bool is_share_data, void* rend_data)
{
    if (!prim)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    prim->getPrimMode(); // assure the ckPrim is initialized

    if (prim->m_rend)
    {
        prim->m_rend->uninit();
    }

    uninit();

    if (prim->m_is_share_data.getType() != is_share_data)
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (data_size > 0 && is_share_data && !rend_data)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_is_active = true;
    m_rend_body_size = body_size;
    m_rend_data_size = data_size;
    m_prim = prim;
    m_rend_body = (m_rend_body_size > 0) ? ckMalloc(m_rend_body_size) : NULL;

    if (is_share_data)
    {
        m_rend_data = rend_data;
    }
    else if (m_rend_data_size > 0)
    {
        m_rend_data = ckMalloc(m_rend_data_size * m_prim->m_max_data_num);

        initData(m_rend_data, m_prim->m_max_data_num);
    }
    else
    {
        m_rend_data = NULL;
    }

    m_prim->m_rend = this;
}


void ckRend::uninit()
{
    if (m_prim)
    {
        m_prim->m_rend = NULL;
    }

    if (m_rend_body)
    {
        ckFree(m_rend_body);
    }

    if (m_rend_data && !m_prim->m_is_share_data.getType())
    {
        ckFree(m_rend_data);
    }

    m_prim = NULL;
    m_rend_body = NULL;
    m_rend_data = NULL;
}


u16 ckRend::getMaxDataNum() const
{
    return m_prim->getMaxDataNum();
}


void ckRend::reallocData(u16 max_data_num)
{
    if (m_rend_data_size > 0)
    {
        void* new_rend_data = ckMalloc(m_rend_data_size * max_data_num);
        u32 copy_num = ckMath::min(max_data_num, m_prim->m_max_data_num);

        initData(new_rend_data, max_data_num);

        ckMemMgr::memcpy(new_rend_data, m_rend_data, m_rend_data_size * copy_num);

        ckFree(m_rend_data);

        m_rend_data = new_rend_data;
    }
}


void ckRend::copyData(u16 dest_index, const ckPrim* src_prim, u16 src_index)
{
    ckRend* src_rend = src_prim->m_rend;

    if (!src_rend || src_rend->getClassID() != getClassID())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (m_rend_data_size > 0)
    {
        void* src = static_cast<u8*>(src_rend->m_rend_data) + m_rend_data_size * src_index;
        void* dest = static_cast<u8*>(m_rend_data) + m_rend_data_size * dest_index;

        ckMemMgr::memcpy(dest, src, m_rend_data_size);
    }
}
