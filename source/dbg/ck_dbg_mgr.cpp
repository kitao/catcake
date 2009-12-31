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


#include <stdarg.h>

#include "ck_dbg_all.h"

#include "ck_sys_all.h"
#include "ck_task_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


const ckID ckDbgMgr::DEBUG_MODE_SCREEN_ID = ckID_("DEBUG_MODE_SCREEN");
const ckID ckDbgMgr::DEBUG_FONT_TEXTURE_ID = ckID_("DEBUG_FONT_TEXTURE");
ckDbgMgr* ckDbgMgr::m_instance = NULL;


CK_DEFINE_MANAGER_IS_CREATED(ckDbgMgr)


CK_DEFINE_MANAGER_CREATE(ckDbgMgr, Last, Second)


CK_DEFINE_MANAGER_DESTROY(ckDbgMgr, Second)


ckDbgMgr::DebugMode ckDbgMgr::getDebugMode()
{
    if (!m_instance)
    {
        return MODE_OFF;
    }

    return instance()->m_dbg_mode.getType();
}


void ckDbgMgr::setDebugMode(DebugMode dbg_mode)
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    if (ins->m_dbg_mode == MODE_OFF)
    {
        ins->m_aim_row = ins->m_cur_row = ins->m_end_row;
        ins->m_dbg_mode_scr->moveLast();
    }

    instance()->m_dbg_mode = dbg_mode;
}


void ckDbgMgr::pageUpConsole()
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    s32 row_num = ins->calcConsoleRowNum();

    if (ins->m_cur_row >= row_num)
    {
        ins->m_aim_row = ins->m_cur_row - row_num;
    }
}


void ckDbgMgr::pageDownConsole()
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    s32 row_num = ins->calcConsoleRowNum();

    ins->m_aim_row = ins->m_cur_row + row_num;

    if (ins->m_aim_row > ins->m_end_row)
    {
        ins->m_aim_row = ins->m_end_row;
    }
}


void ckDbgMgr::trace(const char* str, ...)
{
    if (!m_instance)
    {
        return;
    }

    ckDbgMgr* ins = instance();

    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    CK_VSPRINTF(buf, 256, str);

    ckSysMgr::printf(buf);

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


class ckDbgDaemon : public ckTask
{
public:
    ckDbgDaemon() : ckTask(ORDER_MINUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        if (!ckDbgMgr::m_instance)
        {
            return;
        }

        ckDbgMgr* ins = ckDbgMgr::instance();

        if (ckSysMgr::isFramebufferSizeChanged())
        {
            ins->m_dbg_mode_scr->setAreaInFramebuffer(0, 0, ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
            ins->m_dbg_mode_scr->setViewSize(ckSysMgr::getFramebufferWidth(), ckSysMgr::getFramebufferHeight());
        }

        u32 cur_draw_prim_num = ins->m_cur_draw_prim_num;

        for (u32 i = 0; i < cur_draw_prim_num; i++)
        {
            ins->m_draw_prim[i].setScreenID(ckDrawMgr::INVISIBLE_SCREEN_ID); // TODO
        }

        ins->m_cur_draw_prim_num = 0;
        ins->m_font_sprt.setCurDataNum(0);

        ins->checkSpecialCommand();

        if (ins->m_dbg_mode != ckDbgMgr::MODE_OFF)
        {
            ins->drawConsole();
        }
    }
};


ckDbgMgr::ckDbgMgr()
{
    m_dbg_mode = MODE_OFF;
    m_draw_prim = NULL;
    m_cur_draw_prim_num = m_max_draw_prim_num = 0;
    m_end_row = m_aim_row = m_cur_row = 0;
    m_dbg_mode_tap_cntr = 0;
    m_dbg_dump_tap_cntr = 0;
    m_scroll_hold_cntr = 0;

    newDebugFontTexture();

    m_dbg_mode_scr = ckDrawMgr::newScreen(DEBUG_MODE_SCREEN_ID);
    m_dbg_mode_scr->setClearMode(false, true);
    m_dbg_mode_scr->setPerspective(false);

    reallocDrawPrim();

    m_font_sprt.init(INITIAL_FONT_SPRT_NUM, DEBUG_MODE_SCREEN_ID);
    m_font_sprt.setTextureID(DEBUG_FONT_TEXTURE_ID);
    m_font_sprt.setCurDataNum(0);
    m_font_sprt.setBlendMode(ckDraw::BLEND_HALF, true);
    m_font_sprt.setDrawFlag(ckDraw::FLAG_BILINEAR, false);

    m_dbg_daemon = ckNewTask(ckDbgDaemon);
}


ckDbgMgr::~ckDbgMgr()
{
    if (ckTaskMgr::isCreated())
    {
        ckDeleteTask(m_dbg_daemon);
    }

    ckDeleteArray(m_draw_prim, ckPrim);

    ckDrawMgr::deleteScreen(DEBUG_MODE_SCREEN_ID);
    ckDrawMgr::deleteTexture(DEBUG_FONT_TEXTURE_ID);
}


CK_DEFINE_OPERATOR_EQUAL(ckDbgMgr)


void ckDbgMgr::reallocDrawPrim()
{
    u16 new_draw_prim_num;
    ckPrim* new_draw_prim;

    if (m_draw_prim)
    {
        new_draw_prim_num = m_max_draw_prim_num * 2;
    }
    else
    {
        new_draw_prim_num = INITIAL_DRAW_PRIM_NUM;
    }

    ckNewArray(new_draw_prim, ckPrim, new_draw_prim_num);

    for (u32 i = 0; i < new_draw_prim_num; i++)
    {
        ckPrim* prim = &new_draw_prim[i];

        prim->init(ckPrim::MODE_LINES, 4, ckDrawMgr::INVISIBLE_SCREEN_ID);
        prim->setBlendMode(ckDraw::BLEND_HALF, true);
    }

    if (m_draw_prim)
    {
        for (u32 i = 0; i < m_cur_draw_prim_num; i++)
        {
            ckPrim* src_prim = &m_draw_prim[i];
            ckPrim* dest_prim = &new_draw_prim[i];

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

        ckDeleteArray(m_draw_prim, ckPrim);
    }

    m_draw_prim = new_draw_prim;
    m_max_draw_prim_num = new_draw_prim_num;
}


void ckDbgMgr::checkSpecialCommand()
{
    u8 cmd_no = 0;

    if (ckKeyMgr::isOn(ckKeyMgr::KEY_D))
    {
        if (ckKeyMgr::isPressed(ckKeyMgr::KEY_1))
        {
            cmd_no = 1;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_2))
        {
            cmd_no = 2;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_3))
        {
            cmd_no = 3;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_4))
        {
            cmd_no = 4;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_5))
        {
            cmd_no = 5;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_6))
        {
            cmd_no = 6;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_7))
        {
            cmd_no = 7;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_8))
        {
            cmd_no = 8;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_PAGEUP))
        {
            cmd_no = 100;
        }
        else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_PAGEDOWN))
        {
            cmd_no = 101;
        }
    }

    if (ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON))
    {
        r32 mouse_x = m_dbg_mode_scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
        r32 mouse_y = m_dbg_mode_scr->framebufferYToScreenY(ckKeyMgr::getMouseY());

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
        r32 mouse_y = m_dbg_mode_scr->framebufferYToScreenY(ckKeyMgr::getMouseY());

        if (ckKeyMgr::isOn(ckKeyMgr::KEY_LBUTTON))
        {
            if (mouse_y > 0.0f)
            {
                m_scroll_hold_cntr++;
            }
            else
            {
                m_scroll_hold_cntr--;
            }

            if (ckMath::abs(m_scroll_hold_cntr) >= ckTaskMgr::getAimFPS())
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


void ckDbgMgr::drawConsole()
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

    drawRect(frame_left, frame_top, frame_width, frame_height, -1.0f, ckCol(0, 0, 0, 128));
    drawFrame(frame_left, frame_top, frame_width, frame_height, 0.0f, ckCol(192, 192, 192, 128));

    /*
        draw monitor
    */
    left += LEFT_MARGIN;
    top -= TOP_MARGIN;

    drawString(left, top, ckCol(255, 192, 192, 192), 1, "%.2f FPS", ckTaskMgr::getCurFPS());

    top -= FONT_HEIGHT;

    r32 execute_rate = static_cast<r32>(static_cast<r64>(ckTaskMgr::getExecuteUsecTime()) * ckTaskMgr::getAimFPS() / 1000000.0);
    drawRect(left, top, execute_rate * bar_width, 2.0f, 0.0f, ckCol(255, 0, 0, 192));

    top -= FONT_HEIGHT * 0.5f;

    drawString(left, top, ckCol(192, 255, 192, 192), 1, "%d/%d byte", ckMemMgr::getCurUsedMemorySize(), ckMemMgr::getMaxUsedMemorySize());

    top -= FONT_HEIGHT;

    r32 memory_rate = static_cast<r32>(ckMemMgr::getCurUsedMemorySize()) / ckMemMgr::getMaxUsedMemorySize();
    drawRect(left, top, memory_rate * bar_width, 2.0f, 0.0f, ckCol(0, 255, 0, 192));

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

        ckCol col = (row + row_num > m_end_row) ? ckCol(255, 255, 255, 192) : ckCol(192, 192, 255, 192);

        drawString(left, top - FONT_HEIGHT * i, col, 1, m_console_buf[row % CONSOLE_BUFFER_SIZE].getString());
    }
}


void ckDbgMgr::drawRect(r32 left, r32 top, r32 width, r32 height, r32 z, ckCol col)
{
    r32 x1 = left;
    r32 y1 = top;
    r32 x2 = left + width;
    r32 y2 = top - height;

    ckVec vec1 = ckVec(x1, y1, z);
    ckVec vec2 = ckVec(x1, y2, z);
    ckVec vec3 = ckVec(x2, y1, z);
    ckVec vec4 = ckVec(x2, y2, z);

    ckDbgMgr::drawPolygon(vec1, vec2, vec3, vec4, col, DEBUG_MODE_SCREEN_ID);
}


void ckDbgMgr::drawFrame(r32 left, r32 top, r32 width, r32 height, r32 z, ckCol col)
{
    r32 x1 = left;
    r32 y1 = top;
    r32 x2 = left + width;
    r32 y2 = top - height;

    ckVec vec1 = ckVec(x1, y1, z);
    ckVec vec2 = ckVec(x1, y2, z);
    ckVec vec3 = ckVec(x2, y2, z);
    ckVec vec4 = ckVec(x2, y1, z);

    drawLine(vec1, vec2, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec2, vec3, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec3, vec4, col, DEBUG_MODE_SCREEN_ID);
    drawLine(vec4, vec1, col, DEBUG_MODE_SCREEN_ID);
}


s32 ckDbgMgr::calcConsoleRowNum()
{
    return static_cast<s32>(m_dbg_mode_scr->getViewHeight() / DEBUG_FONT_HEIGHT) - 5;
}


CK_DEFINE_MANAGER_INSTANCE(ckDbgMgr)
