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


#include "ck_dbg_all.h"


void ckDbgMgr::drawLine(const ckVec& pos1, const ckVec& pos2, ckCol col, ckID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_cur_draw_prim_num >= ins->m_max_draw_prim_num)
    {
        ins->reallocDrawPrim();
    }

    ckPrim* prim = &ins->m_draw_prim[ins->m_cur_draw_prim_num];

    prim->setScreenID(scr_id);
    prim->setPrimMode(ckPrim::MODE_LINES);
    prim->setCurDataNum(2);
    prim->setSortCenter((pos1 + pos2) / 2.0f);

    prim->dataPos(0) = pos1;
    prim->dataPos(1) = pos2;

    prim->dataCol(0) = col;
    prim->dataCol(1) = col;

    ins->m_cur_draw_prim_num++;
}


void ckDbgMgr::drawPolygon(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, ckCol col, ckID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_cur_draw_prim_num >= ins->m_max_draw_prim_num)
    {
        ins->reallocDrawPrim();
    }

    ckPrim* prim = &ins->m_draw_prim[ins->m_cur_draw_prim_num];

    prim->setScreenID(scr_id);
    prim->setPrimMode(ckPrim::MODE_TRIANGLES);
    prim->setCurDataNum(3);
    prim->setSortCenter((pos1 + pos2 + pos3) / 3.0f);

    prim->dataPos(0) = pos1;
    prim->dataPos(1) = pos2;
    prim->dataPos(2) = pos3;

    prim->dataCol(0) = col;
    prim->dataCol(1) = col;
    prim->dataCol(2) = col;

    ins->m_cur_draw_prim_num++;
}


void ckDbgMgr::drawPolygon(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, const ckVec& pos4, ckCol col, ckID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_cur_draw_prim_num >= ins->m_max_draw_prim_num)
    {
        ins->reallocDrawPrim();
    }

    ckPrim* prim = &ins->m_draw_prim[ins->m_cur_draw_prim_num];

    prim->setScreenID(scr_id);
    prim->setPrimMode(ckPrim::MODE_TRIANGLE_STRIP);
    prim->setCurDataNum(4);
    prim->setSortCenter((pos1 + pos2 + pos3 + pos4) / 4.0f);

    prim->dataPos(0) = pos1;
    prim->dataPos(1) = pos2;
    prim->dataPos(2) = pos3;
    prim->dataPos(3) = pos4;

    prim->dataCol(0) = col;
    prim->dataCol(1) = col;
    prim->dataCol(2) = col;
    prim->dataCol(3) = col;

    ins->m_cur_draw_prim_num++;
}


void ckDbgMgr::drawBox(const ckMat& center, const ckVec& size, ckCol poly_col, ckCol line_col, ckID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckVec vec_x = center.x_axis * (size.x / 2.0f);
    ckVec vec_y = center.y_axis * (size.y / 2.0f);
    ckVec vec_z = center.z_axis * (size.z / 2.0f);

    ckVec pos1 = center.trans + vec_x + vec_y + vec_z;
    ckVec pos2 = center.trans - vec_x + vec_y + vec_z;
    ckVec pos3 = center.trans + vec_x - vec_y + vec_z;
    ckVec pos4 = center.trans - vec_x - vec_y + vec_z;
    ckVec pos5 = center.trans + vec_x + vec_y - vec_z;
    ckVec pos6 = center.trans - vec_x + vec_y - vec_z;
    ckVec pos7 = center.trans + vec_x - vec_y - vec_z;
    ckVec pos8 = center.trans - vec_x - vec_y - vec_z;

    if (poly_col.a > 0)
    {
        drawPolygon(pos2, pos4, pos1, pos3, poly_col, scr_id);
        drawPolygon(pos5, pos7, pos6, pos8, poly_col, scr_id);
        drawPolygon(pos1, pos3, pos5, pos7, poly_col, scr_id);
        drawPolygon(pos6, pos8, pos2, pos4, poly_col, scr_id);
        drawPolygon(pos6, pos2, pos5, pos1, poly_col, scr_id);
        drawPolygon(pos4, pos8, pos3, pos7, poly_col, scr_id);
    }

    if (line_col.a > 0)
    {
        drawLine(pos2, pos4, line_col, scr_id);
        drawLine(pos4, pos3, line_col, scr_id);
        drawLine(pos1, pos2, line_col, scr_id);
        drawLine(pos3, pos1, line_col, scr_id);

        drawLine(pos5, pos7, line_col, scr_id);
        drawLine(pos7, pos8, line_col, scr_id);
        drawLine(pos6, pos5, line_col, scr_id);
        drawLine(pos8, pos6, line_col, scr_id);

        drawLine(pos3, pos7, line_col, scr_id);
        drawLine(pos5, pos1, line_col, scr_id);
        drawLine(pos2, pos6, line_col, scr_id);
        drawLine(pos8, pos4, line_col, scr_id);
    }
}


void ckDbgMgr::drawSphere(const ckVec& center, r32 radius, ckCol poly_col, ckCol line_col, ckID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckScr* scr = ckDrawMgr::getScreen(scr_id);

    const ckVec& view_x = scr->view().x_axis;
    const ckVec& view_y = scr->view().y_axis;
    const ckVec& view_z = scr->view().z_axis;

    ckVec front = center + view_z * radius;
    ckVec back = center - view_z * radius;

    ckVec pos1;
    ckVec pos2 = center + view_x * (ckMath::cos_s32(24 * 14) * radius) + view_y * (ckMath::sin_s32(24 * 14) * radius);

    for (s32 i = 0; i < 15; i++)
    {
        pos1 = pos2;
        pos2 = center + view_x * (ckMath::cos_s32(24 * i) * radius) + view_y * (ckMath::sin_s32(24 * i) * radius);

        if (poly_col.a > 0)
        {
            drawPolygon(pos1, pos2, front, poly_col, scr_id);
            drawPolygon(pos1, pos2, back, poly_col, scr_id);
        }

        if (line_col.a > 0)
        {
            drawLine(pos1, pos2, line_col, scr_id);
        }
    }
}


void ckDbgMgr::drawAxis(const ckMat& world, r32 size, ckID scr_id)
{
    if (!m_instance)
    {
        return;
    }

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    size /= 2.0f;

    drawLine(world.trans, world.trans + world.x_axis * size, ckCol(255, 0, 0), scr_id);
    drawLine(world.trans, world.trans + world.y_axis * size, ckCol(0, 255, 0), scr_id);
    drawLine(world.trans, world.trans + world.z_axis * size, ckCol(0, 0, 255), scr_id);
}
