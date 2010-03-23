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


ckPrim::PrimData::PrimData()
{
    pos = ckVec::ZERO;
    col = ckCol::FULL;
    u = v = 0.0f;
}


ckPrim::ckPrim()
{
    m_type = TYPE_PRIM;

    m_prim_mode = MODE_TRIANGLE_STRIP;
    m_is_share_data = false;
    m_prim_data = NULL;
    m_cur_data_num = m_max_data_num = 0;
    m_rend = NULL;
}


ckPrim::~ckPrim()
{
    if (m_rend)
    {
        m_rend->uninit();
    }

    if (m_prim_data && !m_is_share_data.getType())
    {
        ckDeleteArray(m_prim_data, PrimData);
    }
}


void ckPrim::init(PrimMode prim_mode, u16 max_data_num, ckID scr_id)
{
    init2(prim_mode, false, NULL, max_data_num, scr_id, NULL);
}


void ckPrim::init(PrimMode prim_mode, u16 max_data_num, ckDraw* parent)
{
    init2(prim_mode, false, NULL, max_data_num, ckID::ZERO, parent);
}


void ckPrim::init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, ckID scr_id)
{
    init2(prim_mode, true, prim_data, max_data_num, scr_id, NULL);
}


void ckPrim::init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, ckDraw* parent)
{
    init2(prim_mode, true, prim_data, max_data_num, ckID::ZERO, parent);
}


bool ckPrim::isShareData() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_is_share_data.getType();
}


ckPrim::PrimMode ckPrim::getPrimMode() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_prim_mode.getType();
}


void ckPrim::setPrimMode(PrimMode prim_mode)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    m_prim_mode = prim_mode;
}


u16 ckPrim::getCurDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_cur_data_num;
}


void ckPrim::setCurDataNum(u16 cur_data_num)
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


u16 ckPrim::getMaxDataNum() const
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    return m_max_data_num;
}


void ckPrim::reallocData(u16 max_data_num)
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

    if (m_rend)
    {
        m_rend->reallocData(max_data_num);
    }

    if (m_prim_data)
    {
        PrimData* new_prim_data;
        ckNewArray(new_prim_data, PrimData, max_data_num);

        u32 copy_num = ckMath::min(max_data_num, m_max_data_num);

        ckMemMgr::memcpy(new_prim_data, m_prim_data, sizeof(PrimData) * copy_num);

        ckDeleteArray(m_prim_data, PrimData);

        m_max_data_num = max_data_num;
        m_prim_data = new_prim_data;
    }
    else
    {
        m_max_data_num = max_data_num;

        ckNewArray(m_prim_data, PrimData, m_max_data_num);
    }

    if (m_cur_data_num > m_max_data_num)
    {
        m_cur_data_num = m_max_data_num;
    }
}


void ckPrim::copyData(u16 dest_index, const ckPrim* src_prim, u16 src_index)
{
    if (m_private_flag.isOff(FLAG_INITIALIZED))
    {
        ckThrow(ExceptionNotInitialized);
    }

    if (!src_prim || dest_index >= m_max_data_num || src_index >= src_prim->m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (m_rend)
    {
        m_rend->copyData(dest_index, src_prim, src_index);
    }

    m_prim_data[dest_index] = src_prim->m_prim_data[src_index];
}


ckVec& ckPrim::dataPos(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].pos;
}


ckCol& ckPrim::dataCol(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].col;
}


r32& ckPrim::dataU(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].u;
}


r32& ckPrim::dataV(u16 index)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return m_prim_data[index].v;
}


void ckPrim::setDataUV(u16 index, r32 u, r32 v)
{
    if (index >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    PrimData* prim_data = &m_prim_data[index];

    prim_data->u = u;
    prim_data->v = v;
}


void ckPrim::setDataRect(u16 index, const ckVec& center, r32 width, r32 height, ckCol col, r32 u1, r32 v1, r32 u2, r32 v2)
{
    if (index + 3 >= m_max_data_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    width /= 2.0f;
    height /= 2.0f;

    PrimData* prim_data = &m_prim_data[index];

    (prim_data + 0)->pos = center + ckVec(-width, height);
    (prim_data + 1)->pos = center + ckVec(-width, -height);
    (prim_data + 2)->pos = center + ckVec(width, -height);
    (prim_data + 3)->pos = center + ckVec(width, height);

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


void ckPrim::render(const ckMat& view)
{
    if (m_rend && m_rend->isActive())
    {
        m_rend->render(view);
    }
    else if (ckDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void ckPrim::init2(PrimMode prim_mode, bool is_share_data, PrimData* prim_data, u16 max_data_num, ckID scr_id, ckDraw* parent)
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
            ckThrow(ExceptionInvalidArgument);
        }

        if (m_prim_data && !m_is_share_data.getType())
        {
            ckDeleteArray(m_prim_data, PrimData);
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


void ckPrim::render_soft(const ckMat& view)
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
    u32 col_size = (m_final_col != ckCol::FULL) ? sizeof(ckCol) * m_cur_data_num : 0;
    u32 uv_size = (tex && tex->m_flag.isOn(ckTex::FLAG_UV_ADJUST)) ? sizeof(r32) * 2 * m_cur_data_num : 0;
    u32 buf_size = col_size + uv_size;

    u8* buf = (buf_size > 0) ? reinterpret_cast<u8*>(ckMemMgr::allocTempBufferForSystem(buf_size)) : 0;

    ckCol* col_buf = reinterpret_cast<ckCol*>(buf);
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

        ckLowLevelAPI::setColorPointer(sizeof(ckCol), reinterpret_cast<const u8*>(col_buf));
    }
    else
    {
        ckLowLevelAPI::setColorPointer(sizeof(PrimData), reinterpret_cast<const u8*>(&m_prim_data->col));
    }

    /*
        setup texture
    */
    if (tex)
    {
        ckLowLevelAPI::setTexture(tex->getTexObj(), 0, 0, m_draw_flag.isOn(FLAG_BILINEAR));

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

            ckLowLevelAPI::setTexCoordPointer(sizeof(r32) * 2, reinterpret_cast<const r32*>(uv_buf));
        }
        else
        {
            ckLowLevelAPI::setTexCoordPointer(sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->u));
        }
    }
    else
    {
        ckLowLevelAPI::setTexture(0, 0, 0, false);
        ckLowLevelAPI::setTexCoordPointer(0, NULL);
    }

    /*
        draw primitives
    */
    ckLowLevelAPI::setVertexPointer(sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->pos));
    ckLowLevelAPI::drawArrays(static_cast<ckLowLevelAPI::DrawMode>(m_prim_mode.getType()), 0, m_cur_data_num);
}


void ckPrim::render_shader(const ckMat& view)
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
        setup color
    */
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[0], m_final_col.r);
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[1], m_final_col.g);
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[2], m_final_col.b);
    ckLowLevelAPI::setUniform_r32(shd->m_uni_loc_tbl[3], m_final_col.a);

    ckLowLevelAPI::setColorPointer(0, NULL);
    ckLowLevelAPI::setAttribPointer_color(shd->m_color_loc, sizeof(PrimData), reinterpret_cast<const u8*>(&m_prim_data->col));

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

        ckLowLevelAPI::setAttribPointer_r32(shd->m_texcoord_loc, 2, sizeof(PrimData), &m_prim_data->u);
    }
    else
    {
        ckLowLevelAPI::setTexture(0, 0, 0, false);
    }

    /*
        draw primitives
    */
    ckLowLevelAPI::setUniform_localToScreen(shd->m_local_to_screen_loc);

    ckLowLevelAPI::setVertexPointer(0, NULL);
    ckLowLevelAPI::setAttribPointer_r32(shd->m_vertex_loc, 3, sizeof(PrimData), reinterpret_cast<const r32*>(&m_prim_data->pos));

    ckLowLevelAPI::drawArrays(static_cast<ckLowLevelAPI::DrawMode>(m_prim_mode.getType()), 0, m_cur_data_num);

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
