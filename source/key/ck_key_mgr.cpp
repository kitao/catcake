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


#include "ck_key_all.h"

#include "ck_task_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckKeyMgr* ckKeyMgr::m_instance = NULL;


CK_DEFINE_MANAGER_IS_CREATED(ckKeyMgr)


CK_DEFINE_MANAGER_CREATE(ckKeyMgr, AfterTask, BeforeSys)


CK_DEFINE_MANAGER_DESTROY(ckKeyMgr, BeforeSys)


ckKeyMgr::KeyEventHandler ckKeyMgr::getKeyEventHandlerN()
{
    return instance()->m_key_event_handler;
}


void ckKeyMgr::setKeyEventHandler(KeyEventHandler handler)
{
    instance()->m_key_event_handler = handler;
}


void ckKeyMgr::defaultKeyEventHandler(KeyType key, KeyState key_state)
{
    ckKeyMgr* ins = instance();

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


ckKeyMgr::MouseEventHandler ckKeyMgr::getMouseEventHandlerN()
{
    return instance()->m_mouse_event_handler;
}


void ckKeyMgr::setMouseEventHandler(MouseEventHandler handler)
{
    instance()->m_mouse_event_handler = handler;
}


void ckKeyMgr::defaultMouseEventHandler(s16 mouse_x, s16 mouse_y)
{
    ckKeyMgr* ins = ckKeyMgr::instance();

    ins->m_mouse_x = mouse_x;
    ins->m_mouse_y = mouse_y;
}


ckKeyMgr::ExtraEventHandler ckKeyMgr::getExtraEventHandlerN()
{
    return instance()->m_extra_event_handler;
}


void ckKeyMgr::setExtraEventHandler(ExtraEventHandler handler)
{
    instance()->m_extra_event_handler = handler;
}


void ckKeyMgr::defaultExtraEventHandler(u8 val_index, r32 value)
{
    ckKeyMgr* ins = ckKeyMgr::instance();

    if (val_index < EXTRA_VALUE_NUM)
    {
        ins->m_real_ext_val[val_index] = value;
    }
}


bool ckKeyMgr::isOn(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return (key_flag & FLAG_CUR_ON) ? true : false;
}


bool ckKeyMgr::isOff(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return (key_flag & FLAG_CUR_ON) ? false : true;
}


bool ckKeyMgr::isPressed(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return ((key_flag & FLAG_CUR_ON) && !(key_flag & FLAG_LAST_ON));
}


bool ckKeyMgr::isReleased(KeyType key)
{
    u8 key_flag = instance()->m_key_flag[key];

    return (!(key_flag & FLAG_CUR_ON) && (key_flag & FLAG_LAST_ON));
}


s16 ckKeyMgr::getMouseX()
{
    return instance()->m_mouse_x;
}


s16 ckKeyMgr::getMouseY()
{
    return instance()->m_mouse_y;
}


s16 ckKeyMgr::getMouseWheel()
{
    return instance()->m_cur_mouse_wheel;
}


void ckKeyMgr::setMousePos(s16 mouse_x, s16 mouse_y)
{
    ckKeyMgr* ins = ckKeyMgr::instance();

    ins->m_mouse_x = mouse_x;
    ins->m_mouse_y = mouse_y;

    ckLowLevelAPI::setMousePos(mouse_x, mouse_y);
}


bool ckKeyMgr::isMouseVisible()
{
    return ckLowLevelAPI::isMouseVisible();
}


void ckKeyMgr::setMouseVisible(bool is_visible)
{
    return ckLowLevelAPI::setMouseVisible(is_visible);
}


s32 ckKeyMgr::getExtraValue_s32(u8 val_index)
{
    return static_cast<s32>(getExtraValue_r32(val_index));
}


r32 ckKeyMgr::getExtraValue_r32(u8 val_index)
{
    ckKeyMgr* ins = instance();

    if (val_index >= EXTRA_VALUE_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_cur_ext_val[val_index];
}


void ckKeyMgr::updateKeyStateForSystem()
{
    ckKeyMgr* ins = instance();
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


void ckKeyMgr::resetKeyStateForSystem()
{
    ckKeyMgr* ins = instance();

    ckMemMgr::memset(ins->m_key_flag, 0, sizeof(ins->m_key_flag));

    ins->m_real_mouse_wheel = ins->m_cur_mouse_wheel = 0;
}


void ckKeyMgr::updateExtraValueForSystem()
{
    ckKeyMgr* ins = instance();

    ckMemMgr::memcpy(ins->m_cur_ext_val, ins->m_real_ext_val, sizeof(ins->m_cur_ext_val));
}


static void lowLevelKeyEventHandler(u8 keycode, bool is_down)
{
    ckKeyMgr::KeyEventHandler handler = ckKeyMgr::getKeyEventHandlerN();

    if (handler)
    {
        (*handler)(static_cast<ckKeyMgr::KeyType>(keycode), is_down ? ckKeyMgr::STATE_DOWN : ckKeyMgr::STATE_UP);
    }
}


static void lowLevelMouseEventHandler(s16 mouse_x, s16 mouse_y)
{
    ckKeyMgr::MouseEventHandler handler = ckKeyMgr::getMouseEventHandlerN();

    if (handler)
    {
        (*handler)(mouse_x, mouse_y);
    }
}


static void lowLevelExtraEventHandler(u8 index, r32 value)
{
    ckKeyMgr::ExtraEventHandler handler = ckKeyMgr::getExtraEventHandlerN();

    if (handler)
    {
        (*handler)(index, value);
    }
}


class ckKeyDaemon : public ckTask
{
public:
    ckKeyDaemon() : ckTask(ORDER_MINUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        ckKeyMgr::updateKeyStateForSystem();
        ckKeyMgr::updateExtraValueForSystem();
    }
};


ckKeyMgr::ckKeyMgr()
{
    ckLowLevelAPI::setKeyEventHandler(lowLevelKeyEventHandler);
    ckLowLevelAPI::setMouseEventHandler(lowLevelMouseEventHandler);
    ckLowLevelAPI::setExtraEventHandler(lowLevelExtraEventHandler);

    m_key_event_handler = defaultKeyEventHandler;
    m_mouse_event_handler = defaultMouseEventHandler;
    m_extra_event_handler = defaultExtraEventHandler;
    m_mouse_x = 0;
    m_mouse_y = 0;
    m_real_mouse_wheel = m_cur_mouse_wheel = 0;

    ckMemMgr::memset(m_key_flag, 0, sizeof(m_key_flag));
    ckMemMgr::memset(m_cur_ext_val, 0, sizeof(m_cur_ext_val));
    ckMemMgr::memset(m_real_ext_val, 0, sizeof(m_real_ext_val));

    setMouseVisible(true);

    m_key_daemon = ckNewTask(ckKeyDaemon);
}


ckKeyMgr::~ckKeyMgr()
{
    if (ckTaskMgr::isCreated())
    {
        ckDeleteTask(m_key_daemon);
    }

    ckLowLevelAPI::setKeyEventHandler(NULL);
    ckLowLevelAPI::setMouseEventHandler(NULL);
}


CK_DEFINE_OPERATOR_EQUAL(ckKeyMgr)


CK_DEFINE_MANAGER_INSTANCE(ckKeyMgr)
