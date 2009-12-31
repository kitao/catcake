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


class ckKeyDaemon;


/*!
    @ingroup ckKey
    The key manager singleton.
*/
class CK_API ckKeyMgr
{
    friend class ckKeyDaemon;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The number of the extra values.
    */
    static const u32 EXTRA_VALUE_NUM = 16;

    /*!
        Key types.
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
        Key states.
    */
    enum KeyState
    {
        STATE_DOWN, //!< A key is on.
        STATE_UP //!< A key is off.
    };

    /*!
        The key event handler type.
        @param[in] key The type of the key.
        @param[in] key_state The state of the key.
    */
    typedef void (*KeyEventHandler)(KeyType key, KeyState key_state);

    /*!
        The mouse event handler type.
        @param[in] mouse_x The x-coordinates of the mouse.
        @param[in] mouse_y The y-coordinates of the mouse.
    */
    typedef void (*MouseEventHandler)(s16 mouse_x, s16 mouse_y);

    /*!
        The extra event handler type.
        @param[in] val_index The index of the value.
        @param[in] value The value of the extra event.
    */
    typedef void (*ExtraEventHandler)(u8 val_index, r32 value);

    /*!
        Returns whether the key manager singleton is created.
        @return Whether the key manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the key manager singleton.
    */
    static void createAfterTask();

    /*!
        Destroys the key manager singleton.
    */
    static void destroyBeforeSys();

    /*!
        Returns the key event handler. If the key event handler doen't exist, returns NULL.
        @return The key event handler.
    */
    static KeyEventHandler getKeyEventHandlerN();

    /*!
        Sets the key event handler.
        @param[in] handler A key event handler.
    */
    static void setKeyEventHandler(KeyEventHandler handler);

    /*!
        The default key event handler.
        @param[in] key The key type.
        @param[in] key_state The state of the key.
    */
    static void defaultKeyEventHandler(KeyType key, KeyState key_state);

    /*!
        Returns the mouse event handler. If the mouse event handler doen't exit, returns NULL.
        @return The mouse event handler.
    */
    static MouseEventHandler getMouseEventHandlerN();

    /*!
        Sets the mouse event handler.
        @param[in] handler A mouse event handler.
    */
    static void setMouseEventHandler(MouseEventHandler handler);

    /*!
        The default key event handler.
        @param[in] mouse_x The x-coordinate of the mouse.
        @param[in] mouse_y The y-coordinate of the mouse.
    */
    static void defaultMouseEventHandler(s16 mouse_x, s16 mouse_y);

    /*!
        Returns the extra event handler. If the extra event handler doen't exit, return NULL.
        @return The extra event handler.
    */
    static ExtraEventHandler getExtraEventHandlerN();

    /*!
        Sets the extra event handler.
        @param[in] handler An extra event handler.
    */
    static void setExtraEventHandler(ExtraEventHandler handler);

    /*!
        The default extra event handler.
        @param[in] val_index The index of the value.
        @param[in] value The value of the extra event.
    */
    static void defaultExtraEventHandler(u8 val_index, r32 value);

    /*!
        Returns whether the specified key is on.
        @param[in] key A key.
        @return Whether the specified key is on.
    */
    static bool isOn(KeyType key);

    /*!
        Returns whether the specified key is off.
        @param[in] key A key.
        @return Whether the specified key is off.
    */
    static bool isOff(KeyType key);

    /*!
        Returns the specified key is pressed.
        @param[in] key A key.
        @return The specified key is pressed.
    */
    static bool isPressed(KeyType key);

    /*!
        Returns the specified key is released.
        @param[in] key A key.
        @return The specified key is released.
    */
    static bool isReleased(KeyType key);

    /*!
        Returns the x-coordinate of the mouse.
        @return The x-coordinate of the mouse.
    */
    static s16 getMouseX();

    /*!
        Returns the y-coordinate of the mouse.
        @return The y-coordinate of the mouse.
    */
    static s16 getMouseY();

    /*!
        Returns the wheel offset of the mouse.
        @return The wheel offset of the mouse.
    */
    static s16 getMouseWheel();

    /*!
        Sets the position of the mouse.
        @param[in] mouse_x The x-coordinate of the mouse.
        @param[in] mouse_y The y-coordinate of the mouse.
    */
    static void setMousePos(s16 mouse_x, s16 mouse_y);

    /*!
        Returns whether the mouse is visible.
        @return Whether the mouse is visible.
    */
    static bool isMouseVisible();

    /*!
        Determines whether the mouse is visible.
        @param[in] is_visible Whether the mouse is visible.
    */
    static void setMouseVisible(bool is_visible);

    /*!
        Returns the extra value as 32-bit integer number.
        @param[in] val_index The index of the extra value.
        @return The extra value.
    */
    static s32 getExtraValue_s32(u8 val_index);

    /*!
        Returns the extra value as 32-bit floating point number.
        @param[in] val_index The index of the extra value.
        @return The extra value.
    */
    static r32 getExtraValue_r32(u8 val_index);

    /*!
        Updates the states of the keys. This method is only for system.
    */
    static void updateKeyStateForSystem();

    /*!
        Resets the states of the kyes. This method is only for system.
    */
    static void resetKeyStateForSystem();

    /*!
        Updates the extra value. This method id only for system.
    */
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

    ckKeyMgr();
    ~ckKeyMgr();
    void operator=(const ckKeyMgr&);

    static ckKeyMgr* instance();

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
    ckKeyDaemon* m_key_daemon;

    static ckKeyMgr* m_instance;
};
