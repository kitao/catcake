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


void pgDbgMgrTest()
{
    /*
        static const pgID DEBUG_MODE_SCREEN_ID
        static const pgID DEBUG_FONT_TEXTURE_ID
    */
    {
        pgAssert(pgDbgMgr::DEBUG_MODE_SCREEN_ID == pgID_("DEBUG_MODE_SCREEN"));
        pgAssert(pgDbgMgr::DEBUG_FONT_TEXTURE_ID == pgID_("DEBUG_FONT_TEXTURE"));

        pgAssertThrow(pgDrawMgr::getScreen(pgDbgMgr::DEBUG_MODE_SCREEN_ID), pgDrawMgr::ExceptionNotFound);
        pgAssertThrow(pgDrawMgr::getTexture(pgDbgMgr::DEBUG_FONT_TEXTURE_ID), pgDrawMgr::ExceptionNotFound);

        pgDbgMgr::createLast();

        pgScr* dbg_mode_scr = pgDrawMgr::getScreen(pgDbgMgr::DEBUG_MODE_SCREEN_ID);

        pgAssert(dbg_mode_scr->getLeftInFramebuffer() == 0 && dbg_mode_scr->getTopInFramebuffer() == 0 && //
            dbg_mode_scr->getWidthInFramebuffer() == pgSysMgr::getFramebufferWidth() && //
            dbg_mode_scr->getHeightInFramebuffer() == pgSysMgr::getFramebufferHeight());
        pgAssert(dbg_mode_scr->getViewWidth() == pgSysMgr::getFramebufferWidth() && //
            dbg_mode_scr->getViewHeight() == pgSysMgr::getFramebufferHeight());
        pgAssert(dbg_mode_scr->isActive());
        pgAssert(!dbg_mode_scr->isClearColor() && dbg_mode_scr->isClearDepth());
        pgAssert(dbg_mode_scr->getClearColor() == pgCol(255, 0, 0, 0));
        pgAssert(!dbg_mode_scr->isPerspective());
        pgAssert(isEqual(dbg_mode_scr->getFocusDist(), static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST)));
        pgAssert(isEqual(dbg_mode_scr->getNearClipDist(), static_cast<r32>(pgScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(dbg_mode_scr->getFarClipDist(), static_cast<r32>(pgScr::DEFAULT_FAR_CLIP_DIST)));
        pgAssert(isEqual(dbg_mode_scr->view(), pgMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(pgScr::DEFAULT_FOCUS_DIST))));

        pgTex* dbg_font_tex = pgDrawMgr::getTexture(pgDbgMgr::DEBUG_FONT_TEXTURE_ID);

        pgAssert(dbg_font_tex->getID() == pgID_("DEBUG_FONT_TEXTURE"));
        pgAssert(dbg_font_tex->getWidth() == 128 && dbg_font_tex->getHeight() == 64);
        pgAssert(dbg_font_tex->getFormat() == pgTex::FORMAT_ALPHA);
        pgAssert(dbg_font_tex->getMode() == pgTex::MODE_READ_WRITE);
        pgAssert(dbg_font_tex->getImage() && dbg_font_tex->getImageSize() == 8192);

        pgDbgMgr::destroySecond();
    }

    /*
        static bool isCreated()
        static void createLast()
        static void destroySecond()
    */
    {
        pgAssert(!pgDbgMgr::isCreated());

        pgDbgMgr::createLast();
        pgDbgMgr::createLast();

        pgAssert(pgDbgMgr::isCreated());

        pgDbgMgr::destroySecond();
        pgDbgMgr::destroySecond();

        pgAssert(!pgDbgMgr::isCreated());
    }

    /*
        static DebugMode getDebugMode()
        static void setDebugMode(DebugMode dbg_mode)
        static void pageUpConsole()
        static void pageDownConsole()
    */
    {
        pgAssert(pgDbgMgr::getDebugMode() == pgDbgMgr::MODE_OFF);

        pgDbgMgr::setDebugMode(pgDbgMgr::MODE_MONITOR);
        pgAssert(pgDbgMgr::getDebugMode() == pgDbgMgr::MODE_OFF);

        pgDbgMgr::pageUpConsole();
        pgDbgMgr::pageDownConsole();

        pgDbgMgr::createLast();

        pgAssert(pgDbgMgr::getDebugMode() == pgDbgMgr::MODE_OFF);

        pgDbgMgr::setDebugMode(pgDbgMgr::MODE_CONSOLE);
        pgAssert(pgDbgMgr::getDebugMode() == pgDbgMgr::MODE_CONSOLE);

        pgDbgMgr::setDebugMode(pgDbgMgr::MODE_MONITOR);
        pgAssert(pgDbgMgr::getDebugMode() == pgDbgMgr::MODE_MONITOR);

        pgDbgMgr::pageUpConsole();
        pgDbgMgr::pageDownConsole();

        pgDbgMgr::destroySecond();
    }

    /*
        static void drawLine(const pgVec& pos1, const pgVec& pos2, pgCol col, pgID scr_id)
        static void drawPolygon(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3, pgCol col, pgID scr_id)
        static void drawPolygon(const pgVec& pos1, const pgVec& pos2, const pgVec& pos3, const pgVec& pos4, pgCol col, pgID scr_id)
        static void drawBox(const pgMat& center, const pgVec& size, pgCol poly_col, pgCol line_col, pgID scr_id)
        static void drawSphere(const pgVec& center, r32 radius, pgCol poly_col, pgCol line_col, pgID scr_id)
        static void drawAxis(const pgMat& world, r32 size, pgID scr_id)
        static void drawString(r32 left, r32 top, pgCol col, u8 scale, const char* str, ...)
        static void trace(const char* str, ...)
    */
    {
        pgDbgMgr::drawLine(pgVec::ZERO, pgVec::ZERO, pgCol::ZERO, pgID::ZERO);
        pgDbgMgr::drawPolygon(pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgCol::ZERO, pgID::ZERO);
        pgDbgMgr::drawPolygon(pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgCol::ZERO, pgID::ZERO);
        pgDbgMgr::drawBox(pgMat::ZERO, pgVec::ZERO, pgCol::ZERO, pgCol::ZERO, pgID::ZERO);
        pgDbgMgr::drawSphere(pgVec::ZERO, 0.0f, pgCol::ZERO, pgCol::ZERO, pgID::ZERO);
        pgDbgMgr::drawAxis(pgMat::ZERO, 0.0f, pgID::ZERO);
        pgDbgMgr::drawString(0.0f, 0.0f, pgCol::ZERO, 0, NULL);
        pgDbgMgr::trace(NULL);

        pgDbgMgr::createLast();

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDbgMgr::drawLine(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgCol::FULL, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgDbgMgr::drawPolygon(pgVec(-1.0f, -2.0f, -3.0f), pgVec(-4.0f, -5.0f, -6.0f), pgVec(-7.0f, -8.0f, -9.0f), pgCol(255, 0, 0, 128), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgDbgMgr::drawPolygon(pgVec(10.0f, 20.0f, 30.0f), pgVec(40.0f, 50.0f, 60.0f), pgVec(70.0f, 80.0f, 90.0f), pgCol(0, 255, 0, 128), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgDbgMgr::drawBox(pgMat::UNIT.rotateX_s32(30), pgVec(1.0f, 2.0f, 3.0f), pgCol(0, 0, 255, 64), pgCol(255, 255, 0, 96), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgDbgMgr::drawSphere(pgVec(11.0f, 22.0f, 33.0f), 123.0f, pgCol(255, 255, 0), pgCol(0, 255, 255), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgDbgMgr::drawAxis(pgMat::UNIT.translate(0.0f, 0.0f, -10.0f), 10.0f, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        for (s32 i = 0; i < 100; i++)
        {
            pgDbgMgr::drawString(-10.0f, -20.0f, pgCol::FULL, 10, "TEST TEST TEST TEST");
            pgDbgMgr::trace("\tTEST TEST TEST TEST\n");
        }

        pgAssert(pgMemMgr::getCurUsedMemorySize() > used_memory_size);

        pgTaskMgr::updateForSystem();

        used_memory_size = pgMemMgr::getCurUsedMemorySize();

        pgDbgMgr::drawLine(pgVec(1.0f, 2.0f, 3.0f), pgVec(4.0f, 5.0f, 6.0f), pgCol::FULL, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgDbgMgr::drawPolygon(pgVec(-1.0f, -2.0f, -3.0f), pgVec(-4.0f, -5.0f, -6.0f), pgVec(-7.0f, -8.0f, -9.0f), pgCol(255, 0, 0, 128), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgDbgMgr::drawPolygon(pgVec(10.0f, 20.0f, 30.0f), pgVec(40.0f, 50.0f, 60.0f), pgVec(70.0f, 80.0f, 90.0f), pgCol(0, 255, 0, 128), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgDbgMgr::drawBox(pgMat::UNIT.rotateX_s32(30), pgVec(1.0f, 2.0f, 3.0f), pgCol(0, 0, 255, 64), pgCol(255, 255, 0, 96), pgDrawMgr::DEFAULT_3D_SCREEN_ID);
        pgDbgMgr::drawSphere(pgVec(11.0f, 22.0f, 33.0f), 123.0f, pgCol(255, 255, 0), pgCol(0, 255, 255), pgDrawMgr::DEFAULT_2D_SCREEN_ID);
        pgDbgMgr::drawAxis(pgMat::UNIT.translate(0.0f, 0.0f, -10.0f), 10.0f, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

        for (s32 i = 0; i < 100; i++)
        {
            pgDbgMgr::drawString(-10.0f, -20.0f, pgCol::FULL, 10, "TEST TEST TEST TEST");
            pgDbgMgr::trace("\tTEST TEST TEST TEST\n");
        }

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgTaskMgr::updateForSystem();

        pgAssertThrow(pgDbgMgr::drawLine(pgVec::ZERO, pgVec::ZERO, pgCol::ZERO, pgID::ZERO), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawPolygon(pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgCol::ZERO, pgID::ZERO), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawPolygon(pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgVec::ZERO, pgCol::ZERO, pgID::ZERO), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawBox(pgMat::ZERO, pgVec::ZERO, pgCol::ZERO, pgCol::ZERO, pgID::ZERO), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawSphere(pgVec::ZERO, 0.0f, pgCol::ZERO, pgCol::ZERO, pgID::ZERO), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawAxis(pgMat::ZERO, 0.0f, pgID::ZERO), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawString(0.0f, 0.0f, pgCol::ZERO, 0, ""), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::drawString(0.0f, 0.0f, pgCol::ZERO, 1, NULL), pgDbgMgr::ExceptionInvalidArgument);
        pgAssertThrow(pgDbgMgr::trace(NULL), pgDbgMgr::ExceptionInvalidArgument);

        pgDbgMgr::destroySecond();
    }

    /*
        static void dumpVector(const pgVec& vec, const char* name = NULL)
        static void dumpMatrix(const pgMat& mat, const char* name = NULL)
        static void dumpMemory()
        static void dumpTask()
        static void dumpResource()
        static void dumpConfig()
        static void dumpScreen()
        static void dumpTexture()
        static void dumpShader()
    */
    {
        pgDbgMgr::dumpVector(pgVec(1.0f, 2.0f, 3.0f), "VEC");
        pgDbgMgr::dumpMatrix(pgMat::UNIT, "MAT");
        pgDbgMgr::dumpMemory();
        pgDbgMgr::dumpTask();
        pgDbgMgr::dumpResource();
        pgDbgMgr::dumpConfig();
        pgDbgMgr::dumpScreen();
        pgDbgMgr::dumpTexture();
        pgDbgMgr::dumpShader();

        pgDbgMgr::createLast();

        pgDbgMgr::dumpVector(pgVec(1.0f, 2.0f, 3.0f));
        pgDbgMgr::dumpVector(pgVec(4.0f, 5.0f, 6.0f), "TEST1");
        pgDbgMgr::dumpMatrix(pgMat::UNIT);
        pgDbgMgr::dumpMatrix(pgMat::ZERO, "TEST2");
        pgDbgMgr::dumpMemory();
        pgDbgMgr::dumpTask();
        pgDbgMgr::dumpResource();
        pgDbgMgr::dumpConfig();
        pgDbgMgr::dumpScreen();
        pgDbgMgr::dumpTexture();
        pgDbgMgr::dumpShader();

        pgDbgMgr::destroySecond();
    }
}
