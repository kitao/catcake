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


pgPrim::PrimData::PrimData()
{
    pos = pgVec::ZERO;
    col = pgCol::FULL;
    u = v = 0.0f;
}


pgPrim::pgPrim()
{
    m_type = TYPE_PRIM;

    m_prim_mode = MODE_TRIANGLE_STRIP;
    m_is_share_data = false;
    m_prim_data = NULL;
    m_cur_data_num = m_max_data_num = 0;
    m_rend = NULL;
}


pgPrim::~pgPrim()
{
    if (m_rend)
    {
        m_rend->uninit();
    }

    if (m_prim_data && !m_is_share_data.getType())
    {
        pgDeleteArray(m_prim_data, PrimData);
    }
}


void pgPrim::init(PrimMode prim_mode, u16 max_data_num, pgID scr_id)
{
    init2(prim_mode, false, NULL, max_data_num, scr_id, NULL);
}


void pgPrim::init(PrimMode prim_mode, u16 max_data_num, pgDraw* parent)
{
    init2(prim_mode, false, NULL, max_data_num, pgID::ZERO, parent);
}


void pgPrim::init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, pgID scr_id)
{
    init2(prim_mode, true, prim_data, max_data_num, scr_id, NULL);
}


void pgPrim::init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, pgDraw* parent)
{
    init2(prim_mode, true, prim_data, max_data_num, pgID::ZERO, parent);
}


bool pgPrim::isShareData() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_is_share_data.getType();
}


pgPrim::PrimMode pgPrim::getPrimMode() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_prim_mode.getType();
}


void pgPrim::setPrimMode(PrimMode prim_mode)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    m_prim_mode = prim_mode;
}


u16 pgPrim::getCurDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_cur_data_num;
}


void pgPrim::setCurDataNum(u16 cur_data_num)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (cur_data_num > m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_cur_data_num = cur_data_num;
}


u16 pgPrim::getMaxDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_max_data_num;
}


void pgPrim::reallocData(u16 max_data_num)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (m_is_share_data.getType())
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (max_data_num == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (m_rend)
    {
        m_rend->reallocData(max_data_num);
    }

    if (m_prim_data)
    {
        PrimData* new_prim_data;
        pgNewArray(new_prim_data, PrimData, max_data_num);

        u32 copy_num = pgMath::min(max_data_num, m_max_data_num);

        pgMemMgr::memcpy(new_prim_data, m_prim_data, sizeof(PrimData) * copy_num);

        pgDeleteArray(m_prim_data, PrimData);

        m_max_data_num = max_data_num;
        m_prim_data = new_prim_data;
    }
    else
    {
        m_max_data_num = max_data_num;

        pgNewArray(m_prim_data, PrimData, m_max_data_num);
    }

    if (m_cur_data_num > m_max_data_num)
    {
        m_cur_data_num = m_max_data_num;
    }
}


void pgPrim::copyData(u16 dest_index, const pgPrim* src_prim, u16 src_index)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!src_prim || dest_index >= m_max_data_num || src_index >= src_prim->m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (m_rend)
    {
        m_rend->copyData(dest_index, src_prim, src_index);
    }

    m_prim_data[dest_index] = src_prim->m_prim_data[src_index];
}


pgVec& pgPrim::dataPos(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].pos;
}


pgCol& pgPrim::dataCol(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].col;
}


r32& pgPrim::dataU(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].u;
}


r32& pgPrim::dataV(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].v;
}


void pgPrim::setDataUV(u16 index, r32 u, r32 v)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    PrimData* prim_data = &m_prim_data[index];

    prim_data->u = u;
    prim_data->v = v;
}


void pgPrim::setDataRect(u16 index, const pgVec& center, r32 width, r32 height, pgCol col, r32 u1, r32 v1, r32 u2, r32 v2)
{
    if (index + 3 >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    width /= 2.0f;
    height /= 2.0f;

    PrimData* prim_data = &m_prim_data[index];

    (prim_data + 0)->pos = center + pgVec(-width, height);
    (prim_data + 1)->pos = center + pgVec(-width, -height);
    (prim_data + 2)->pos = center + pgVec(width, -height);
    (prim_data + 3)->pos = center + pgVec(width, height);

    (prim_data + 0)->col = col;
    (prim_data + 1)->col = col;
    (prim_data + 2)->col = col;
    (prim_data + 3)->col = col;

    (prim_data + 0)->u = u1;
    (prim_data + 0)->v = v1;
    (prim_data + 1)->u = u1;
    (prim_data + 1)->v = v2;
    (prim_data + 2)->u = u2;
    (prim_data + 2)->v = v2;
    (prim_data + 3)->u = u2;
    (prim_data + 3)->v = v1;
}


void pgPrim::render(const pgMat& view)
{
    if (m_rend && m_rend->isActive())
    {
        m_rend->render(view);
    }
    else if (pgDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void pgPrim::init2(PrimMode prim_mode, bool is_share_data, PrimData* prim_data, u16 max_data_num, pgID scr_id, pgDraw* parent)
{
    if (m_rend)
    {
        m_rend->uninit();
    }

    m_private_flag.setOn(FLAG_INITIALIZED);

    setPrimMode(prim_mode);

    if (is_share_data)
    {
        if (!prim_data || max_data_num == 0)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        if (m_prim_data && !m_is_share_data.getType())
        {
            pgDeleteArray(m_prim_data, PrimData);
        }

        m_is_share_data = true;
        m_prim_data = const_cast<PrimData*>(prim_data);
        m_max_data_num = max_data_num;
    }
    else
    {
        if (m_prim_data && m_is_share_data.getType())
        {
            m_prim_data = NULL;
            m_max_data_num = 0;
        }

        m_is_share_data = false;

        reallocData(max_data_num);
    }

    setCurDataNum(max_data_num);
    setTextureID(pgID::ZERO);

    if (parent)
    {
        setParent(parent);
    }
    else
    {
        setScreenID(scr_id);
    }
}


void pgPrim::render_soft(const pgMat& view)
{
    if (m_cur_data_num == 0)
    {
        return;
    }

    /*
        reset shader
    */
    pgLowLevelAPI::setShader(0);

    /*
        alloc buffer
    */
    pgTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;
    u32 col_size = (m_final_col != pgCol::FULL) ? sizeof(pgCol) * m_cur_data_num : 0;
    u32 uv_size = (tex && tex->m_flag.isOn(pgTex::FLAG_UV_ADJUST)) ? sizeof(r32) * 2 * m_cur_data_num : 0;
    u32 buf_size = col_size + uv_size;

    u8* buf = (buf_size > 0) ? reinterpret_cast<u8*>(pgMemMgr::allocTempBufferForSystem(buf_size)) : 0;

    pgCol* col_buf = reinterpret_cast<pgCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup color
    */
    if (col_size > 0)
    {
        for (s32 i = 0; i < m_cur_data_num; i++)
        {
            col_buf[i] = m_final_col * m_prim_data[i].col;
        }

        pgLowLevelAPI::setColorPointer(sizeof(pgCol), reinterpret_cast<const u8*>(col_buf));
    }
    else
    {
        pgLowLevelAPI::setColorPointer(sizeof(PrimData), reinterpret_cast<const u8*>(&m_prim_data->col));
    }

    /*
        setup texture
    */
    if (tex)
    {
        pgLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        if (uv_size > 0)
        {
            r32* uv_ptr = uv_buf;

            for (u32 i = 0; i < m_cur_data_num; i++)
            {
                PrimData* prim_data = &m_prim_data[i];

                *uv_ptr = prim_data->u * tex->m_u_param_a + tex->m_u_param_b;
                uv_ptr++;

                *uv_ptr = prim_data->v * tex->m_v_param_a + tex->m_v_param_b;
                uv_ptr++;
            }

            pgLowLevelAPI::setTexCoordPointer(sizeof(r32) * 2, reinterpret_cast<const r32*>(uv_buf));
        }
        else
        {
            pgLowLevelAPI::setTexCoordPointer(sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->u));
        }
    }
    else
    {
        pgLowLevelAPI::setTexture(0, 0, 0, false);
        pgLowLevelAPI::setTexCoordPointer(0, NULL);
    }

    /*
        draw primitives
    */
    pgLowLevelAPI::setVertexPointer(sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->pos));
    pgLowLevelAPI::drawArrays(static_cast<pgLowLevelAPI::DrawMode>(m_prim_mode.getType()), 0, m_cur_data_num);
}


void pgPrim::render_shader(const pgMat& view)
{
    if (m_cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    pgShd* shd = pgDrawMgr::getShader(pgDrawMgr::DEFAULT_SHADER_ID);

    if (shd->isValid())
    {
        pgLowLevelAPI::setShader(shd->getShdObj());
    }
    else
    {
        render_soft(view);
        return;
    }

    /*
        setup color
    */
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[0], m_final_col.r);
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[1], m_final_col.g);
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[2], m_final_col.b);
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[3], m_final_col.a);

    pgLowLevelAPI::setColorPointer(0, NULL);
    pgLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(PrimData), reinterpret_cast<const u8*>(&m_prim_data->col));

    /*
        setup texture
    */
    pgLowLevelAPI::setTexCoordPointer(0, NULL);

    pgTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;

    if (tex)
    {
        pgLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[4], tex->m_u_param_a);
        pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[5], tex->m_u_param_b);
        pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[6], tex->m_v_param_a);
        pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[7], tex->m_v_param_b);

        switch (tex->m_format.getType())
        {
        case pgTex::FORMAT_RGB:
        case pgTex::FORMAT_PNG_RGB:
            pgLowLevelAPI::setUniform_s32(shd->m_uni_loc_tbl[8], 1);
            break;

        case pgTex::FORMAT_RGBA:
        case pgTex::FORMAT_PNG_RGBA:
            pgLowLevelAPI::setUniform_s32(shd->m_uni_loc_tbl[8], 2);
            break;

        case pgTex::FORMAT_ALPHA:
        case pgTex::FORMAT_PNG_ALPHA:
            pgLowLevelAPI::setUniform_s32(shd->m_uni_loc_tbl[8], 3);
            break;
        }

        pgLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(PrimData), &m_prim_data->u);
    }
    else
    {
        pgLowLevelAPI::setTexture(0, 0, 0, false);
        pgLowLevelAPI::setUniform_s32(shd->m_uni_loc_tbl[8], 0);
    }

    /*
        draw primitives
    */
    pgLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);

    pgLowLevelAPI::setVertexPointer(0, NULL);
    pgLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->pos));

    pgLowLevelAPI::drawArrays(static_cast<pgLowLevelAPI::DrawMode>(m_prim_mode.getType()), 0, m_cur_data_num);

    /*
        disable attributes
    */
    pgLowLevelAPI::disableAttribPointer(shd->m_vertex_loc);
    pgLowLevelAPI::disableAttribPointer(shd->m_color_loc);
    pgLowLevelAPI::disableAttribPointer(shd->m_texcoord_loc);

    for (s32 i = 0; i < shd->m_att_num; i++)
    {
        pgLowLevelAPI::disableAttribPointer(shd->m_att_loc_tbl[i]);
    }
}
