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


#include "pg_draw_all.h"

#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgRend::~pgRend()
{
    uninit();
}


bool pgRend::isActive() const
{
    if (!m_prim)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_is_active.getType();
}


void pgRend::setActive(bool is_active)
{
    if (!m_prim)
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_is_active = is_active;
}


pgRend::pgRend()
{
    m_prim = NULL;
    m_rend_body = NULL;
    m_rend_data = NULL;
}


void pgRend::init(pgPrim* prim, u32 body_size, u32 data_size)
{
    init2(prim, body_size, data_size, false, NULL);
}


void pgRend::init(pgPrim* prim, u32 body_size, u32 data_size, void* rend_data)
{
    init2(prim, body_size, data_size, true, rend_data);
}


pgPrim* pgRend::getPrim() const
{
    if (!m_prim)
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_prim;
}


void pgRend::renderAllocBuffer(pgVec** pos_buf, pgCol** col_buf, r32** uv_buf, pgVec** normal_buf, //
    const pgPrim* prim, bool has_pos_buf, bool has_col_buf, bool has_uv_buf, bool has_normal_buf)
{
    u32 data_size = 0;

    if (has_pos_buf)
    {
        data_size += sizeof(pgVec);
    }

    if (has_col_buf)
    {
        data_size += sizeof(pgCol);
    }

    if (has_uv_buf)
    {
        data_size += sizeof(r32) * 2;
    }

    if (has_normal_buf)
    {
        data_size += sizeof(pgVec);
    }

    if (data_size == 0)
    {
        *pos_buf = NULL;
        *col_buf = NULL;
        *uv_buf = NULL;
        *normal_buf = NULL;

        return;
    }

    u8* buf = static_cast<u8*>(pgMemMgr::allocTempBufferForSystem(data_size * prim->m_cur_data_num));

    if (has_pos_buf)
    {
        *pos_buf = reinterpret_cast<pgVec*>(buf);
        buf += sizeof(pgVec) * prim->m_cur_data_num;
    }
    else
    {
        *pos_buf = NULL;
    }

    if (has_col_buf)
    {
        *col_buf = reinterpret_cast<pgCol*>(buf);
        buf += sizeof(pgCol) * prim->m_cur_data_num;
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
        *normal_buf = reinterpret_cast<pgVec*>(buf);
        buf += sizeof(pgVec) * prim->m_cur_data_num;
    }
    else
    {
        *normal_buf = NULL;
    }
}


const pgMat& pgRend::renderGetPrimWorld(const pgPrim* prim)
{
    return prim->m_world;
}


pgTex* pgRend::renderGetPrimTextureN(const pgPrim* prim)
{
    return prim->m_tex ? (prim->m_tex->m_proxy_tex ? prim->m_tex->m_proxy_tex : prim->m_tex) : NULL;
}


pgCol pgRend::renderGetPrimFinalColor(const pgPrim* prim)
{
    return prim->m_final_col;
}


bool pgRend::renderIsTextureUVAdjustNeeded(pgTex* tex)
{
    return tex->m_flag.isOn(pgTex::FLAG_UV_ADJUST);
}


void pgRend::renderGetTextureParam(r32* u_param_a, r32* u_param_b, r32* v_param_a, r32* v_param_b, const pgTex* tex)
{
    *u_param_a = tex->m_u_param_a;
    *u_param_b = tex->m_u_param_b;
    *v_param_a = tex->m_v_param_a;
    *v_param_b = tex->m_v_param_b;
}


void pgRend::renderCalcColorBuffer(pgCol* col_buf, const pgPrim* prim)
{
    pgPrim::PrimData* prim_data = prim->m_prim_data;

    for (s32 i = 0; i < prim->m_cur_data_num; i++)
    {
        col_buf[i] = prim->m_final_col * prim_data[i].col;
    }
}


void pgRend::renderCalcUVBuffer(r32* uv_buf, const pgPrim* prim)
{
    pgTex* tex = prim->m_tex->m_proxy_tex ? prim->m_tex->m_proxy_tex : prim->m_tex;
    r32 u_param_a = tex->m_u_param_a;
    r32 u_param_b = tex->m_u_param_b;
    r32 v_param_a = tex->m_v_param_a;
    r32 v_param_b = tex->m_v_param_b;

    for (u32 i = 0; i < prim->m_cur_data_num; i++)
    {
        pgPrim::PrimData* prim_data = &prim->m_prim_data[i];

        *uv_buf = prim_data->u * u_param_a + u_param_b;
        uv_buf++;

        *uv_buf = prim_data->v * v_param_a + v_param_b;
        uv_buf++;
    }
}


void pgRend::renderSetTexture(pgTex* tex1, pgTex* tex2, pgTex* tex3, bool is_bilinear)
{
    pgLowLevelAPI::setTexture(tex1 ? tex1->getTexObj() : 0, tex2 ? tex2->getTexObj() : 0, tex3 ? tex3->getTexObj() : 0, is_bilinear);
}


void pgRend::renderSetVertexPointer(u32 stride, const r32* vert)
{
    pgLowLevelAPI::setVertexPointer(stride, vert);
}


void pgRend::renderSetColorPointer(u32 stride, const u8* color)
{
    pgLowLevelAPI::setColorPointer(stride, color);
}


void pgRend::renderSetTexCoordPointer(u32 stride, const r32* uv)
{
    pgLowLevelAPI::setTexCoordPointer(stride, uv);
}


void pgRend::renderSetTexture(const pgPrim* prim)
{
    pgTex* tex = prim->m_tex ? (prim->m_tex->m_proxy_tex ? prim->m_tex->m_proxy_tex : prim->m_tex) : NULL;

    if (tex)
    {
        pgLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, prim->m_draw_flag.isOn(pgDraw::FLAG_BILINEAR));
    }
    else
    {
        pgLowLevelAPI::setTexture(0, 0, 0, false);
    }
}


void pgRend::renderSetVertexPointer(const pgPrim* prim)
{
    pgLowLevelAPI::setVertexPointer(sizeof(pgPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->pos));
}


void pgRend::renderSetColorPointer(const pgPrim* prim)
{
    pgLowLevelAPI::setColorPointer(sizeof(pgPrim::PrimData), reinterpret_cast<const u8*>(&prim->m_prim_data->col));
}


void pgRend::renderSetTexCoordPointer(const pgPrim* prim)
{
    pgLowLevelAPI::setTexCoordPointer(sizeof(pgPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->u));
}


void pgRend::renderDrawArrays(u8 prim_mode, u16 first, u16 count)
{
    pgLowLevelAPI::drawArrays(static_cast<pgLowLevelAPI::DrawMode>(prim_mode), first, count);
}


u32 pgRend::renderGetShaderUniformLocation(const pgShd* shd, u8 index)
{
    return shd->m_uni_loc_tbl[index];
}


u32 pgRend::renderGetShaderAttribLocation(const pgShd* shd, u8 index)
{
    return shd->m_att_loc_tbl[index];
}


u32 pgRend::renderGetShaderTextureLocation(const pgShd* shd, u8 index)
{
    return shd->m_tex_loc_tbl[index];
}


void pgRend::renderSetShader(pgShd* shd)
{
    if (shd)
    {
        pgLowLevelAPI::setShader(shd->getShdObj());
    }
    else
    {
        pgLowLevelAPI::setShader(0);
    }
}


void pgRend::renderSetUniform_s32(u32 location, s32 uniform)
{
    pgLowLevelAPI::setUniform_s32(location, uniform);
}


void pgRend::renderSetUniform_r32(u32 location, r32 uniform)
{
    pgLowLevelAPI::setUniform_r32(location, uniform);
}


void pgRend::renderSetUniform_localToScreen(const pgShd* shd)
{
    pgLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);
}


void pgRend::renderSetAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib)
{
    pgLowLevelAPI::setAttribPointer_r32(location, size, stride, attrib);
}


void pgRend::renderSetAttribPointer_vertex(const pgShd* shd, u32 stride, const r32* vert)
{
    pgLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, stride, vert);
}


void pgRend::renderSetAttribPointer_color(const pgShd* shd, u32 stride, const u8* color)
{
    pgLowLevelAPI::setAttribPointer_color(shd->m_color_loc, stride, color);
}


void pgRend::renderSetAttribPointer_texCoord(const pgShd* shd, u32 stride, const r32* uv)
{
    pgLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, stride, uv);
}


void pgRend::renderSetAttribPointer_vertex(const pgShd* shd, const pgPrim* prim)
{
    pgLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(pgPrim::PrimData), reinterpret_cast<const r32*>(&prim->m_prim_data->pos));
}


void pgRend::renderSetAttribPointer_color(const pgShd* shd, const pgPrim* prim)
{
    pgLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(pgPrim::PrimData), reinterpret_cast<const u8*>(&prim->m_prim_data->col));
}


void pgRend::renderSetAttribPointer_texCoord(const pgShd* shd, const pgPrim* prim)
{
    pgLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(pgPrim::PrimData), &prim->m_prim_data->u);
}


void pgRend::renderDisableAttribPointers(const pgShd* shd)
{
    pgLowLevelAPI::disableAttribPointer(shd->m_vertex_loc);
    pgLowLevelAPI::disableAttribPointer(shd->m_color_loc);
    pgLowLevelAPI::disableAttribPointer(shd->m_texcoord_loc);

    for (s32 i = 0; i < shd->m_att_num; i++)
    {
        pgLowLevelAPI::disableAttribPointer(shd->m_att_loc_tbl[i]);
    }
}


void pgRend::renderCallPrimRenderWithDestroyingBuffer(pgPrim* prim, const pgMat& view)
{
    if (pgDrawMgr::isShaderAvailable())
    {
        prim->render_shader(view);
    }
    else
    {
        prim->render_soft(view);
    }
}


PG_DEFINE_COPY_CONSTRUCTOR(pgRend)


PG_DEFINE_OPERATOR_EQUAL(pgRend)


void pgRend::init2(pgPrim* prim, u32 body_size, u32 data_size, bool is_share_data, void* rend_data)
{
    if (!prim)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    prim->getPrimMode(); // assure the pgPrim is initialized

    if (prim->m_rend)
    {
        prim->m_rend->uninit();
    }

    uninit();

    if (prim->m_is_share_data.getType() != is_share_data)
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (data_size > 0 && is_share_data && !rend_data)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_is_active = true;
    m_rend_body_size = body_size;
    m_rend_data_size = data_size;
    m_prim = prim;
    m_rend_body = (m_rend_body_size > 0) ? pgMalloc(m_rend_body_size) : NULL;

    if (is_share_data)
    {
        m_rend_data = rend_data;
    }
    else if (m_rend_data_size > 0)
    {
        m_rend_data = pgMalloc(m_rend_data_size * m_prim->m_max_data_num);

        initData(m_rend_data, m_prim->m_max_data_num);
    }
    else
    {
        m_rend_data = NULL;
    }

    m_prim->m_rend = this;
}


void pgRend::uninit()
{
    if (m_prim)
    {
        m_prim->m_rend = NULL;
    }

    if (m_rend_body)
    {
        pgFree(m_rend_body);
    }

    if (m_rend_data && !m_prim->m_is_share_data.getType())
    {
        pgFree(m_rend_data);
    }

    m_prim = NULL;
    m_rend_body = NULL;
    m_rend_data = NULL;
}


u16 pgRend::getMaxDataNum() const
{
    return m_prim->getMaxDataNum();
}


void pgRend::reallocData(u16 max_data_num)
{
    if (m_rend_data_size > 0)
    {
        void* new_rend_data = pgMalloc(m_rend_data_size * max_data_num);
        u32 copy_num = pgMath::min(max_data_num, m_prim->m_max_data_num);

        initData(new_rend_data, max_data_num);

        pgMemMgr::memcpy(new_rend_data, m_rend_data, m_rend_data_size * copy_num);

        pgFree(m_rend_data);

        m_rend_data = new_rend_data;
    }
}


void pgRend::copyData(u16 dest_index, const pgPrim* src_prim, u16 src_index)
{
    pgRend* src_rend = src_prim->m_rend;

    if (!src_rend || src_rend->getClassID() != getClassID())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (m_rend_data_size > 0)
    {
        void* src = static_cast<u8*>(src_rend->m_rend_data) + m_rend_data_size * src_index;
        void* dest = static_cast<u8*>(m_rend_data) + m_rend_data_size * dest_index;

        pgMemMgr::memcpy(dest, src, m_rend_data_size);
    }
}
