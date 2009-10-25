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
    pgAssert(pgKeyMgr::isOn(pgKeyMgr::KEY_A) == is_a_on);
    pgAssert(pgKeyMgr::isOff(pgKeyMgr::KEY_A) == is_a_off);
    pgAssert(pgKeyMgr::isPressed(pgKeyMgr::KEY_A) == is_a_pressed);
    pgAssert(pgKeyMgr::isReleased(pgKeyMgr::KEY_A) == is_a_released);

    pgAssert(pgKeyMgr::isOn(pgKeyMgr::KEY_B) == is_b_on);
    pgAssert(pgKeyMgr::isOff(pgKeyMgr::KEY_B) == is_b_off);
    pgAssert(pgKeyMgr::isPressed(pgKeyMgr::KEY_B) == is_b_pressed);
    pgAssert(pgKeyMgr::isReleased(pgKeyMgr::KEY_B) == is_b_released);

    pgAssert(pgKeyMgr::isOn(pgKeyMgr::KEY_C) == is_c_on);
    pgAssert(pgKeyMgr::isOff(pgKeyMgr::KEY_C) == is_c_off);
    pgAssert(pgKeyMgr::isPressed(pgKeyMgr::KEY_C) == is_c_pressed);
    pgAssert(pgKeyMgr::isReleased(pgKeyMgr::KEY_C) == is_c_released);

    pgAssert(pgKeyMgr::isOn(pgKeyMgr::KEY_ANY) == is_any_on);
    pgAssert(pgKeyMgr::isOff(pgKeyMgr::KEY_ANY) == is_any_off);
    pgAssert(pgKeyMgr::isPressed(pgKeyMgr::KEY_ANY) == is_any_pressed);
    pgAssert(pgKeyMgr::isReleased(pgKeyMgr::KEY_ANY) == is_any_released);

    pgAssert(!pgKeyMgr::isOn(pgKeyMgr::KEY_NONE));
    pgAssert(pgKeyMgr::isOff(pgKeyMgr::KEY_NONE));
    pgAssert(!pgKeyMgr::isPressed(pgKeyMgr::KEY_NONE));
    pgAssert(!pgKeyMgr::isReleased(pgKeyMgr::KEY_NONE));
}


void pgKeyMgrTest()
{
    /*
        static bool isCreated()
        static void createAfterTask()
        static void destroyBeforeSys()
    */
    {
        pgAssert(!pgKeyMgr::isCreated());

        pgKeyMgr::createAfterTask();
        pgKeyMgr::createAfterTask();

        pgAssert(pgKeyMgr::isCreated());

        pgKeyMgr::destroyBeforeSys();
        pgKeyMgr::destroyBeforeSys();

        pgAssert(!pgKeyMgr::isCreated());
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
        pgAssertThrow(pgKeyMgr::getKeyEventHandlerN(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::setKeyEventHandler(NULL), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_NONE, pgKeyMgr::STATE_DOWN), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::isOn(pgKeyMgr::KEY_NONE), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::isOff(pgKeyMgr::KEY_NONE), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::isPressed(pgKeyMgr::KEY_NONE), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::isReleased(pgKeyMgr::KEY_NONE), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::updateKeyStateForSystem(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::resetKeyStateForSystem(), pgKeyMgr::ExceptionNotInitialized);

        pgKeyMgr::createAfterTask();

        pgAssert(pgKeyMgr::getKeyEventHandlerN() == pgKeyMgr::defaultKeyEventHandler);

        pgKeyMgr::setKeyEventHandler(NULL);
        pgAssert(!pgKeyMgr::getKeyEventHandlerN());

        pgTaskMgr::updateForSystem();

        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_A, pgKeyMgr::STATE_DOWN);
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        pgKeyMgr::updateKeyStateForSystem();
        assertKeyState(true, false, true, false, false, true, false, false, //
            false, true, false, false, true, false, true, false);

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_B, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_B, pgKeyMgr::STATE_UP);
        pgKeyMgr::updateKeyStateForSystem();
        assertKeyState(true, false, false, false, true, false, true, false, //
            false, true, false, false, true, false, false, false);

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_A, pgKeyMgr::STATE_UP);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_C, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::updateKeyStateForSystem();
        assertKeyState(false, true, false, true, false, true, false, true, //
            true, false, true, false, true, false, false, false);

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_C, pgKeyMgr::STATE_UP);
        pgKeyMgr::updateKeyStateForSystem();
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, true, false, true, false, true);

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_B, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::resetKeyStateForSystem();
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        pgKeyMgr::updateKeyStateForSystem();
        assertKeyState(false, true, false, false, false, true, false, false, //
            false, true, false, false, false, true, false, false);

        pgKeyMgr::destroyBeforeSys();
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
        pgAssertThrow(pgKeyMgr::getMouseEventHandlerN(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::setMouseEventHandler(NULL), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::defaultMouseEventHandler(0, 0), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::getMouseX(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::getMouseY(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::getMouseWheel(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::setMousePos(0, 0), pgKeyMgr::ExceptionNotInitialized);

        pgKeyMgr::createAfterTask();

        pgAssert(pgKeyMgr::getMouseEventHandlerN() == pgKeyMgr::defaultMouseEventHandler);
        pgAssert(pgKeyMgr::isMouseVisible());

        pgKeyMgr::setKeyEventHandler(NULL);

        pgKeyMgr::setMouseEventHandler(NULL);
        pgAssert(!pgKeyMgr::getMouseEventHandlerN());

        pgTaskMgr::updateForSystem();

        pgAssert(pgKeyMgr::getMouseX() == 0 && pgKeyMgr::getMouseY() == 0);
        pgAssert(pgKeyMgr::getMouseWheel() == 0);

        pgKeyMgr::defaultMouseEventHandler(12, 34);
        pgAssert(pgKeyMgr::getMouseX() == 12 && pgKeyMgr::getMouseY() == 34);

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELUP, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELUP, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELUP, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELDOWN, pgKeyMgr::STATE_DOWN);
        pgAssert(pgKeyMgr::getMouseWheel() == 0);

        pgKeyMgr::updateKeyStateForSystem();
        pgAssert(pgKeyMgr::getMouseWheel() == 2);
        pgAssert(pgKeyMgr::isOn(pgKeyMgr::KEY_WHEELUP) && pgKeyMgr::isOn(pgKeyMgr::KEY_WHEELDOWN));

        pgKeyMgr::updateKeyStateForSystem();
        pgAssert(pgKeyMgr::getMouseWheel() == 0);
        pgAssert(pgKeyMgr::isOff(pgKeyMgr::KEY_WHEELUP) && pgKeyMgr::isOff(pgKeyMgr::KEY_WHEELDOWN));

        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELUP, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELDOWN, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELDOWN, pgKeyMgr::STATE_DOWN);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELDOWN, pgKeyMgr::STATE_UP);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELDOWN, pgKeyMgr::STATE_UP);
        pgKeyMgr::defaultKeyEventHandler(pgKeyMgr::KEY_WHEELUP, pgKeyMgr::STATE_UP);
        pgAssert(pgKeyMgr::getMouseWheel() == 0);

        pgKeyMgr::updateKeyStateForSystem();
        pgAssert(pgKeyMgr::getMouseWheel() == -1);
        pgAssert(pgKeyMgr::isOn(pgKeyMgr::KEY_WHEELUP) && pgKeyMgr::isOn(pgKeyMgr::KEY_WHEELDOWN));

        pgKeyMgr::resetKeyStateForSystem();
        pgAssert(pgKeyMgr::getMouseX() == 12 && pgKeyMgr::getMouseY() == 34);
        pgAssert(pgKeyMgr::getMouseWheel() == 0);

        pgKeyMgr::updateKeyStateForSystem();
        pgAssert(pgKeyMgr::getMouseX() == 12 && pgKeyMgr::getMouseY() == 34);
        pgAssert(pgKeyMgr::getMouseWheel() == 0);

        pgKeyMgr::setMousePos(23, 45);
        pgAssert(pgKeyMgr::getMouseX() == 23 && pgKeyMgr::getMouseY() == 45);

        pgKeyMgr::updateKeyStateForSystem();
        pgAssert(pgKeyMgr::getMouseX() == 23 && pgKeyMgr::getMouseY() == 45);

        pgKeyMgr::setMouseVisible(false);
        pgAssert(!pgKeyMgr::isMouseVisible());

        pgKeyMgr::setMouseVisible(true);
        pgAssert(pgKeyMgr::isMouseVisible());

        pgKeyMgr::destroyBeforeSys();
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
        pgAssert(pgKeyMgr::EXTRA_VALUE_NUM == 16);

        pgAssertThrow(pgKeyMgr::getExtraEventHandlerN(), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::setExtraEventHandler(NULL), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::getExtraValue_s32(0), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::getExtraValue_r32(0), pgKeyMgr::ExceptionNotInitialized);
        pgAssertThrow(pgKeyMgr::updateExtraValueForSystem(), pgKeyMgr::ExceptionNotInitialized);

        pgKeyMgr::createAfterTask();

        pgAssert(pgKeyMgr::getExtraEventHandlerN() == pgKeyMgr::defaultExtraEventHandler);

        for (u32 i = 0; i < pgKeyMgr::EXTRA_VALUE_NUM; i++)
        {
            pgAssert(pgKeyMgr::getExtraValue_s32(i) == 0);
            pgAssert(isEqual(pgKeyMgr::getExtraValue_r32(i), 0.0f));
        }

        pgKeyMgr::setExtraEventHandler(NULL);
        pgAssert(!pgKeyMgr::getExtraEventHandlerN());

        pgTaskMgr::updateForSystem();

        for (u32 i = 0; i < pgKeyMgr::EXTRA_VALUE_NUM; i++)
        {
            pgAssert(pgKeyMgr::getExtraValue_s32(i) == 0);
            pgAssert(isEqual(pgKeyMgr::getExtraValue_r32(i), 0.0f));
        }

        pgKeyMgr::defaultExtraEventHandler(0, 123.0f);
        pgAssert(pgKeyMgr::getExtraValue_s32(0) == 0);
        pgAssert(isEqual(pgKeyMgr::getExtraValue_r32(0), 0.0f));

        pgKeyMgr::defaultExtraEventHandler(pgKeyMgr::EXTRA_VALUE_NUM + 100, 123.0f);

        pgKeyMgr::updateExtraValueForSystem();
        pgAssert(pgKeyMgr::getExtraValue_s32(0) == 123);
        pgAssert(isEqual(pgKeyMgr::getExtraValue_r32(0), 123.0f));

        pgAssertThrow(pgKeyMgr::getExtraValue_s32(pgKeyMgr::EXTRA_VALUE_NUM), pgKeyMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgKeyMgr::getExtraValue_r32(pgKeyMgr::EXTRA_VALUE_NUM), pgKeyMgr::ExceptionInvalidArgument);

        pgKeyMgr::destroyBeforeSys();
    }
}
