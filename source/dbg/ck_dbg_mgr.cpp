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


#include <stdarg.h>

#include "pg_dbg_all.h"

#include "pg_sys_all.h"
#include "pg_task_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


const pgID pgDbgMgr::DEBUG_MODE_SCREEN_ID = pgID_("DEBUG_MODE_SCREEN");
const pgID pgDbgMgr::DEBUG_FONT_TEXTURE_ID = pgID_("DEBUG_FONT_TEXTURE");
pgDbgMgr* pgDbgMgr::m_instance = NULL;


PG_DEFINE_MANAGER_IS_CREATED(pgDbgMgr)


PG_DEFINE_MANAGER_CREATE(pgDbgMgr, Last, Second)


PG_DEFINE_MANAGER_DESTROY(pgDbgMgr, Second)


pgDbgMgr::DebugMode pgDbgMgr::getDebugMode()
{
    if (!m_instance)
    {
        return MODE_OFF;
    }

    return instance()->m_dbg_mode.getType();
}


void pgDbgMgr::setDebugMode(DebugMode dbg_mode)
{
    if (!m_instance)
    {
        return;
    }

    pgDbgMgr* ins = instance();

    if (ins->m_dbg_mode == MODE_OFF)
    {
        ins->m_aim_row = ins->m_cur_row = ins->m_end_row;
        ins->m_dbg_mode_scr->moveLast();
    }

    instance()->m_dbg_mode = dbg_mode;
}


void pgDbgMgr::pageUpConsole()
{
    if (!m_instance)
    {
        return;
    }

    pgDbgMgr* ins = instance();

    s32 row_num = ins->calcConsoleRowNum();

    if (ins->m_cur_row >= row_num)
    {
        ins->m_aim_row = ins->m_cur_row - row_num;
    }
}


void pgDbgMgr::pageDownConsole()
{
    if (!m_instance)
    {
        return;
    }

    pgDbgMgr* ins = instance();

    s32 row_num = ins->calcConsoleRowNum();

    ins->m_aim_row = ins->m_cur_row + row_num;

    if (ins->m_aim_row > ins->m_end_row)
    {
        ins->m_aim_row = ins->m_end_row;
    }
}


void pgDbgMgr::trace(const char* str, ...)
{
    if (!m_instance)
    {
        return;
    }

    pgDbgMgr* ins = instance();

    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    PG_VSPRINTF(buf, 256, str);

    pgSysMgr::printf(buf);

    for (char* c = buf; *c != '\0'; c++)
    {
        u32 index = ins->m_end_row % CONSOLE_BUFFER_SIZE;

        if (*c == '\n' || ins->m_console_buf[index].getLength() >= CONSOLE_COLUMN_NUM)
        {
            ins->m_end_row++;
            index = ins->m_end_row % CONSOLE_BUFFER_SIZE;

            if (*c == '\n')
            {
                ins->m_console_buf[index] = "";
            }
            else
            {
                ins->m_console_buf[index] = *c;
            }

            if (ins->m_end_row == ins->m_aim_row + 1)
            {
                ins->m_aim_row = ins->m_end_row;
            }
        }
        else if (*c == '\t')
        {
            u32 space_num = CONSOLE_TAB_WIDTH - ins->m_console_buf[index].getLength() % CONSOLE_TAB_WIDTH;

            for (u32 i = 0; i < space_num; i++)
            {
                ins->m_console_buf[index] += " ";
            }
        }
        else
        {
            ins->m_console_buf[index] += *c;
        }
    }
}


class pgDbgDaemon : public pgTask
{
public:
    pgDbgDaemon() : pgTask(ORDER_MINUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        if (!pgDbgMgr::m_instance)
        {
            return;
        }

        pgDbgMgr* ins = pgDbgMgr::instance();

        if (pgSysMgr::isFramebufferSizeChanged())
        {
            ins->m_dbg_mode_scr->setAreaInFramebuffer(0, 0, pgSysMgr::getFramebufferWidth(), pgSysMgr::getFramebufferHeight());
            ins->m_dbg_mode_scr->setViewSize(pgSysMgr::getFramebufferWidth(), pgSysMgr::getFramebufferHeight());
        }

        u32 cur_draw_prim_num = ins->m_cur_draw_prim_num;

        for (u32 i = 0; i < cur_draw_prim_num; i++)
        {
            ins->m_draw_prim[i].setScreenID(pgDrawMgr::INVISIBLE_SCREEN_ID); // TODO
        }

        ins->m_cur_draw_prim_num = 0;
        ins->m_font_sprt.setCurDataNum(0);

        ins->checkSpecialCommand();

        if (ins->m_dbg_mode != pgDbgMgr::MODE_OFF)
        {
            ins->drawConsole();
        }
    }
};


pgDbgMgr::pgDbgMgr()
{
    m_dbg_mode = MODE_OFF;
    m_draw_prim = NULL;
    m_cur_draw_prim_num = m_max_draw_prim_num = 0;
    m_end_row = m_aim_row = m_cur_row = 0;
    m_dbg_mode_tap_cntr = 0;
    m_dbg_dump_tap_cntr = 0;
    m_scroll_hold_cntr = 0;

    newDebugFontTexture();

    m_dbg_mode_scr = pgDrawMgr::newScreen(DEBUG_MODE_SCREEN_ID);
    m_dbg_mode_scr->setClearMode(false, true);
    m_dbg_mode_scr->setPerspective(false);

    reallocDrawPrim();

    m_font_sprt.init(INITIAL_FONT_SPRT_NUM, DEBUG_MODE_SCREEN_ID);
    m_font_sprt.setTextureID(DEBUG_FONT_TEXTURE_ID);
    m_font_sprt.setCurDataNum(0);
    m_font_sprt.setPreset_defaultBlendHalf();
    m_font_sprt.setDrawFlag(pgDraw::FLAG_BILINEAR, false);

    m_dbg_daemon = pgNewTask(pgDbgDaemon);
}


pgDbgMgr::~pgDbgMgr()
{
    if (pgTaskMgr::isCreated())
    {
        pgDeleteTask(m_dbg_daemon);
    }

    pgDeleteArray(m_draw_prim, pgPrim);

    pgDrawMgr::deleteScreen(DEBUG_MODE_SCREEN_ID);
    pgDrawMgr::deleteTexture(DEBUG_FONT_TEXTURE_ID);
}


PG_DEFINE_OPERATOR_EQUAL(pgDbgMgr)


void pgDbgMgr::reallocDrawPrim()
{
    u16 new_draw_prim_num;
    pgPrim* new_draw_prim;

    if (m_draw_prim)
    {
        new_draw_prim_num = m_max_draw_prim_num * 2;
    }
    else
    {
        new_draw_prim_num = INITIAL_DRAW_PRIM_NUM;
    }

    pgNewArray(new_draw_prim, pgPrim, new_draw_prim_num);

    for (u32 i = 0; i < new_draw_prim_num; i++)
    {
        pgPrim* prim = &new_draw_prim[i];

        prim->init(pgPrim::MODE_LINES, 4, pgDrawMgr::INVISIBLE_SCREEN_ID);
        prim->setPreset_defaultBlendHalf();
    }

    if (m_draw_prim)
    {
        for (u32 i = 0; i < m_cur_draw_prim_num; i++)
        {
            pgPrim* src_prim = &m_draw_prim[i];
            pgPrim* dest_prim = &new_draw_prim[i];

            u16 cur_data_num = src_prim->getCurDataNum();

            dest_prim->setScreenID(src_prim->getScreenID());
            dest_prim->setPrimMode(src_prim->getPrimMode());
            dest_prim->setCurDataNum(cur_data_num);
            dest_prim->setSortCenter(dest_prim->getSortCenter());

            for (u32 j = 0; j < cur_data_num; j++)
            {
                dest_prim->copyData(j, src_prim, j);
            }
        }

        pgDeleteArray(m_draw_prim, pgPrim);
    }

    m_draw_prim = new_draw_prim;
    m_max_draw_prim_num = new_draw_prim_num;
}


void pgDbgMgr::checkSpecialCommand()
{
    u8 cmd_no = 0;

    if (pgKeyMgr::isOn(pgKeyMgr::KEY_D))
    {
        if (pgKeyMgr::isPressed(pgKeyMgr::KEY_1))
        {
            cmd_no = 1;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_2))
        {
            cmd_no = 2;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_3))
        {
            cmd_no = 3;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_4))
        {
            cmd_no = 4;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_5))
        {
            cmd_no = 5;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_6))
        {
            cmd_no = 6;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_7))
        {
            cmd_no = 7;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_8))
        {
            cmd_no = 8;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_PAGEUP))
        {
            cmd_no = 100;
        }
        else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_PAGEDOWN))
        {
            cmd_no = 101;
        }
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_LBUTTON))
    {
        r32 mouse_x = m_dbg_mode_scr->framebufferXToScreenX(pgKeyMgr::getMouseX());
        r32 mouse_y = m_dbg_mode_scr->framebufferYToScreenY(pgKeyMgr::getMouseY());

        if (mouse_x > 0.0f && mouse_y > 0.0f)
        {
            if (m_dbg_mode_tap_cntr < 2)
            {
                m_dbg_mode_tap_cntr++;
            }
            else if (m_dbg_mode_tap_cntr > 2)
            {
                m_dbg_mode_tap_cntr = 1;
            }
        }
        else if ((m_dbg_mode_tap_cntr >= 2 && m_dbg_mode_tap_cntr <= 3 && mouse_x < 0.0f && mouse_y > 0.0f) || //
            (m_dbg_mode_tap_cntr >= 4 && m_dbg_mode_tap_cntr <= 5 && mouse_x < 0.0f && mouse_y < 0.0f) || //
            (m_dbg_mode_tap_cntr >= 6 && m_dbg_mode_tap_cntr <= 7 && mouse_x > 0.0f && mouse_y < 0.0f))
        {
            m_dbg_mode_tap_cntr++;
        }
        else
        {
            m_dbg_mode_tap_cntr = 0;
        }

        if (m_dbg_mode_tap_cntr == 8)
        {
            cmd_no = 1;

            m_dbg_mode_tap_cntr = 0;
            m_dbg_dump_tap_cntr = 0;
            m_scroll_hold_cntr = 0;
        }

        if (m_dbg_mode.getType() == MODE_CONSOLE)
        {
            if (mouse_x > 0.0f && mouse_y > 0.0f)
            {
                if (m_dbg_dump_tap_cntr >= 3 && m_dbg_dump_tap_cntr < 100)
                {
                    cmd_no = m_dbg_dump_tap_cntr - 1;

                    m_dbg_mode_tap_cntr = 0;
                    m_scroll_hold_cntr = 0;
                }

                m_dbg_dump_tap_cntr = 1;
            }
            else if (mouse_x < 0.0f && mouse_y < 0.0f)
            {
                m_dbg_dump_tap_cntr++;
            }
            else
            {
                m_dbg_dump_tap_cntr = 0;
            }
        }
    }

    if (m_dbg_mode.getType() == MODE_CONSOLE)
    {
        r32 mouse_y = m_dbg_mode_scr->framebufferYToScreenY(pgKeyMgr::getMouseY());

        if (pgKeyMgr::isOn(pgKeyMgr::KEY_LBUTTON))
        {
            if (mouse_y > 0.0f)
            {
                m_scroll_hold_cntr++;
            }
            else
            {
                m_scroll_hold_cntr--;
            }

            if (pgMath::abs(m_scroll_hold_cntr) >= pgTaskMgr::getAimFPS())
            {
                cmd_no = (m_scroll_hold_cntr > 0) ? 100 : 101;

                m_dbg_mode_tap_cntr = 0;
                m_dbg_dump_tap_cntr = 0;
                m_scroll_hold_cntr = 0;
            }
        }
        else
        {
            m_scroll_hold_cntr = 0;
        }
    }

    switch (cmd_no)
    {
    case 1:
        if (m_dbg_mode.getType() == MODE_OFF)
        {
            setDebugMode(MODE_MONITOR);
        }
        else if (m_dbg_mode.getType() == MODE_MONITOR)
        {
            setDebugMode(MODE_CONSOLE);
        }
        else
        {
            setDebugMode(MODE_OFF);
        }

        break;

    case 2:
        dumpMemory();
        break;

    case 3:
        dumpTask();
        break;

    case 4:
        dumpResource();
        break;

    case 5:
        dumpConfig();
        break;

    case 6:
        dumpScreen();
        break;

    case 7:
        dumpTexture();
        break;

    case 8:
        dumpShader();
        break;

    case 100:
        pageUpConsole();
        break;

    case 101:
        pageDownConsole();
        break;

    default:
        break;
    }
}


void pgDbgMgr::drawConsole()
{
    static const r32 FONT_WIDTH = static_cast<r32>(DEBUG_FONT_WIDTH);
    static const r32 FONT_HEIGHT = static_cast<r32>(DEBUG_FONT_HEIGHT);

    static const r32 LEFT_MARGIN = 9.0f;
    static const r32 RIGHT_MARGIN = 7.0f;
    static const r32 TOP_MARGIN = 7.0f;
    static const r32 BOTTOM_MARGIN = 7.0f;

    static const r32 FRAME_MARGIN = 4.0f;

    /*
        draw background
    */
    r32 scr_width = m_dbg_mode_scr->getViewWidth();
    r32 scr_height = m_dbg_mode_scr->getViewHeight();

    s32 row_num = (m_dbg_mode == MODE_CONSOLE) ? calcConsoleRowNum() : 0;

    r32 mon_width = FONT_WIDTH * CONSOLE_COLUMN_NUM + LEFT_MARGIN + RIGHT_MARGIN;
    r32 mon_height = FONT_HEIGHT * (3 + row_num) + TOP_MARGIN + BOTTOM_MARGIN;

    r32 bar_width = FONT_WIDTH * CONSOLE_COLUMN_NUM - 1.0f;

    r32 left = (mon_width > scr_width) ? -scr_width / 2.0f : -mon_width / 2.0f;
    r32 top = (mon_height > scr_height) ? scr_height / 2.0f : mon_height / 2.0f;

    r32 frame_left = left + FRAME_MARGIN;
    r32 frame_top = top - FRAME_MARGIN;
    r32 frame_width = mon_width - FRAME_MARGIN * 2.0f;
    r32 frame_height = mon_height - FRAME_MARGIN * 2.0f;

    drawRect(frame_left, frame_top, frame_width, frame_height, -1.0f, pgCol(0, 0, 0, 128));
    drawFrame(frame_left, frame_top, frame_width, frame_height, 0.0f, pgCol(192, 192, 192, 128));

    /*
        draw monitor
    */
    left += LEFT_MARGIN;
    top -= TOP_MARGIN;

    drawString(left, top, pgCol(255, 192, 192, 192), 1, "%.2f FPS", pgTaskMgr::getCurFPS());

    top -= FONT_HEIGHT;

    r32 execute_rate = static_cast<r32>(static_cast<r64>(pgTaskMgr::getExecuteUsecTime()) * pgTaskMgr::getAimFPS() / 1000000.0);
    drawRect(left, top, execute_rate * bar_width, 2.0f, 0.0f, pgCol(255, 0, 0, 192));

    top -= FONT_HEIGHT * 0.5f;

    drawString(left, top, pgCol(192, 255, 192, 192), 1, "%d/%d byte", pgMemMgr::getCurUsedMemorySize(), pgMemMgr::getMaxUsedMemorySize());

    top -= FONT_HEIGHT;

    r32 memory_rate = static_cast<r32>(pgMemMgr::getCurUsedMemorySize()) / pgMemMgr::getMaxUsedMemorySize();
    drawRect(left, top, memory_rate * bar_width, 2.0f, 0.0f, pgCol(0, 255, 0, 192));

    top -= FONT_HEIGHT * 0.5f;

    /*
        draw console
    */
    s32 diff = m_aim_row - m_cur_row;

    if (diff > 0 && diff < 4)
    {
        m_cur_row++;
    }
    else if (diff < 0 && diff > -4)
    {
        m_cur_row--;
    }
    else
    {
        m_cur_row += diff / 4;
    }

    if (m_end_row - m_cur_row + row_num > static_cast<s32>(CONSOLE_BUFFER_SIZE))
    {
        m_cur_row = m_end_row + row_num - CONSOLE_BUFFER_SIZE;
    }

    s32 start_row = (m_cur_row < row_num) ? 0 : m_cur_row - row_num + 1;

    for (s32 i = 0; i < row_num; i++)
    {
        s32 row = start_row + i;

        if (row > m_end_row)
        {
            break;
        }

        pgCol col = (row + row_num > m_end_row) ? pgCol(255, 255, 255, 192) : pgCol(192, 192, 255, 192);

        drawString(left, top - FONT_HEIGHT * i, col, 1, m_console_buf[row % CONSOLE_BUFFER_SIZE].getString());
    }
}


void pgDbgMgr::drawRect(r32 left, r32 top, r32 width, r32 height, r32 z, pgCol col)
{
    r32 x1 = left;
    r32 y1 = top;
    r32 x2 = left + width;
    r32 y2 = top - height;

    pgVec vec1 = pgVec(x1, y1, z);
    pgVec vec2 = pgVec(x1, y2, z);
    pgVec vec3 = pgVec(x2, y1, z);
    pgVec vec4 = pgVec(x2, y2, z);

    pgDbgMgr::drawPolygon(vec1, vec2, vec3, vec4, col, DEBUG_MODE_SCREEN_ID);
}


void pgDbgMgr::drawFrame(r32 left, r32 top, r32 width, r32 height, r32 z, pgCol col)
{
    r32 x1 = left;
    r32 y1 = top;
    r32 x2 = left + width;
    r32 y2 = top - height;

    pgVec vec1 = pgVec(x1, y1, z);
    pgVec vec2 = pgVec(x1, y2, z);
    pgVec vec3 = pgVec(x2, y2, z);
    pgVec vec4 = pgVec(x2, y1, z);

    drawLine(vec1, vec2, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec2, vec3, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec3, vec4, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec4, vec1, col, DEBUG_MODE_SCREEN_ID);
}


s32 pgDbgMgr::calcConsoleRowNum()
{
    return static_cast<s32>(m_dbg_mode_scr->getViewHeight() / DEBUG_FONT_HEIGHT) - 5;
}


PG_DEFINE_MANAGER_INSTANCE(pgDbgMgr)
