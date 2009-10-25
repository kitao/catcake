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


#include "pogolyn.h"


class SoundMixer : public pgTask
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
    pgNewTask(SoundMixer);
}


SoundMixer::SoundMixer() : pgTask(ORDER_ZERO)
{
    m_is_button_down = false;
    m_track_no = 0;
    m_button_no = 0;
}


static bool checkClick(u8* trk_no, u8* btn_no)
{
    pgScr* scr = pgDrawMgr::getScreen(pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    r32 mouse_x = scr->framebufferXToScreenX(pgKeyMgr::getMouseX());
    r32 mouse_y = scr->framebufferYToScreenY(pgKeyMgr::getMouseY());

    *trk_no = static_cast<u8>((mouse_x + 300.0f) / 150.0f);
    mouse_x += -150.0f * *trk_no + 300.0f;

    for (s32 i = 0; i < 4; i++)
    {
        if (pgMath::abs(-35.0f * i + 130.0f - mouse_y) < 10.0f)
        {
            if (pgMath::abs(40.0f - mouse_x) < 20.0f)
            {
                *btn_no = i * 2;

                return true;
            }
            else if (pgMath::abs(105.0f - mouse_x) < 20.0f)
            {
                *btn_no = i * 2 + 1;

                return true;
            }
        }
    }

    r32 fader_y = 110.0f * pgSndMgr::getTrackVolume(*trk_no) / 255.0f - 130.0f;

    if (pgMath::abs(75.0f - mouse_x) < 30.0f && pgMath::abs(fader_y - mouse_y) < 7.5f)
    {
        *btn_no = 8;

        return true;
    }

    return false;
}


static void drawLine(const pgVec& pos1, const pgVec& pos2)
{
    pgVec offset1(pgMath::rand(-2.0f, 2.0f, 1.0f), pgMath::rand(-2.0f, 2.0f, 1.0f));
    pgVec offset2(pgMath::rand(-2.0f, 2.0f, 1.0f), pgMath::rand(-2.0f, 2.0f, 1.0f));

    pgDbgMgr::drawLine(pos1 + offset1, pos2 + offset2, pgCol::FULL, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
}


static void drawRect(const pgVec& center, r32 width, r32 height, bool is_active)
{
    width /= 2.0f;
    height /= 2.0f;

    pgVec pos1(center.x - width, center.y + height);
    pgVec pos2(center.x - width, center.y - height);
    pgVec pos3(center.x + width, center.y + height);
    pgVec pos4(center.x + width, center.y - height);

    pgCol col = is_active ? pgCol(255, 128, 64, 255) : pgCol(128, 128, 128, 96);
    pgDbgMgr::drawPolygon(pos1, pos2, pos3, pos4, col, pgDrawMgr::DEFAULT_2D_SCREEN_ID);

    drawLine(pos1, pos2);
    drawLine(pos2, pos4);
    drawLine(pos4, pos3);
    drawLine(pos3, pos1);
}


static void drawTriangle(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3, bool is_active)
{
    pgCol col = is_active ? pgCol(255, 128, 64, 255) : pgCol(128, 128, 128, 96);
    pgDbgMgr::drawPolygon(pos1, pos2, pos3, col, pgDrawMgr::DEFAULT_2D_SCREEN_ID);

    drawLine(pos1, pos2);
    drawLine(pos2, pos3);
    drawLine(pos3, pos1);
}


static void drawMixer(bool is_pushed, u8 trk_no, u8 btn_no)
{
    /*
        draw frame
    */
    drawRect(pgVec::ZERO, 600.0f, 320.0f, false);
    drawLine(pgVec(-150.0f, 160.0f), pgVec(-150.0f, -160.0f));
    drawLine(pgVec(0.0f, 160.0f), pgVec(0.0f, -160.0f));
    drawLine(pgVec(150.0f, 160.0f), pgVec(150.0f, -160.0f));

    btn_no = is_pushed ? trk_no * 9 + btn_no + 1 : 0;

    for (u32 i = 0; i < pgSndMgr::TRACK_NUM; i++)
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
                drawRect(pgVec(x - 32.0f + 64.0f * k, button_y), 40.0f, 20.0f, (btn_no == i * 9 + j * 2 + k + 1));
            }
        }

        drawTriangle(pgVec(x - 32.0f, 35.0f), pgVec(x - 52.0f, 15.0f), pgVec(x - 12.0f, 15.0f), (btn_no == i * 9 + 7));
        drawTriangle(pgVec(x + 12.0f, 35.0f), pgVec(x + 32.0f, 15.0f), pgVec(x + 52.0f, 35.0f), (btn_no == i * 9 + 8));

        /*
            draw fader
        */
        r32 y = 110.0f * pgSndMgr::getTrackVolume(i) / 255.0f - 130.0f;

        drawRect(pgVec(x, y), 60.0f, 15.0f, (btn_no == i * 9 + 9));

        if (y > -20.0f - 7.5f)
        {
            drawLine(pgVec(x, y - 7.5f), pgVec(x, -130.0f));
        }
        else if (y < -130.0f + 7.5f)
        {
            drawLine(pgVec(x, -20.0f), pgVec(x, y + 7.5f));
        }
        else
        {
            drawLine(pgVec(x, -20.0f), pgVec(x, y + 7.5f));
            drawLine(pgVec(x, y - 7.5f), pgVec(x, -130.0f));
        }
    }
}


void SoundMixer::onUpdate()
{
    if (pgKeyMgr::isOff(pgKeyMgr::KEY_LBUTTON))
    {
        m_is_button_down = false;
    }
    else if (pgKeyMgr::isPressed(pgKeyMgr::KEY_LBUTTON) && checkClick(&m_track_no, &m_button_no))
    {
        m_is_button_down = true;

        if (m_button_no < 6)
        {
            pgMsg<4> msg;
            msg.setParam(0, m_track_no);
            msg.setParam(1, m_button_no);

            pgTaskMgr::sendMessage(pgID_("PLAY"), msg);
        }
        else if (m_button_no < 8)
        {
            pgMsg<4> msg;
            msg.setParam(0, m_track_no);
            msg.setParam(1, static_cast<u8>((m_button_no == 6) ? 255 : 0));
            msg.setParam(2, 4.0f);

            pgTaskMgr::sendMessage(pgID_("FADE"), msg);
        }
    }

    if (m_is_button_down && m_button_no == 8)
    {
        r32 trk_vol = pgMath::clamp(255.0f * (370.0f - static_cast<r32>(pgKeyMgr::getMouseY())) / 110.0f, 0.0f, 255.0f);

        pgSndMgr::setTrackVolume(m_track_no, static_cast<u8>(trk_vol));
    }

    drawMixer(m_is_button_down, m_track_no, m_button_no);
}
