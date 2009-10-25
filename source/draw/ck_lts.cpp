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

#include "pg_private_macro.h"


pgLts* pgLts::getPrevN() const
{
    pgDrawMgr* ins = pgDrawMgr::instance();
    const pgID* id = ins->m_lts_map.getPrevKeyN(m_id);

    return id ? *ins->m_lts_map.get(*id) : NULL;
}


pgLts* pgLts::getNextN() const
{
    pgDrawMgr* ins = pgDrawMgr::instance();
    const pgID* id = ins->m_lts_map.getNextKeyN(m_id);

    return id ? *ins->m_lts_map.get(*id) : NULL;
}


pgID pgLts::getID() const
{
    return m_id;
}


pgCol pgLts::getAmbientColor() const
{
    return m_amb_col;
}


void pgLts::setAmbientColor(pgCol col)
{
    m_amb_col = col;
}


bool pgLts::isParaLightActive(u8 index) const
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_para_lit[index].is_active.getType();
}


void pgLts::setParaLightActive(u8 index, bool is_active)
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_para_lit[index].is_active = is_active;
}


const pgVec& pgLts::getParaLightDir(u8 index) const
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_para_lit[index].dir;
}


void pgLts::setParaLightDir(u8 index, const pgVec& dir)
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f)
    {
        m_para_lit[index].dir.set(0.0f, -1.0f, 0.0f);
    }
    else
    {
        m_para_lit[index].dir = dir.normalize();
    }
}


pgCol pgLts::getParaLightColor(u8 index) const
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_para_lit[index].col;
}


void pgLts::setParaLightColor(u8 index, pgCol col)
{
    if (index >= MAX_PARA_LIGHT_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ParaLight* para_lit = &m_para_lit[index];

    para_lit->col = col;
    para_lit->col_int = col.r + col.g + col.b;
}


u8 pgLts::getNearLightNum() const
{
    return m_near_lit_num;
}


pgLit* pgLts::getPointLight(pgID lit_id)
{
    pgLit* lit = getPointLightN(lit_id);

    if (!lit)
    {
        pgThrow(ExceptionNotFound);
    }

    return lit;
}


pgLit* pgLts::newPointLight(pgID lit_id)
{
    if (lit_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (getPointLightN(lit_id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    pgLit* lit = pgNew(pgLit)(lit_id);

    m_lit_list.addLast(&lit->m_item);

    return lit;
}


void pgLts::deletePointLight(pgID lit_id)
{
    if (lit_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    m_near_lit_num = 0;

    pgDelete(getPointLight(lit_id), pgLit);
}


pgLit* pgLts::getFirstPointLightN() const
{
    pgList<pgLit>::Item* item = m_lit_list.getFirstN();

    return item ? item->getSelf() : NULL;
}


pgLit* pgLts::getLastPointLightN() const
{
    pgList<pgLit>::Item* item = m_lit_list.getLastN();

    return item ? item->getSelf() : NULL;
}


u32 pgLts::getPointLightNum() const
{
    return m_lit_list.getItemNum();
}


void pgLts::findNearLight(const pgVec& pos)
{
    m_near_lit_num = 0;

    for (u32 i = 0; i < MAX_PARA_LIGHT_NUM; i++)
    {
        ParaLight* para_lit = &m_para_lit[i];

        if (para_lit->is_active.getValue())
        {
            addLightToNearLight(para_lit->dir, para_lit->col, para_lit->col_int);
        }
    }

    for (pgList<pgLit>::Item* item = m_lit_list.getFirstN(); item; item = item->getNextN())
    {
        pgLit* lit = item->getSelf();

        if (!lit->m_is_active.getValue() || //
            pos.x < lit->m_min_bound.x || pos.y < lit->m_min_bound.y || pos.z < lit->m_min_bound.z || //
            pos.x > lit->m_max_bound.x || pos.y > lit->m_max_bound.y || pos.z > lit->m_max_bound.z)
        {
            continue;
        }

        pgVec diff = pos - lit->m_pos;
        r32 sq_dist = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

        if (sq_dist <= lit->m_sq_outer_rad)
        {
            r32 dist = pgMath::sqrt(sq_dist);
            pgVec dir = (dist < pgMath::EPSILON) ? pgVec(0.0f, -1.0f, 0.0f) : diff / dist;
            pgCol col = (dist <= lit->m_inner_rad) ? lit->m_col : lit->m_col * ((lit->m_outer_rad - dist) / (lit->m_outer_rad - lit->m_inner_rad));
            col.a = 255;

            addLightToNearLight(dir, col, col.r + col.g + col.b);
        }
    }
}


const pgVec& pgLts::getNearLightDir(u8 index) const
{
    if (index >= m_near_lit_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_near_lit[m_near_lit_index_tbl[index]].dir;
}


pgCol pgLts::getNearLightColor(u8 index) const
{
    if (index >= m_near_lit_num)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return m_near_lit[m_near_lit_index_tbl[index]].col;
}


void pgLts::clearPointLight()
{
    while (m_lit_list.hasItem())
    {
        pgDelete(m_lit_list.getFirstN()->getSelf(), pgLit);
    }
}


pgLts::pgLts(pgID lts_id)
{
    m_id = lts_id;
    m_near_lit_num = 0;

    setAmbientColor(pgCol::ZERO);

    for (u32 i = 0; i < MAX_PARA_LIGHT_NUM; i++)
    {
        setParaLightActive(i, false);
        setParaLightDir(i, pgVec::X_UNIT);
        setParaLightColor(i, pgCol::ZERO);
    }

    clearPointLight();

    pgDrawMgr::instance()->m_lts_map.add(m_id, this);
}


pgLts::~pgLts()
{
    pgDrawMgr::instance()->m_lts_map.remove(m_id);

    clearPointLight();
}


PG_DEFINE_OPERATOR_EQUAL(pgLts)


pgLit* pgLts::getPointLightN(pgID lit_id)
{
    if (lit_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    for (pgList<pgLit>::Item* item = m_lit_list.getFirstN(); item; item = item->getNextN())
    {
        pgLit* lit = item->getSelf();

        if (lit->m_id == lit_id)
        {
            m_lit_list.addFirst(&lit->m_item);

            return lit;
        }
    }

    return NULL;
}


void pgLts::addLightToNearLight(const pgVec& dir, pgCol col, u16 col_int)
{
    if (m_near_lit_num == 0)
    {
        NearLight* near_lit = &m_near_lit[0];

        near_lit->dir = dir;
        near_lit->col = col;
        near_lit->col_int = col_int;

        m_near_lit_index_tbl[0] = 0;

        m_near_lit_num = 1;

        return;
    }
    else if (m_near_lit_num < MAX_NEAR_LIGHT_NUM)
    {
        NearLight* near_lit = &m_near_lit[m_near_lit_num];

        near_lit->dir = dir;
        near_lit->col = col;
        near_lit->col_int = col_int;

        m_near_lit_index_tbl[m_near_lit_num] = m_near_lit_num;

        m_near_lit_num++;
    }
    else
    {
        u8 index = m_near_lit_index_tbl[m_near_lit_num - 1];
        NearLight* near_lit = &m_near_lit[index];

        if (col_int > near_lit->col_int)
        {
            near_lit->dir = dir;
            near_lit->col = col;
            near_lit->col_int = col_int;
        }
        else
        {
            return;
        }
    }

    if (m_near_lit_num > 1)
    {
        for (s32 i = m_near_lit_num - 1; i > 0; i--)
        {
            u8 cur_index = m_near_lit_index_tbl[i];
            u8 prev_index = m_near_lit_index_tbl[i - 1];

            if (m_near_lit[cur_index].col_int > m_near_lit[prev_index].col_int)
            {
                m_near_lit_index_tbl[i] = prev_index;
                m_near_lit_index_tbl[i - 1] = cur_index;
            }
            else
            {
                return;
            }
        }
    }
}
