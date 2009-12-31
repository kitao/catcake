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


#include "catcake.h"


class SoundMixer : public ckTask
{
public:
    SoundMixer();

private:
    virtual void onUpdate();

    bool m_is_button_down;
    u8 m_track_no;
    u8 m_button_no;
};


void newSoundMixer()
{
    ckNewTask(SoundMixer);
}


SoundMixer::SoundMixer() : ckTask(ORDER_ZERO)
{
    m_is_button_down = false;
    m_track_no = 0;
    m_button_no = 0;
}


static bool checkClick(u8* trk_no, u8* btn_no)
{
    ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_2D_SCREEN_ID);
    r32 mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
    r32 mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());

    *trk_no = static_cast<u8>((mouse_x + 300.0f) / 150.0f);
    mouse_x += -150.0f * *trk_no + 300.0f;

    for (s32 i = 0; i < 4; i++)
    {
        if (ckMath::abs(-35.0f * i + 130.0f - mouse_y) < 10.0f)
        {
            if (ckMath::abs(40.0f - mouse_x) < 20.0f)
            {
                *btn_no = i * 2;

                return true;
            }
            else if (ckMath::abs(105.0f - mouse_x) < 20.0f)
            {
                *btn_no = i * 2 + 1;

                return true;
            }
        }
    }

    r32 fader_y = 110.0f * ckSndMgr::getTrackVolume(*trk_no) / 255.0f - 130.0f;

    if (ckMath::abs(75.0f - mouse_x) < 30.0f && ckMath::abs(fader_y - mouse_y) < 7.5f)
    {
        *btn_no = 8;

        return true;
    }

    return false;
}


static void drawLine(const ckVec& pos1, const ckVec& pos2)
{
    ckVec offset1(ckMath::rand(-2.0f, 2.0f, 1.0f), ckMath::rand(-2.0f, 2.0f, 1.0f));
    ckVec offset2(ckMath::rand(-2.0f, 2.0f, 1.0f), ckMath::rand(-2.0f, 2.0f, 1.0f));

    ckDbgMgr::drawLine(pos1 + offset1, pos2 + offset2, ckCol::FULL, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
}


static void drawRect(const ckVec& center, r32 width, r32 height, bool is_active)
{
    width /= 2.0f;
    height /= 2.0f;

    ckVec pos1(center.x - width, center.y + height);
    ckVec pos2(center.x - width, center.y - height);
    ckVec pos3(center.x + width, center.y + height);
    ckVec pos4(center.x + width, center.y - height);

    ckCol col = is_active ? ckCol(255, 128, 64, 255) : ckCol(128, 128, 128, 96);
    ckDbgMgr::drawPolygon(pos1, pos2, pos3, pos4, col, ckDrawMgr::DEFAULT_2D_SCREEN_ID);

    drawLine(pos1, pos2);
    drawLine(pos2, pos4);
    drawLine(pos4, pos3);
    drawLine(pos3, pos1);
}


static void drawTriangle(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, bool is_active)
{
    ckCol col = is_active ? ckCol(255, 128, 64, 255) : ckCol(128, 128, 128, 96);
    ckDbgMgr::drawPolygon(pos1, pos2, pos3, col, ckDrawMgr::DEFAULT_2D_SCREEN_ID);

    drawLine(pos1, pos2);
    drawLine(pos2, pos3);
    drawLine(pos3, pos1);
}


static void drawMixer(bool is_pushed, u8 trk_no, u8 btn_no)
{
    /*
        draw frame
    */
    drawRect(ckVec::ZERO, 600.0f, 320.0f, false);
    drawLine(ckVec(-150.0f, 160.0f), ckVec(-150.0f, -160.0f));
    drawLine(ckVec(0.0f, 160.0f), ckVec(0.0f, -160.0f));
    drawLine(ckVec(150.0f, 160.0f), ckVec(150.0f, -160.0f));

    btn_no = is_pushed ? trk_no * 9 + btn_no + 1 : 0;

    for (u32 i = 0; i < ckSndMgr::TRACK_NUM; i++)
    {
        /*
            draw buttons
        */
        r32 x = 150.0f * i - 225.0f;

        for (s32 j = 0; j < 3; j++)
        {
            r32 button_y = -35.0f * j + 130.0f;

            for (s32 k = 0; k < 2; k++)
            {
                drawRect(ckVec(x - 32.0f + 64.0f * k, button_y), 40.0f, 20.0f, (btn_no == i * 9 + j * 2 + k + 1));
            }
        }

        drawTriangle(ckVec(x - 32.0f, 35.0f), ckVec(x - 52.0f, 15.0f), ckVec(x - 12.0f, 15.0f), (btn_no == i * 9 + 7));
        drawTriangle(ckVec(x + 12.0f, 35.0f), ckVec(x + 32.0f, 15.0f), ckVec(x + 52.0f, 35.0f), (btn_no == i * 9 + 8));

        /*
            draw fader
        */
        r32 y = 110.0f * ckSndMgr::getTrackVolume(i) / 255.0f - 130.0f;

        drawRect(ckVec(x, y), 60.0f, 15.0f, (btn_no == i * 9 + 9));

        if (y > -20.0f - 7.5f)
        {
            drawLine(ckVec(x, y - 7.5f), ckVec(x, -130.0f));
        }
        else if (y < -130.0f + 7.5f)
        {
            drawLine(ckVec(x, -20.0f), ckVec(x, y + 7.5f));
        }
        else
        {
            drawLine(ckVec(x, -20.0f), ckVec(x, y + 7.5f));
            drawLine(ckVec(x, y - 7.5f), ckVec(x, -130.0f));
        }
    }
}


void SoundMixer::onUpdate()
{
    if (ckKeyMgr::isOff(ckKeyMgr::KEY_LBUTTON))
    {
        m_is_button_down = false;
    }
    else if (ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON) && checkClick(&m_track_no, &m_button_no))
    {
        m_is_button_down = true;

        if (m_button_no < 6)
        {
            ckMsg<4> msg;
            msg.setParam(0, m_track_no);
            msg.setParam(1, m_button_no);

            ckTaskMgr::sendMessage(ckID_("PLAY"), msg);
        }
        else if (m_button_no < 8)
        {
            ckMsg<4> msg;
            msg.setParam(0, m_track_no);
            msg.setParam(1, static_cast<u8>((m_button_no == 6) ? 255 : 0));
            msg.setParam(2, 4.0f);

            ckTaskMgr::sendMessage(ckID_("FADE"), msg);
        }
    }

    if (m_is_button_down && m_button_no == 8)
    {
        r32 trk_vol = ckMath::clamp(255.0f * (370.0f - static_cast<r32>(ckKeyMgr::getMouseY())) / 110.0f, 0.0f, 255.0f);

        ckSndMgr::setTrackVolume(m_track_no, static_cast<u8>(trk_vol));
    }

    drawMixer(m_is_button_down, m_track_no, m_button_no);
}
