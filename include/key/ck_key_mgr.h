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


class pgKeyDaemon;


/*!
    @ingroup pgKey
    TODO
*/
class PG_API pgKeyMgr
{
    friend class pgKeyDaemon;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotInitialized);

    static const u32 EXTRA_VALUE_NUM = 16;

    /*!
        TODO
    */
    enum KeyType
    {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, //

        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, //
        KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z, //

        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, //

        KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN, //
        KEY_SHIFT, KEY_CTRL, KEY_ALT, //

        KEY_BACKSPACE, KEY_TAB, KEY_ENTER, KEY_ESCAPE, KEY_SPACE, //
        KEY_PAGEUP, KEY_PAGEDOWN, KEY_END, KEY_HOME, KEY_INSERT, KEY_DELETE, //

        KEY_NUMPAD0, KEY_NUMPAD1, KEY_NUMPAD2, KEY_NUMPAD3, KEY_NUMPAD4, //
        KEY_NUMPAD5, KEY_NUMPAD6, KEY_NUMPAD7, KEY_NUMPAD8, KEY_NUMPAD9, //
        KEY_MULTIPLY, KEY_ADD, KEY_SEPARATOR, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE, //

        KEY_LBUTTON, KEY_MBUTTON, KEY_RBUTTON, KEY_WHEELUP, KEY_WHEELDOWN, //

        KEY_EXT_00, KEY_EXT_01, KEY_EXT_02, KEY_EXT_03, KEY_EXT_04, KEY_EXT_05, KEY_EXT_06, KEY_EXT_07, //
        KEY_EXT_08, KEY_EXT_09, KEY_EXT_10, KEY_EXT_11, KEY_EXT_12, KEY_EXT_13, KEY_EXT_14, KEY_EXT_15, //

        KEY_ANY, KEY_NONE
    };

    /*!
        TODO
    */
    enum KeyState
    {
        STATE_DOWN, //
        STATE_UP
    };

    typedef void (*KeyEventHandler)(KeyType key, KeyState key_state);
    typedef void (*MouseEventHandler)(s16 mouse_x, s16 mouse_y);
    typedef void (*ExtraEventHandler)(u8 val_index, r32 value);

    static bool isCreated();
    static void createAfterTask();
    static void destroyBeforeSys();

    static KeyEventHandler getKeyEventHandlerN();
    static void setKeyEventHandler(KeyEventHandler handler);
    static void defaultKeyEventHandler(KeyType key, KeyState key_state);

    static MouseEventHandler getMouseEventHandlerN();
    static void setMouseEventHandler(MouseEventHandler handler);
    static void defaultMouseEventHandler(s16 mouse_x, s16 mouse_y);

    static ExtraEventHandler getExtraEventHandlerN();
    static void setExtraEventHandler(ExtraEventHandler handler);
    static void defaultExtraEventHandler(u8 val_index, r32 value);

    static bool isOn(KeyType key);
    static bool isOff(KeyType key);
    static bool isPressed(KeyType key);
    static bool isReleased(KeyType key);

    static s16 getMouseX();
    static s16 getMouseY();
    static s16 getMouseWheel();
    static void setMousePos(s16 mouse_x, s16 mouse_y);
    static bool isMouseVisible();
    static void setMouseVisible(bool is_visible);

    static s32 getExtraValue_s32(u8 val_index);
    static r32 getExtraValue_r32(u8 val_index);

    static void updateKeyStateForSystem();
    static void resetKeyStateForSystem();
    static void updateExtraValueForSystem();

private:
    static const u32 KEY_TYPE_NUM = KEY_NONE + 1;

    enum KeyFlag
    {
        FLAG_REAL_ON = 0x01, //
        FLAG_DELAY_ON = 0x02, //
        FLAG_CUR_ON = 0x04, //
        FLAG_LAST_ON = 0x08
    };

    pgKeyMgr();
    ~pgKeyMgr();
    void operator=(const pgKeyMgr&);

    static pgKeyMgr* instance();

    KeyEventHandler m_key_event_handler;
    MouseEventHandler m_mouse_event_handler;
    ExtraEventHandler m_extra_event_handler;
    u8 m_key_flag[KEY_TYPE_NUM];
    r32 m_cur_ext_val[EXTRA_VALUE_NUM];
    r32 m_real_ext_val[EXTRA_VALUE_NUM];
    s16 m_mouse_x;
    s16 m_mouse_y;
    s16 m_real_mouse_wheel;
    s16 m_cur_mouse_wheel;
    pgKeyDaemon* m_key_daemon;

    static pgKeyMgr* m_instance;
};
