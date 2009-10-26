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


class ckDbgDaemon;


/*!
    @ingroup ckDbg
    TODO
*/
class CK_API ckDbgMgr
{
    friend class ckDbgDaemon;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotInitialized);

    static const ckID DEBUG_MODE_SCREEN_ID;
    static const ckID DEBUG_FONT_TEXTURE_ID;

    enum DebugMode
    {
        MODE_OFF, //
        MODE_MONITOR, //
        MODE_CONSOLE
    };

    static bool isCreated();
    static void createLast();
    static void destroySecond();

    static DebugMode getDebugMode();
    static void setDebugMode(DebugMode dbg_mode);

    static void pageUpConsole();
    static void pageDownConsole();

    static void drawLine(const ckVec& pos1, const ckVec& pos2, ckCol col, ckID scr_id);
    static void drawPolygon(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, ckCol col, ckID scr_id);
    static void drawPolygon(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, const ckVec& pos4, ckCol col, ckID scr_id);

    static void drawBox(const ckMat& center, const ckVec& size, ckCol poly_col, ckCol line_col, ckID scr_id);
    static void drawSphere(const ckVec& center, r32 radius, ckCol poly_col, ckCol line_col, ckID scr_id);
    static void drawAxis(const ckMat& world, r32 size, ckID scr_id);

    static void drawString(r32 left, r32 top, ckCol col, u8 scale, const char* str, ...);
    static void trace(const char* str, ...);

    static void dumpVector(const ckVec& vec, const char* name = NULL);
    static void dumpMatrix(const ckMat& mat, const char* name = NULL);

    static void dumpMemory();
    static void dumpTask();
    static void dumpResource();
    static void dumpConfig();
    static void dumpScreen();
    static void dumpTexture();
    static void dumpShader();

private:
    static const u32 INITIAL_DRAW_PRIM_NUM = 64;
    static const u32 INITIAL_FONT_SPRT_NUM = 256;
    static const u32 DEBUG_FONT_WIDTH = 6;
    static const u32 DEBUG_FONT_HEIGHT = 10;
    static const u32 CONSOLE_COLUMN_NUM = 80;
    static const u32 CONSOLE_BUFFER_SIZE = 1000;
    static const u32 CONSOLE_TAB_WIDTH = 4;

    ckDbgMgr();
    ~ckDbgMgr();
    void operator=(const ckDbgMgr&);

    void newDebugFontTexture();
    void reallocDrawPrim();
    void checkSpecialCommand();

    void drawConsole();
    static void drawRect(r32 left, r32 top, r32 width, r32 height, r32 z, ckCol col);
    static void drawFrame(r32 left, r32 top, r32 width, r32 height, r32 z, ckCol col);
    s32 calcConsoleRowNum();

    static ckDbgMgr* instance();

    ckType<u8, DebugMode> m_dbg_mode;
    ckScr* m_dbg_mode_scr;
    ckPrim* m_draw_prim;
    u32 m_cur_draw_prim_num;
    u32 m_max_draw_prim_num;
    ckSprt m_font_sprt;
    ckStr<char, CONSOLE_COLUMN_NUM> m_console_buf[CONSOLE_BUFFER_SIZE];
    s32 m_end_row;
    s32 m_aim_row;
    s32 m_cur_row;
    u8 m_dbg_mode_tap_cntr;
    u8 m_dbg_dump_tap_cntr;
    s16 m_scroll_hold_cntr;
    ckDbgDaemon* m_dbg_daemon;

    static ckDbgMgr* m_instance;
};
