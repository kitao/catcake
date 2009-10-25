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


pgSprt::SprtData::SprtData()
{
    pos = pgVec::ZERO;
    col = pgCol::FULL;
    w = h = 0.0f;
    u1 = v1 = 0.0f;
    u2 = v2 = 1.0f;
    ang = 0;
}


pgSprt::pgSprt()
{
    m_type = TYPE_SPRT;

    m_is_share_data = false;
    m_sprt_data = NULL;
    m_cur_data_num = m_max_data_num = 0;
}


pgSprt::~pgSprt()
{
    if (m_sprt_data && !m_is_share_data.getType())
    {
        pgDeleteArray(m_sprt_data, SprtData);
    }
}


void pgSprt::init(u16 max_data_num, pgID scr_id)
{
    init2(false, NULL, max_data_num, scr_id, NULL);
}


void pgSprt::init(u16 max_data_num, pgDraw* parent)
{
    init2(false, NULL, max_data_num, pgID::ZERO, parent);
}


void pgSprt::init(SprtData* sprt_data, u16 max_data_num, pgID scr_id)
{
    init2(true, sprt_data, max_data_num, scr_id, NULL);
}


void pgSprt::init(SprtData* sprt_data, u16 max_data_num, pgDraw* parent)
{
    init2(true, sprt_data, max_data_num, pgID::ZERO, parent);
}


bool pgSprt::isShareData() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_is_share_data.getType();
}


u16 pgSprt::getCurDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_cur_data_num;
}


void pgSprt::setCurDataNum(u16 cur_data_num)
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


u16 pgSprt::getMaxDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    return m_max_data_num;
}


void pgSprt::reallocData(u16 max_data_num)
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

    if (m_sprt_data)
    {
        SprtData* new_sprt_data;
        pgNewArray(new_sprt_data, SprtData, max_data_num);

        u32 copy_num = pgMath::min(max_data_num, m_max_data_num);

        pgMemMgr::memcpy(new_sprt_data, m_sprt_data, sizeof(SprtData) * copy_num);

        pgDeleteArray(m_sprt_data, SprtData);

        m_max_data_num = max_data_num;
        m_sprt_data = new_sprt_data;
    }
    else
    {
        m_max_data_num = max_data_num;

        pgNewArray(m_sprt_data, SprtData, m_max_data_num);
    }

    if (m_cur_data_num > m_max_data_num)
    {
        m_cur_data_num = m_max_data_num;
    }
}


void pgSprt::copyData(u16 dest_index, const pgSprt* src_sprt, u16 src_index)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        pgThrow(ExceptionNotInitialized);
    }

    if (!src_sprt || dest_index >= m_max_data_num || src_index >= src_sprt->m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_sprt_data[dest_index] = src_sprt->m_sprt_data[src_index];
}


pgVec& pgSprt::dataPos(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].pos;
}


pgCol& pgSprt::dataCol(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].col;
}


r32& pgSprt::dataW(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].w;
}


r32& pgSprt::dataH(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].h;
}


r32& pgSprt::dataU1(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].u1;
}


r32& pgSprt::dataV1(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].v1;
}


r32& pgSprt::dataU2(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].u2;
}


r32& pgSprt::dataV2(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].v2;
}


s32& pgSprt::dataAng(u16 index)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].ang;
}


void pgSprt::setDataSize(u16 index, r32 width, r32 height)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    SprtData* sprt_data = &m_sprt_data[index];

    sprt_data->w = width;
    sprt_data->h = height;
}


void pgSprt::setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2)
{
    if (index >= m_max_data_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    SprtData* sprt_data = &m_sprt_data[index];

    sprt_data->u1 = u1;
    sprt_data->v1 = v1;
    sprt_data->u2 = u2;
    sprt_data->v2 = v2;
}


void pgSprt::init2(bool is_share_data, SprtData* sprt_data, u16 max_data_num, pgID scr_id, pgDraw* parent)
{
    m_private_flag.setOn(FLAG_INITIALIZED);

    if (is_share_data)
    {
        if (!sprt_data || max_data_num == 0)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        if (m_sprt_data && !m_is_share_data.getType())
        {
            pgDeleteArray(m_sprt_data, SprtData);
        }

        m_is_share_data = true;
        m_sprt_data = const_cast<SprtData*>(sprt_data);
        m_max_data_num = max_data_num;
    }
    else
    {
        if (m_sprt_data && m_is_share_data.getType())
        {
            m_sprt_data = NULL;
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


void pgSprt::render(const pgMat& view)
{
    if (pgDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void pgSprt::render_soft(const pgMat& view)
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
    u32 pos_size = sizeof(pgVec) * 4 * m_cur_data_num;
    u32 col_size = sizeof(pgCol) * 4 * m_cur_data_num;
    u32 uv_size = tex ? sizeof(r32) * 8 * m_cur_data_num : 0;

    u8* buf = reinterpret_cast<u8*>(pgMemMgr::allocTempBufferForSystem(pos_size + col_size + uv_size));

    pgVec* pos_buf = reinterpret_cast<pgVec*>(buf);
    buf += pos_size;

    pgCol* col_buf = reinterpret_cast<pgCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup buffer
    */
    pgVec view_x = view.x_axis.toLocalOf_noTrans(m_world);
    pgVec view_y = view.y_axis.toLocalOf_noTrans(m_world);

    pgVec* pos_ptr = pos_buf;
    pgCol* col_ptr = col_buf;
    r32* uv_ptr = uv_buf;

    u16 count = 0;

    for (s32 i = 0; i < m_cur_data_num; i++)
    {
        if (m_final_col.a == 0)
        {
            continue;
        }

        count += 4;

        SprtData* sprt_data = &m_sprt_data[i];
        pgVec vec_w, vec_h;

        if (sprt_data->ang == 0)
        {
            vec_w = view_x * sprt_data->w / 2.0f;
            vec_h = view_y * sprt_data->h / 2.0f;
        }
        else
        {
            r32 sin = pgMath::sin_s32(-sprt_data->ang);
            r32 cos = pgMath::cos_s32(-sprt_data->ang);

            vec_w = (view_x * cos - view_y * sin) * sprt_data->w / 2.0f;
            vec_h = (view_y * cos + view_x * sin) * sprt_data->h / 2.0f;
        }

        *(pos_ptr + 0) = sprt_data->pos - vec_w + vec_h;
        *(pos_ptr + 1) = sprt_data->pos - vec_w - vec_h;
        *(pos_ptr + 2) = sprt_data->pos + vec_w - vec_h;
        *(pos_ptr + 3) = sprt_data->pos + vec_w + vec_h;

        pos_ptr += 4;

        *(col_ptr + 0) = *(col_ptr + 1) = *(col_ptr + 2) = *(col_ptr + 3) = m_final_col * sprt_data->col;

        col_ptr += 4;

        if (tex)
        {
            r32 u1 = sprt_data->u1;
            r32 v1 = sprt_data->v1;
            r32 u2 = sprt_data->u2;
            r32 v2 = sprt_data->v2;

            *(uv_ptr + 0) = u1;
            *(uv_ptr + 1) = v1;

            *(uv_ptr + 2) = u1;
            *(uv_ptr + 3) = v2;

            *(uv_ptr + 4) = u2;
            *(uv_ptr + 5) = v2;

            *(uv_ptr + 6) = u2;
            *(uv_ptr + 7) = v1;

            uv_ptr += 8;
        }
    }

    /*
        setup texture
    */
    if (tex)
    {
        pgLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        if (tex->m_flag.isOn(pgTex::FLAG_UV_ADJUST))
        {
            r32* uv_ptr = uv_buf;

            for (s32 i = 0; i < count; i++)
            {
                *uv_ptr = *uv_ptr * tex->m_u_param_a + tex->m_u_param_b;
                uv_ptr++;

                *uv_ptr = *uv_ptr * tex->m_v_param_a + tex->m_v_param_b;
                uv_ptr++;
            }
        }

        pgLowLevelAPI::setTexCoordPointer(sizeof(r32) * 2, uv_buf);
    }
    else
    {
        pgLowLevelAPI::setTexture(0, 0, 0, false);
        pgLowLevelAPI::setTexCoordPointer(0, NULL);
    }

    /*
        draw sprites
    */
    pgLowLevelAPI::setVertexPointer(sizeof(pgVec), reinterpret_cast<const r32*>(pos_buf));
    pgLowLevelAPI::setColorPointer(sizeof(pgCol), reinterpret_cast<const u8*>(col_buf));

    for (s32 i = 0; i < count; i += 4)
    {
        pgLowLevelAPI::drawArrays(pgLowLevelAPI::DRAW_TRIANGLE_FAN, i, 4);
    }
}


void pgSprt::render_shader(const pgMat& view)
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
        alloc buffer
    */
    pgTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;
    u32 pos_size = sizeof(pgVec) * 4 * m_cur_data_num;
    u32 col_size = sizeof(pgCol) * 4 * m_cur_data_num;
    u32 uv_size = tex ? sizeof(r32) * 8 * m_cur_data_num : 0;

    u8* buf = reinterpret_cast<u8*>(pgMemMgr::allocTempBufferForSystem(pos_size + col_size + uv_size));

    pgVec* pos_buf = reinterpret_cast<pgVec*>(buf);
    buf += pos_size;

    pgCol* col_buf = reinterpret_cast<pgCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup buffer
    */
    pgVec view_x = view.x_axis.toLocalOf_noTrans(m_world);
    pgVec view_y = view.y_axis.toLocalOf_noTrans(m_world);

    pgVec* pos_ptr = pos_buf;
    pgCol* col_ptr = col_buf;
    r32* uv_ptr = uv_buf;

    u16 count = 0;

    for (s32 i = 0; i < m_cur_data_num; i++)
    {
        if (m_final_col.a == 0)
        {
            continue;
        }

        count += 4;

        SprtData* sprt_data = &m_sprt_data[i];
        pgVec vec_w, vec_h;

        if (sprt_data->ang == 0)
        {
            vec_w = view_x * sprt_data->w / 2.0f;
            vec_h = view_y * sprt_data->h / 2.0f;
        }
        else
        {
            r32 sin = pgMath::sin_s32(-sprt_data->ang);
            r32 cos = pgMath::cos_s32(-sprt_data->ang);

            vec_w = (view_x * cos - view_y * sin) * sprt_data->w / 2.0f;
            vec_h = (view_y * cos + view_x * sin) * sprt_data->h / 2.0f;
        }

        *(pos_ptr + 0) = sprt_data->pos - vec_w + vec_h;
        *(pos_ptr + 1) = sprt_data->pos - vec_w - vec_h;
        *(pos_ptr + 2) = sprt_data->pos + vec_w - vec_h;
        *(pos_ptr + 3) = sprt_data->pos + vec_w + vec_h;

        pos_ptr += 4;

        *(col_ptr + 0) = *(col_ptr + 1) = *(col_ptr + 2) = *(col_ptr + 3) = sprt_data->col;

        col_ptr += 4;

        if (tex)
        {
            r32 u1 = sprt_data->u1;
            r32 v1 = sprt_data->v1;
            r32 u2 = sprt_data->u2;
            r32 v2 = sprt_data->v2;

            *(uv_ptr + 0) = u1;
            *(uv_ptr + 1) = v1;

            *(uv_ptr + 2) = u1;
            *(uv_ptr + 3) = v2;

            *(uv_ptr + 4) = u2;
            *(uv_ptr + 5) = v2;

            *(uv_ptr + 6) = u2;
            *(uv_ptr + 7) = v1;

            uv_ptr += 8;
        }
    }

    /*
        setup color
    */
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[0], m_final_col.r);
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[1], m_final_col.g);
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[2], m_final_col.b);
    pgLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[3], m_final_col.a);

    pgLowLevelAPI::setColorPointer(0, NULL);
    pgLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(pgCol), reinterpret_cast<const u8*>(col_buf));

    /*
        setup texture
    */
    pgLowLevelAPI::setTexCoordPointer(0, NULL);

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

        pgLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(r32) * 2, uv_buf);
    }
    else
    {
        pgLowLevelAPI::setTexture(0, 0, 0, false);
        pgLowLevelAPI::setUniform_s32(shd->m_uni_loc_tbl[8], 0);
    }

    /*
        draw sprites
    */
    pgLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);

    pgLowLevelAPI::setVertexPointer(0, NULL);
    pgLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(pgVec), reinterpret_cast<const r32*>(pos_buf));

    for (s32 i = 0; i < count; i += 4)
    {
        pgLowLevelAPI::drawArrays(pgLowLevelAPI::DRAW_TRIANGLE_FAN, i, 4);
    }

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
