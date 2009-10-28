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


#include "ck_draw_all.h"

#include "ck_sys_all.h"
#include "ck_util_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckShd* ckShd::getPrevN() const
{
    ckDrawMgr* ins = ckDrawMgr::instance();
    const ckID* id = ins->m_shd_map.getPrevKeyN(m_id);

    return id ? *ins->m_shd_map.get(*id) : NULL;
}


ckShd* ckShd::getNextN() const
{
    ckDrawMgr* ins = ckDrawMgr::instance();
    const ckID* id = ins->m_shd_map.getNextKeyN(m_id);

    return id ? *ins->m_shd_map.get(*id) : NULL;
}


ckID ckShd::getID() const
{
    return m_id;
}


u8 ckShd::getUniformNum() const
{
    return m_uni_num;
}


u8 ckShd::getAttribNum() const
{
    return m_att_num;
}


u8 ckShd::getTextureNum() const
{
    return m_tex_num;
}


bool ckShd::isValid() const
{
    return m_flag.isOff(FLAG_COMPILE_ERROR);
}


ckShd::ckShd(ckID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
{
    m_id = shd_id;
    m_vert_code = NULL;
    m_frag_code = NULL;
    m_uni_num = uni_num;
    m_att_num = att_num;
    m_tex_num = tex_num;
    m_shd_obj = 0;
    m_local_to_screen_loc = 0;
    m_vertex_loc = 0;
    m_color_loc = 0;
    m_texcoord_loc = 0;
    m_uni_loc_tbl = NULL;
    m_att_loc_tbl = NULL;

    m_flag.clear();

    if (ckDrawMgr::isShaderAvailable())
    {
        if (vert_code)
        {
            u32 vert_code_size = ckUtil::strlen(vert_code) + 1;

            m_vert_code = static_cast<char*>(ckMalloc(vert_code_size));
            ckMemMgr::memcpy(m_vert_code, vert_code, vert_code_size);
        }

        if (frag_code)
        {
            u32 frag_code_size = ckUtil::strlen(frag_code) + 1;

            m_frag_code = static_cast<char*>(ckMalloc(frag_code_size));
            ckMemMgr::memcpy(m_frag_code, frag_code, frag_code_size);
        }

        if (m_uni_num > 0)
        {
            ckNewArray(m_uni_loc_tbl, u32, m_uni_num);
        }

        if (m_att_num > 0)
        {
            ckNewArray(m_att_loc_tbl, u32, m_att_num);
        }
    }

    m_flag.setOn(FLAG_UPLOAD);
    getShdObj(); // upload the shader code to the vram

    ckDrawMgr::instance()->m_shd_map.add(shd_id, this);
}


ckShd::~ckShd()
{
    ckDrawMgr::instance()->m_shd_map.remove(m_id);

    if (m_shd_obj != 0)
    {
        ckLowLevelAPI::unregisterShader(m_shd_obj);
    }

    if (m_vert_code)
    {
        ckFree(m_vert_code);
    }

    if (m_frag_code)
    {
        ckFree(m_frag_code);
    }

    if (m_uni_loc_tbl)
    {
        ckDeleteArray(m_uni_loc_tbl, u32);
    }

    if (m_att_loc_tbl)
    {
        ckDeleteArray(m_att_loc_tbl, u32);
    }
}


CK_DEFINE_OPERATOR_EQUAL(ckShd)


u32 ckShd::getShdObj()
{
    if (m_flag.isOn(FLAG_UPLOAD))
    {
        m_shd_obj = ckLowLevelAPI::registerShader(m_vert_code, m_frag_code);

        if (m_shd_obj == 0)
        {
            m_flag.setOn(ckShd::FLAG_COMPILE_ERROR);
        }
        else
        {
            m_local_to_screen_loc = ckLowLevelAPI::getUniformLocation(m_shd_obj, "ck_local_to_screen");
            m_vertex_loc = ckLowLevelAPI::getAttribLocation(m_shd_obj, "ck_vertex");
            m_color_loc = ckLowLevelAPI::getAttribLocation(m_shd_obj, "ck_color");
            m_texcoord_loc = ckLowLevelAPI::getAttribLocation(m_shd_obj, "ck_texcoord");

            char buf[16];

            for (s32 i = 0; i < m_uni_num; i++)
            {
                ckSysMgr::sprintf(buf, 16, "ck_uni_%02d", i);
                m_uni_loc_tbl[i] = ckLowLevelAPI::getUniformLocation(m_shd_obj, buf);
            }

            for (s32 i = 0; i < m_att_num; i++)
            {
                ckSysMgr::sprintf(buf, 16, "ck_att_%02d", i);
                m_att_loc_tbl[i] = ckLowLevelAPI::getAttribLocation(m_shd_obj, buf);
            }

            for (s32 i = 0; i < m_tex_num; i++)
            {
                ckSysMgr::sprintf(buf, 16, "ck_tex_%02d", i);
                m_tex_loc_tbl[i] = ckLowLevelAPI::getUniformLocation(m_shd_obj, buf);
            }
        }

        m_flag.setOff(FLAG_UPLOAD);
    }

    return m_shd_obj;
}
