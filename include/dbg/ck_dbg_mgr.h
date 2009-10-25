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


class pgDbgDaemon;


/*!
    @ingroup pgDbg
    TODO
*/
class PG_API pgDbgMgr
{
    friend class pgDbgDaemon;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotInitialized);

    static const pgID DEBUG_MODE_SCREEN_ID;
    static const pgID DEBUG_FONT_TEXTURE_ID;

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

    static void drawLine(const pgVec& pos1, const pgVec& pos2, pgCol col, pgID scr_id);
    static void drawPolygon(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3, pgCol col, pgID scr_id);
    static void drawPolygon(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3, const pgVec& pos4, pgCol col, pgID scr_id);

    static void drawBox(const pgMat& center, const pgVec& size, pgCol poly_col, pgCol line_col, pgID scr_id);
    static void drawSphere(const pgVec& center, r32 radius, pgCol poly_col, pgCol line_col, pgID scr_id);
    static void drawAxis(const pgMat& world, r32 size, pgID scr_id);

    static void drawString(r32 left, r32 top, pgCol col, u8 scale, const char* str, ...);
    static void trace(const char* str, ...);

    static void dumpVector(const pgVec& vec, const char* name = NULL);
    static void dumpMatrix(const pgMat& mat, const char* name = NULL);

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

    pgDbgMgr();
    ~pgDbgMgr();
    void operator=(const pgDbgMgr&);

    void newDebugFontTexture();
    void reallocDrawPrim();
    void checkSpecialCommand();

    void drawConsole();
    static void drawRect(r32 left, r32 top, r32 width, r32 height, r32 z, pgCol col);
    static void drawFrame(r32 left, r32 top, r32 width, r32 height, r32 z, pgCol col);
    s32 calcConsoleRowNum();

    static pgDbgMgr* instance();

    pgType<u8, DebugMode> m_dbg_mode;
    pgScr* m_dbg_mode_scr;
    pgPrim* m_draw_prim;
    u32 m_cur_draw_prim_num;
    u32 m_max_draw_prim_num;
    pgSprt m_font_sprt;
    pgStr<char, CONSOLE_COLUMN_NUM> m_console_buf[CONSOLE_BUFFER_SIZE];
    s32 m_end_row;
    s32 m_aim_row;
    s32 m_cur_row;
    u8 m_dbg_mode_tap_cntr;
    u8 m_dbg_dump_tap_cntr;
    s16 m_scroll_hold_cntr;
    pgDbgDaemon* m_dbg_daemon;

    static pgDbgMgr* m_instance;
};
