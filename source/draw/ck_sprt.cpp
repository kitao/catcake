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


ckSprt::SprtData::SprtData()
{
    pos = ckVec::ZERO;
    col = ckCol::FULL;
    w = h = 0.0f;
    u1 = v1 = 0.0f;
    u2 = v2 = 1.0f;
    ang = 0;
}


ckSprt::ckSprt()
{
    m_type = TYPE_SPRT;

    m_is_share_data = false;
    m_sprt_data = NULL;
    m_cur_data_num = m_max_data_num = 0;
}


ckSprt::~ckSprt()
{
    if (m_sprt_data && !m_is_share_data.getType())
    {
        ckDeleteArray(m_sprt_data, SprtData);
    }
}


void ckSprt::init(u16 max_data_num, ckID scr_id)
{
    init2(false, NULL, max_data_num, scr_id, NULL);
}


void ckSprt::init(u16 max_data_num, ckDraw* parent)
{
    init2(false, NULL, max_data_num, ckID::ZERO, parent);
}


void ckSprt::init(SprtData* sprt_data, u16 max_data_num, ckID scr_id)
{
    init2(true, sprt_data, max_data_num, scr_id, NULL);
}


void ckSprt::init(SprtData* sprt_data, u16 max_data_num, ckDraw* parent)
{
    init2(true, sprt_data, max_data_num, ckID::ZERO, parent);
}


bool ckSprt::isShareData() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_is_share_data.getType();
}


u16 ckSprt::getCurDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_cur_data_num;
}


void ckSprt::setCurDataNum(u16 cur_data_num)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (cur_data_num > m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_cur_data_num = cur_data_num;
}


u16 ckSprt::getMaxDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_max_data_num;
}


void ckSprt::reallocData(u16 max_data_num)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (m_is_share_data.getType())
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (max_data_num == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (m_sprt_data)
    {
        SprtData* new_sprt_data;
        ckNewArray(new_sprt_data, SprtData, max_data_num);

        u32 copy_num = ckMath::min(max_data_num, m_max_data_num);

        ckMemMgr::memcpy(new_sprt_data, m_sprt_data, sizeof(SprtData) * copy_num);

        ckDeleteArray(m_sprt_data, SprtData);

        m_max_data_num = max_data_num;
        m_sprt_data = new_sprt_data;
    }
    else
    {
        m_max_data_num = max_data_num;

        ckNewArray(m_sprt_data, SprtData, m_max_data_num);
    }

    if (m_cur_data_num > m_max_data_num)
    {
        m_cur_data_num = m_max_data_num;
    }
}


void ckSprt::copyData(u16 dest_index, const ckSprt* src_sprt, u16 src_index)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!src_sprt || dest_index >= m_max_data_num || src_index >= src_sprt->m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_sprt_data[dest_index] = src_sprt->m_sprt_data[src_index];
}


ckVec& ckSprt::dataPos(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].pos;
}


ckCol& ckSprt::dataCol(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].col;
}


r32& ckSprt::dataW(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].w;
}


r32& ckSprt::dataH(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].h;
}


r32& ckSprt::dataU1(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].u1;
}


r32& ckSprt::dataV1(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].v1;
}


r32& ckSprt::dataU2(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].u2;
}


r32& ckSprt::dataV2(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].v2;
}


s32& ckSprt::dataAng(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_sprt_data[index].ang;
}


void ckSprt::setDataSize(u16 index, r32 width, r32 height)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    SprtData* sprt_data = &m_sprt_data[index];

    sprt_data->w = width;
    sprt_data->h = height;
}


void ckSprt::setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    SprtData* sprt_data = &m_sprt_data[index];

    sprt_data->u1 = u1;
    sprt_data->v1 = v1;
    sprt_data->u2 = u2;
    sprt_data->v2 = v2;
}


void ckSprt::init2(bool is_share_data, SprtData* sprt_data, u16 max_data_num, ckID scr_id, ckDraw* parent)
{
    m_private_flag.setOn(FLAG_INITIALIZED);

    if (is_share_data)
    {
        if (!sprt_data || max_data_num == 0)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        if (m_sprt_data && !m_is_share_data.getType())
        {
            ckDeleteArray(m_sprt_data, SprtData);
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
    setTextureID(ckID::ZERO);

    if (parent)
    {
        setParent(parent);
    }
    else
    {
        setScreenID(scr_id);
    }
}


void ckSprt::render(const ckMat& view)
{
    if (ckDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void ckSprt::render_soft(const ckMat& view)
{
    if (m_cur_data_num == 0)
    {
        return;
    }

    /*
        reset shader
    */
    ckLowLevelAPI::setShader(0);

    /*
        alloc buffer
    */
    ckTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;
    u32 pos_size = sizeof(ckVec) * 4 * m_cur_data_num;
    u32 col_size = sizeof(ckCol) * 4 * m_cur_data_num;
    u32 uv_size = tex ? sizeof(r32) * 8 * m_cur_data_num : 0;

    u8* buf = reinterpret_cast<u8*>(ckMemMgr::allocTempBufferForSystem(pos_size + col_size + uv_size));

    ckVec* pos_buf = reinterpret_cast<ckVec*>(buf);
    buf += pos_size;

    ckCol* col_buf = reinterpret_cast<ckCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup buffer
    */
    ckVec view_x = view.x_axis.toLocalOf_noTrans(m_world);
    ckVec view_y = view.y_axis.toLocalOf_noTrans(m_world);

    ckVec* pos_ptr = pos_buf;
    ckCol* col_ptr = col_buf;
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
        ckVec vec_w, vec_h;

        if (sprt_data->ang == 0)
        {
            vec_w = view_x * sprt_data->w / 2.0f;
            vec_h = view_y * sprt_data->h / 2.0f;
        }
        else
        {
            r32 sin = ckMath::sin_s32(-sprt_data->ang);
            r32 cos = ckMath::cos_s32(-sprt_data->ang);

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
        ckLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        if (tex->m_flag.isOn(ckTex::FLAG_UV_ADJUST))
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

        ckLowLevelAPI::setTexCoordPointer(sizeof(r32) * 2, uv_buf);
    }
    else
    {
        ckLowLevelAPI::setTexture(0, 0, 0, false);
        ckLowLevelAPI::setTexCoordPointer(0, NULL);
    }

    /*
        draw sprites
    */
    ckLowLevelAPI::setVertexPointer(sizeof(ckVec), reinterpret_cast<const r32*>(pos_buf));
    ckLowLevelAPI::setColorPointer(sizeof(ckCol), reinterpret_cast<const u8*>(col_buf));

    for (s32 i = 0; i < count; i += 4)
    {
        ckLowLevelAPI::drawArrays(ckLowLevelAPI::DRAW_TRIANGLE_FAN, i, 4);
    }
}


void ckSprt::render_shader(const ckMat& view)
{
    if (m_cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    static const ckID s_shader_id[] =
    {
        ckDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID, ckDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID, ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID, //
        ckDrawMgr::DEFAULT_RGB_TEXTURE_SHADER_ID, ckDrawMgr::DEFAULT_RGBA_TEXTURE_SHADER_ID, ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID
    };

    ckTex* tex = m_tex ? (m_tex->m_proxy_tex ? m_tex->m_proxy_tex : m_tex) : NULL;
    ckShd* shd = ckDrawMgr::getShader(tex ? s_shader_id[tex->m_format.getType()] : ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);

    if (shd->isValid())
    {
        ckLowLevelAPI::setShader(shd->getShdObj());
    }
    else
    {
        render_soft(view);
        return;
    }

    /*
        alloc buffer
    */
    u32 pos_size = sizeof(ckVec) * 4 * m_cur_data_num;
    u32 col_size = sizeof(ckCol) * 4 * m_cur_data_num;
    u32 uv_size = tex ? sizeof(r32) * 8 * m_cur_data_num : 0;

    u8* buf = reinterpret_cast<u8*>(ckMemMgr::allocTempBufferForSystem(pos_size + col_size + uv_size));

    ckVec* pos_buf = reinterpret_cast<ckVec*>(buf);
    buf += pos_size;

    ckCol* col_buf = reinterpret_cast<ckCol*>(buf);
    buf += col_size;

    r32* uv_buf = reinterpret_cast<r32*>(buf);

    /*
        setup buffer
    */
    ckVec view_x = view.x_axis.toLocalOf_noTrans(m_world);
    ckVec view_y = view.y_axis.toLocalOf_noTrans(m_world);

    ckVec* pos_ptr = pos_buf;
    ckCol* col_ptr = col_buf;
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
        ckVec vec_w, vec_h;

        if (sprt_data->ang == 0)
        {
            vec_w = view_x * sprt_data->w / 2.0f;
            vec_h = view_y * sprt_data->h / 2.0f;
        }
        else
        {
            r32 sin = ckMath::sin_s32(-sprt_data->ang);
            r32 cos = ckMath::cos_s32(-sprt_data->ang);

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
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[0], m_final_col.r);
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[1], m_final_col.g);
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[2], m_final_col.b);
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[3], m_final_col.a);

    ckLowLevelAPI::setColorPointer(0, NULL);
    ckLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(ckCol), reinterpret_cast<const u8*>(col_buf));

    /*
        setup texture
    */
    ckLowLevelAPI::setTexCoordPointer(0, NULL);

    if (tex)
    {
        ckLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

        ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[4], tex->m_u_param_a);
        ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[5], tex->m_u_param_b);
        ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[6], tex->m_v_param_a);
        ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[7], tex->m_v_param_b);

        ckLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(r32) * 2, uv_buf);
    }
    else
    {
        ckLowLevelAPI::setTexture(0, 0, 0, false);
    }

    /*
        draw sprites
    */
    ckLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);

    ckLowLevelAPI::setVertexPointer(0, NULL);
    ckLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(ckVec), reinterpret_cast<const r32*>(pos_buf));

    for (s32 i = 0; i < count; i += 4)
    {
        ckLowLevelAPI::drawArrays(ckLowLevelAPI::DRAW_TRIANGLE_FAN, i, 4);
    }

    /*
        disable attributes
    */
    ckLowLevelAPI::disableAttribPointer(shd->m_vertex_loc);
    ckLowLevelAPI::disableAttribPointer(shd->m_color_loc);
    ckLowLevelAPI::disableAttribPointer(shd->m_texcoord_loc);

    for (s32 i = 0; i < shd->m_att_num; i++)
    {
        ckLowLevelAPI::disableAttribPointer(shd->m_att_loc_tbl[i]);
    }
}
