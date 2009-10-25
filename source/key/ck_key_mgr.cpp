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


#include "pg_key_all.h"

#include "pg_task_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgKeyMgr* pgKeyMgr::m_instance = NULL;


PG_DEFINE_MANAGER_IS_CREATED(pgKeyMgr)


PG_DEFINE_MANAGER_CREATE(pgKeyMgr, AfterTask, BeforeSys)


PG_DEFINE_MANAGER_DESTROY(pgKeyMgr, BeforeSys)


pgKeyMgr::KeyEventHandler pgKeyMgr::getKeyEventHandlerN()
{
    return instance()->m_key_event_handler;
}


void pgKeyMgr::setKeyEventHandler(KeyEventHandler handler)
{
    instance()->m_key_event_handler = handler;
}


void pgKeyMgr::defaultKeyEventHandler(KeyType key, KeyState key_state)
{
    pgKeyMgr* ins = instance();

    if (key < KEY_ANY)
    {
        if (key_state == STATE_DOWN)
        {
            ins->m_key_flag[key] |= FLAG_REAL_ON | FLAG_DELAY_ON;

            if (key == KEY_WHEELUP)
            {
                ins->m_real_mouse_wheel++;
            }
            else if (key == KEY_WHEELDOWN)
            {
                ins->m_real_mouse_wheel--;
            }
        }
        else // STATE_UP
        {
            ins->m_key_flag[key] &= ~FLAG_REAL_ON;
        }
    }
}


pgKeyMgr::MouseEventHandler pgKeyMgr::getMouseEventHandlerN()
{
    return instance()->m_mouse_event_handler;
}


void pgKeyMgr::setMouseEventHandler(MouseEventHandler handler)
{
    instance()->m_mouse_event_handler = handler;
}


void pgKeyMgr::defaultMouseEventHandler(s16 mouse_x, s16 mouse_y)
{
    pgKeyMgr* ins = pgKeyMgr::instance();

    ins->m_mouse_x = mouse_x;
    ins->m_mouse_y = mouse_y;
}


pgKeyMgr::ExtraEventHandler pgKeyMgr::getExtraEventHandlerN()
{
    return instance()->m_extra_event_handler;
}


void pgKeyMgr::setExtraEventHandler(ExtraEventHandler handler)
{
    instance()->m_extra_event_handler = handler;
}


void pgKeyMgr::defaultExtraEventHandler(u8 val_index, r32 value)
{
    pgKeyMgr* ins = pgKeyMgr::instance();

    if (val_index < EXTRA_VALUE_NUM)
    {
        ins->m_real_ext_val[val_index] = value;
    }
}


bool pgKeyMgr::isOn(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return (key_flag & FLAG_CUR_ON) ? true : false;
}


bool pgKeyMgr::isOff(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return (key_flag & FLAG_CUR_ON) ? false : true;
}


bool pgKeyMgr::isPressed(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return ((key_flag & FLAG_CUR_ON) && !(key_flag & FLAG_LAST_ON));
}


bool pgKeyMgr::isReleased(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return (!(key_flag & FLAG_CUR_ON) && (key_flag & FLAG_LAST_ON));
}


s16 pgKeyMgr::getMouseX()
{
    return instance()->m_mouse_x;
}


s16 pgKeyMgr::getMouseY()
{
    return instance()->m_mouse_y;
}


s16 pgKeyMgr::getMouseWheel()
{
    return instance()->m_cur_mouse_wheel;
}


void pgKeyMgr::setMousePos(s16 mouse_x, s16 mouse_y)
{
    pgKeyMgr* ins = pgKeyMgr::instance();

    ins->m_mouse_x = mouse_x;
    ins->m_mouse_y = mouse_y;

    pgLowLevelAPI::setMousePos(mouse_x, mouse_y);
}


bool pgKeyMgr::isMouseVisible()
{
    return pgLowLevelAPI::isMouseVisible();
}


void pgKeyMgr::setMouseVisible(bool is_visible)
{
    return pgLowLevelAPI::setMouseVisible(is_visible);
}


s32 pgKeyMgr::getExtraValue_s32(u8 val_index)
{
    return static_cast<s32>(getExtraValue_r32(val_index));
}


r32 pgKeyMgr::getExtraValue_r32(u8 val_index)
{
    pgKeyMgr* ins = instance();

    if (val_index >= EXTRA_VALUE_NUM)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_cur_ext_val[val_index];
}


void pgKeyMgr::updateKeyStateForSystem()
{
    pgKeyMgr* ins = instance();
    u8* key_flag = ins->m_key_flag;

    key_flag[KEY_WHEELUP] &= ~FLAG_REAL_ON;
    key_flag[KEY_WHEELDOWN] &= ~FLAG_REAL_ON;

    u8* flag;
    bool is_any_key_on = false;

    for (u32 i = 0; i < KEY_ANY; i++)
    {
        flag = &key_flag[i];

        if (*flag & FLAG_CUR_ON)
        {
            *flag |= FLAG_LAST_ON;
        }
        else
        {
            *flag &= ~FLAG_LAST_ON;
        }

        if (*flag & (FLAG_DELAY_ON | FLAG_REAL_ON))
        {
            *flag |= FLAG_CUR_ON;
            is_any_key_on = true;
        }
        else
        {
            *flag &= ~FLAG_CUR_ON;
        }

        *flag &= ~FLAG_DELAY_ON;
    }

    flag = &key_flag[KEY_ANY];

    if (*flag & FLAG_CUR_ON)
    {
        *flag |= FLAG_LAST_ON;
    }
    else
    {
        *flag &= ~FLAG_LAST_ON;
    }

    if (is_any_key_on)
    {
        *flag |= FLAG_CUR_ON;
    }
    else
    {
        *flag &= ~FLAG_CUR_ON;
    }

    ins->m_cur_mouse_wheel = ins->m_real_mouse_wheel;
    ins->m_real_mouse_wheel = 0;
}


void pgKeyMgr::resetKeyStateForSystem()
{
    pgKeyMgr* ins = instance();

    pgMemMgr::memset(ins->m_key_flag, 0, sizeof(ins->m_key_flag));

    ins->m_real_mouse_wheel = ins->m_cur_mouse_wheel = 0;
}


void pgKeyMgr::updateExtraValueForSystem()
{
    pgKeyMgr* ins = instance();

    pgMemMgr::memcpy(ins->m_cur_ext_val, ins->m_real_ext_val, sizeof(ins->m_cur_ext_val));
}


static void lowLevelKeyEventHandler(u8 keycode, bool is_down)
{
    pgKeyMgr::KeyEventHandler handler = pgKeyMgr::getKeyEventHandlerN();

    if (handler)
    {
        (*handler)(static_cast<pgKeyMgr::KeyType>(keycode), is_down ? pgKeyMgr::STATE_DOWN : pgKeyMgr::STATE_UP);
    }
}


static void lowLevelMouseEventHandler(s16 mouse_x, s16 mouse_y)
{
    pgKeyMgr::MouseEventHandler handler = pgKeyMgr::getMouseEventHandlerN();

    if (handler)
    {
        (*handler)(mouse_x, mouse_y);
    }
}


static void lowLevelExtraEventHandler(u8 index, r32 value)
{
    pgKeyMgr::ExtraEventHandler handler = pgKeyMgr::getExtraEventHandlerN();

    if (handler)
    {
        (*handler)(index, value);
    }
}


class pgKeyDaemon : public pgTask
{
public:
    pgKeyDaemon() : pgTask(ORDER_MINUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        pgKeyMgr::updateKeyStateForSystem();
        pgKeyMgr::updateExtraValueForSystem();
    }
};


pgKeyMgr::pgKeyMgr()
{
    pgLowLevelAPI::setKeyEventHandler(lowLevelKeyEventHandler);
    pgLowLevelAPI::setMouseEventHandler(lowLevelMouseEventHandler);
    pgLowLevelAPI::setExtraEventHandler(lowLevelExtraEventHandler);

    m_key_event_handler = defaultKeyEventHandler;
    m_mouse_event_handler = defaultMouseEventHandler;
    m_extra_event_handler = defaultExtraEventHandler;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_real_mouse_wheel = m_cur_mouse_wheel = 0;

    pgMemMgr::memset(m_key_flag, 0, sizeof(m_key_flag));
    pgMemMgr::memset(m_cur_ext_val, 0, sizeof(m_cur_ext_val));
    pgMemMgr::memset(m_real_ext_val, 0, sizeof(m_real_ext_val));

    setMouseVisible(true);

    m_key_daemon = pgNewTask(pgKeyDaemon);
}


pgKeyMgr::~pgKeyMgr()
{
    if (pgTaskMgr::isCreated())
    {
        pgDeleteTask(m_key_daemon);
    }

    pgLowLevelAPI::setKeyEventHandler(NULL);
    pgLowLevelAPI::setMouseEventHandler(NULL);
}


PG_DEFINE_OPERATOR_EQUAL(pgKeyMgr)


PG_DEFINE_MANAGER_INSTANCE(pgKeyMgr)
