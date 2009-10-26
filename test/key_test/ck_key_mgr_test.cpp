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


#include "test.h"


static void assertKeyState( //
    bool is_a_on, bool is_a_off, bool is_a_pressed, bool is_a_released, //
    bool is_b_on, bool is_b_off, bool is_b_pressed, bool is_b_released, //
    bool is_c_on, bool is_c_off, bool is_c_pressed, bool is_c_released, //
    bool is_any_on, bool is_any_off, bool is_any_pressed, bool is_any_released)
{
    ckAssert(ckKeyMgr::isOn(ckKeyMgr::KEY_A) == is_a_on);
    ckAssert(ckKeyMgr::isOff(ckKeyMgr::KEY_A) == is_a_off);
    ckAssert(ckKeyMgr::isPressed(ckKeyMgr::KEY_A) == is_a_pressed);
    ckAssert(ckKeyMgr::isReleased(ckKeyMgr::KEY_A) == is_a_released);

    ckAssert(ckKeyMgr::isOn(ckKeyMgr::KEY_B) == is_b_on);
    ckAssert(ckKeyMgr::isOff(ckKeyMgr::KEY_B) == is_b_off);
    ckAssert(ckKeyMgr::isPressed(ckKeyMgr::KEY_B) == is_b_pressed);
    ckAssert(ckKeyMgr::isReleased(ckKeyMgr::KEY_B) == is_b_released);

    ckAssert(ckKeyMgr::isOn(ckKeyMgr::KEY_C) == is_c_on);
    ckAssert(ckKeyMgr::isOff(ckKeyMgr::KEY_C) == is_c_off);
    ckAssert(ckKeyMgr::isPressed(ckKeyMgr::KEY_C) == is_c_pressed);
    ckAssert(ckKeyMgr::isReleased(ckKeyMgr::KEY_C) == is_c_released);

    ckAssert(ckKeyMgr::isOn(ckKeyMgr::KEY_ANY) == is_any_on);
    ckAssert(ckKeyMgr::isOff(ckKeyMgr::KEY_ANY) == is_any_off);
    ckAssert(ckKeyMgr::isPressed(ckKeyMgr::KEY_ANY) == is_any_pressed);
    ckAssert(ckKeyMgr::isReleased(ckKeyMgr::KEY_ANY) == is_any_released);

    ckAssert(!ckKeyMgr::isOn(ckKeyMgr::KEY_NONE));
    ckAssert(ckKeyMgr::isOff(ckKeyMgr::KEY_NONE));
    ckAssert(!ckKeyMgr::isPressed(ckKeyMgr::KEY_NONE));
    ckAssert(!ckKeyMgr::isReleased(ckKeyMgr::KEY_NONE));
}


void ckKeyMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterTask()
        static void destroyBeforeSys()
    */
    {
        ckAssert(!ckKeyMgr::isCreated());

        ckKeyMgr::createAfterTask();
        ckKeyMgr::createAfterTask();

        ckAssert(ckKeyMgr::isCreated());

        ckKeyMgr::destroyBeforeSys();
        ckKeyMgr::destroyBeforeSys();

        ckAssert(!ckKeyMgr::isCreated());
    }

    /*
        static KeyEventHandler getKeyEventHandlerN()
        static void setKeyEventHandler(KeyEventHandler handler)
        static void defaultKeyEventHandler(KeyType key, KeyState key_state)
        static bool isOn(KEY key)
        static bool isOff(KEY key)
        static bool isPressed(KEY key)
        static bool isReleased(KEY key)
        static void updateKeyStateForSystem()
        static void resetKeyStateForSystem()
    */
    {
        ckAssertThrow(ckKeyMgr::getKeyEventHandlerN(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::setKeyEventHandler(NULL), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_NONE, ckKeyMgr::STATE_DOWN), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::isOn(ckKeyMgr::KEY_NONE), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::isOff(ckKeyMgr::KEY_NONE), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::isPressed(ckKeyMgr::KEY_NONE), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::isReleased(ckKeyMgr::KEY_NONE), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::updateKeyStateForSystem(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::resetKeyStateForSystem(), ckKeyMgr::ExceptionNotInitialized);

        ckKeyMgr::createAfterTask();

        ckAssert(ckKeyMgr::getKeyEventHandlerN() == ckKeyMgr::defaultKeyEventHandler);

        ckKeyMgr::setKeyEventHandler(NULL);
        ckAssert(!ckKeyMgr::getKeyEventHandlerN());

        ckTaskMgr::updateForSystem();

        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_A, ckKeyMgr::STATE_DOWN);
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        ckKeyMgr::updateKeyStateForSystem();
        assertKeyState(true, false, true, false, false, true, false, false, //
            false, true, false, false, true, false, true, false);

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_B, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_B, ckKeyMgr::STATE_UP);
        ckKeyMgr::updateKeyStateForSystem();
        assertKeyState(true, false, false, false, true, false, true, false, //
            false, true, false, false, true, false, false, false);

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_A, ckKeyMgr::STATE_UP);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_C, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::updateKeyStateForSystem();
        assertKeyState(false, true, false, true, false, true, false, true, //
            true, false, true, false, true, false, false, false);

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_C, ckKeyMgr::STATE_UP);
        ckKeyMgr::updateKeyStateForSystem();
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, true, false, true, false, true);

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_B, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::resetKeyStateForSystem();
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        ckKeyMgr::updateKeyStateForSystem();
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        ckKeyMgr::destroyBeforeSys();
    }

    /*
        static MouseEventHandler getMouseEventHandlerN()
        static void setMouseEventHandler(MouseEventHandler handler)
        static void defaultMouseEventHandler(u16 mouse_x, u16 mouse_y)
        static u16 getMouseX()
        static u16 getMouseY()
        static s16 getMouseWheel()
        static void setMousePos(u16 mouse_x, u16 mouse_y)
        static bool isMouseVisible()
        static void setMouseVisible(bool is_visible)
    */
    {
        ckAssertThrow(ckKeyMgr::getMouseEventHandlerN(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::setMouseEventHandler(NULL), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::defaultMouseEventHandler(0, 0), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::getMouseX(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::getMouseY(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::getMouseWheel(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::setMousePos(0, 0), ckKeyMgr::ExceptionNotInitialized);

        ckKeyMgr::createAfterTask();

        ckAssert(ckKeyMgr::getMouseEventHandlerN() == ckKeyMgr::defaultMouseEventHandler);
        ckAssert(ckKeyMgr::isMouseVisible());

        ckKeyMgr::setKeyEventHandler(NULL);

        ckKeyMgr::setMouseEventHandler(NULL);
        ckAssert(!ckKeyMgr::getMouseEventHandlerN());

        ckTaskMgr::updateForSystem();

        ckAssert(ckKeyMgr::getMouseX() == 0 && ckKeyMgr::getMouseY() == 0);
        ckAssert(ckKeyMgr::getMouseWheel() == 0);

        ckKeyMgr::defaultMouseEventHandler(12, 34);
        ckAssert(ckKeyMgr::getMouseX() == 12 && ckKeyMgr::getMouseY() == 34);

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELUP, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELUP, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELUP, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELDOWN, ckKeyMgr::STATE_DOWN);
        ckAssert(ckKeyMgr::getMouseWheel() == 0);

        ckKeyMgr::updateKeyStateForSystem();
        ckAssert(ckKeyMgr::getMouseWheel() == 2);
        ckAssert(ckKeyMgr::isOn(ckKeyMgr::KEY_WHEELUP) && ckKeyMgr::isOn(ckKeyMgr::KEY_WHEELDOWN));

        ckKeyMgr::updateKeyStateForSystem();
        ckAssert(ckKeyMgr::getMouseWheel() == 0);
        ckAssert(ckKeyMgr::isOff(ckKeyMgr::KEY_WHEELUP) && ckKeyMgr::isOff(ckKeyMgr::KEY_WHEELDOWN));

        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELUP, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELDOWN, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELDOWN, ckKeyMgr::STATE_DOWN);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELDOWN, ckKeyMgr::STATE_UP);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELDOWN, ckKeyMgr::STATE_UP);
        ckKeyMgr::defaultKeyEventHandler(ckKeyMgr::KEY_WHEELUP, ckKeyMgr::STATE_UP);
        ckAssert(ckKeyMgr::getMouseWheel() == 0);

        ckKeyMgr::updateKeyStateForSystem();
        ckAssert(ckKeyMgr::getMouseWheel() == -1);
        ckAssert(ckKeyMgr::isOn(ckKeyMgr::KEY_WHEELUP) && ckKeyMgr::isOn(ckKeyMgr::KEY_WHEELDOWN));

        ckKeyMgr::resetKeyStateForSystem();
        ckAssert(ckKeyMgr::getMouseX() == 12 && ckKeyMgr::getMouseY() == 34);
        ckAssert(ckKeyMgr::getMouseWheel() == 0);

        ckKeyMgr::updateKeyStateForSystem();
        ckAssert(ckKeyMgr::getMouseX() == 12 && ckKeyMgr::getMouseY() == 34);
        ckAssert(ckKeyMgr::getMouseWheel() == 0);

        ckKeyMgr::setMousePos(23, 45);
        ckAssert(ckKeyMgr::getMouseX() == 23 && ckKeyMgr::getMouseY() == 45);

        ckKeyMgr::updateKeyStateForSystem();
        ckAssert(ckKeyMgr::getMouseX() == 23 && ckKeyMgr::getMouseY() == 45);

        ckKeyMgr::setMouseVisible(false);
        ckAssert(!ckKeyMgr::isMouseVisible());

        ckKeyMgr::setMouseVisible(true);
        ckAssert(ckKeyMgr::isMouseVisible());

        ckKeyMgr::destroyBeforeSys();
    }

    /*
        static const u32 EXTRA_VALUE_NUM

        static ExtraEventHandler getExtraEventHandlerN()
        static void setExtraEventHandler(ExtraEventHandler handler)
        static void defaultExtraEventHandler(u8 val_index, r32 value)
        static s32 getExtraValue_s32(u8 val_index)
        static r32 getExtraValue_r32(u8 val_index)
        static void updateExtraValueForSystem()
    */
    {
        ckAssert(ckKeyMgr::EXTRA_VALUE_NUM == 16);

        ckAssertThrow(ckKeyMgr::getExtraEventHandlerN(), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::setExtraEventHandler(NULL), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::getExtraValue_s32(0), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::getExtraValue_r32(0), ckKeyMgr::ExceptionNotInitialized);
        ckAssertThrow(ckKeyMgr::updateExtraValueForSystem(), ckKeyMgr::ExceptionNotInitialized);

        ckKeyMgr::createAfterTask();

        ckAssert(ckKeyMgr::getExtraEventHandlerN() == ckKeyMgr::defaultExtraEventHandler);

        for (u32 i = 0; i < ckKeyMgr::EXTRA_VALUE_NUM; i++)
        {
            ckAssert(ckKeyMgr::getExtraValue_s32(i) == 0);
            ckAssert(isEqual(ckKeyMgr::getExtraValue_r32(i), 0.0f));
        }

        ckKeyMgr::setExtraEventHandler(NULL);
        ckAssert(!ckKeyMgr::getExtraEventHandlerN());

        ckTaskMgr::updateForSystem();

        for (u32 i = 0; i < ckKeyMgr::EXTRA_VALUE_NUM; i++)
        {
            ckAssert(ckKeyMgr::getExtraValue_s32(i) == 0);
            ckAssert(isEqual(ckKeyMgr::getExtraValue_r32(i), 0.0f));
        }

        ckKeyMgr::defaultExtraEventHandler(0, 123.0f);
        ckAssert(ckKeyMgr::getExtraValue_s32(0) == 0);
        ckAssert(isEqual(ckKeyMgr::getExtraValue_r32(0), 0.0f));

        ckKeyMgr::defaultExtraEventHandler(ckKeyMgr::EXTRA_VALUE_NUM + 100, 123.0f);

        ckKeyMgr::updateExtraValueForSystem();
        ckAssert(ckKeyMgr::getExtraValue_s32(0) == 123);
        ckAssert(isEqual(ckKeyMgr::getExtraValue_r32(0), 123.0f));

        ckAssertThrow(ckKeyMgr::getExtraValue_s32(ckKeyMgr::EXTRA_VALUE_NUM), ckKeyMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckKeyMgr::getExtraValue_r32(ckKeyMgr::EXTRA_VALUE_NUM), ckKeyMgr::ExceptionInvalidArgument);

        ckKeyMgr::destroyBeforeSys();
    }
}
