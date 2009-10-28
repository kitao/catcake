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


void ckDbgMgrTest()
{
    /*
        static const ckID DEBUG_MODE_SCREEN_ID
        static const ckID DEBUG_FONT_TEXTURE_ID
    */
    {
        ckAssert(ckDbgMgr::DEBUG_MODE_SCREEN_ID == ckID_("DEBUG_MODE_SCREEN"));
        ckAssert(ckDbgMgr::DEBUG_FONT_TEXTURE_ID == ckID_("DEBUG_FONT_TEXTURE"));

        ckAssertThrow(ckDrawMgr::getScreen(ckDbgMgr::DEBUG_MODE_SCREEN_ID), ckDrawMgr::ExceptionNotFound);
        ckAssertThrow(ckDrawMgr::getTexture(ckDbgMgr::DEBUG_FONT_TEXTURE_ID), ckDrawMgr::ExceptionNotFound);

        ckDbgMgr::createLast();

        ckScr* dbg_mode_scr = ckDrawMgr::getScreen(ckDbgMgr::DEBUG_MODE_SCREEN_ID);

        ckAssert(dbg_mode_scr->getLeftInFramebuffer() == 0 && dbg_mode_scr->getTopInFramebuffer() == 0 && //
            dbg_mode_scr->getWidthInFramebuffer() == ckSysMgr::getFramebufferWidth() && //
            dbg_mode_scr->getHeightInFramebuffer() == ckSysMgr::getFramebufferHeight());
        ckAssert(dbg_mode_scr->getViewWidth() == ckSysMgr::getFramebufferWidth() && //
            dbg_mode_scr->getViewHeight() == ckSysMgr::getFramebufferHeight());
        ckAssert(dbg_mode_scr->isEnabled());
        ckAssert(!dbg_mode_scr->isClearColor() && dbg_mode_scr->isClearDepth());
        ckAssert(dbg_mode_scr->getClearColor() == ckCol(255, 0, 0, 0));
        ckAssert(!dbg_mode_scr->isPerspective());
        ckAssert(isEqual(dbg_mode_scr->getFocusDist(), static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST)));
        ckAssert(isEqual(dbg_mode_scr->getNearClipDist(), static_cast<r32>(ckScr::DEFAULT_NEAR_CLIP_DIST)) && //
            isEqual(dbg_mode_scr->getFarClipDist(), static_cast<r32>(ckScr::DEFAULT_FAR_CLIP_DIST)));
        ckAssert(isEqual(dbg_mode_scr->view(), ckMat::UNIT.translate(0.0f, 0.0f, static_cast<r32>(ckScr::DEFAULT_FOCUS_DIST))));

        ckTex* dbg_font_tex = ckDrawMgr::getTexture(ckDbgMgr::DEBUG_FONT_TEXTURE_ID);

        ckAssert(dbg_font_tex->getID() == ckID_("DEBUG_FONT_TEXTURE"));
        ckAssert(dbg_font_tex->getWidth() == 128 && dbg_font_tex->getHeight() == 64);
        ckAssert(dbg_font_tex->getFormat() == ckTex::FORMAT_ALPHA);
        ckAssert(dbg_font_tex->getMode() == ckTex::MODE_READ_WRITE);
        ckAssert(dbg_font_tex->getImage() && dbg_font_tex->getImageSize() == 8192);

        ckDbgMgr::destroySecond();
    }

    /*
        static bool isCreated()
        static void createLast()
        static void destroySecond()
    */
    {
        ckAssert(!ckDbgMgr::isCreated());

        ckDbgMgr::createLast();
        ckDbgMgr::createLast();

        ckAssert(ckDbgMgr::isCreated());

        ckDbgMgr::destroySecond();
        ckDbgMgr::destroySecond();

        ckAssert(!ckDbgMgr::isCreated());
    }

    /*
        static DebugMode getDebugMode()
        static void setDebugMode(DebugMode dbg_mode)
        static void pageUpConsole()
        static void pageDownConsole()
    */
    {
        ckAssert(ckDbgMgr::getDebugMode() == ckDbgMgr::MODE_OFF);

        ckDbgMgr::setDebugMode(ckDbgMgr::MODE_MONITOR);
        ckAssert(ckDbgMgr::getDebugMode() == ckDbgMgr::MODE_OFF);

        ckDbgMgr::pageUpConsole();
        ckDbgMgr::pageDownConsole();

        ckDbgMgr::createLast();

        ckAssert(ckDbgMgr::getDebugMode() == ckDbgMgr::MODE_OFF);

        ckDbgMgr::setDebugMode(ckDbgMgr::MODE_CONSOLE);
        ckAssert(ckDbgMgr::getDebugMode() == ckDbgMgr::MODE_CONSOLE);

        ckDbgMgr::setDebugMode(ckDbgMgr::MODE_MONITOR);
        ckAssert(ckDbgMgr::getDebugMode() == ckDbgMgr::MODE_MONITOR);

        ckDbgMgr::pageUpConsole();
        ckDbgMgr::pageDownConsole();

        ckDbgMgr::destroySecond();
    }

    /*
        static void drawLine(const ckVec& pos1, const ckVec& pos2, ckCol col, ckID scr_id)
        static void drawPolygon(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, ckCol col, ckID scr_id)
        static void drawPolygon(const ckVec& pos1, const ckVec& pos2, const ckVec& pos3, const ckVec& pos4, ckCol col, ckID scr_id)
        static void drawBox(const ckMat& center, const ckVec& size, ckCol poly_col, ckCol line_col, ckID scr_id)
        static void drawSphere(const ckVec& center, r32 radius, ckCol poly_col, ckCol line_col, ckID scr_id)
        static void drawAxis(const ckMat& world, r32 size, ckID scr_id)
        static void drawString(r32 left, r32 top, ckCol col, u8 scale, const char* str, ...)
        static void trace(const char* str, ...)
    */
    {
        ckDbgMgr::drawLine(ckVec::ZERO, ckVec::ZERO, ckCol::ZERO, ckID::ZERO);
        ckDbgMgr::drawPolygon(ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckCol::ZERO, ckID::ZERO);
        ckDbgMgr::drawPolygon(ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckCol::ZERO, ckID::ZERO);
        ckDbgMgr::drawBox(ckMat::ZERO, ckVec::ZERO, ckCol::ZERO, ckCol::ZERO, ckID::ZERO);
        ckDbgMgr::drawSphere(ckVec::ZERO, 0.0f, ckCol::ZERO, ckCol::ZERO, ckID::ZERO);
        ckDbgMgr::drawAxis(ckMat::ZERO, 0.0f, ckID::ZERO);
        ckDbgMgr::drawString(0.0f, 0.0f, ckCol::ZERO, 0, NULL);
        ckDbgMgr::trace(NULL);

        ckDbgMgr::createLast();

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDbgMgr::drawLine(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckCol::FULL, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckDbgMgr::drawPolygon(ckVec(-1.0f, -2.0f, -3.0f), ckVec(-4.0f, -5.0f, -6.0f), ckVec(-7.0f, -8.0f, -9.0f), ckCol(255, 0, 0, 128), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckDbgMgr::drawPolygon(ckVec(10.0f, 20.0f, 30.0f), ckVec(40.0f, 50.0f, 60.0f), ckVec(70.0f, 80.0f, 90.0f), ckCol(0, 255, 0, 128), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckDbgMgr::drawBox(ckMat::UNIT.rotateX_s32(30), ckVec(1.0f, 2.0f, 3.0f), ckCol(0, 0, 255, 64), ckCol(255, 255, 0, 96), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckDbgMgr::drawSphere(ckVec(11.0f, 22.0f, 33.0f), 123.0f, ckCol(255, 255, 0), ckCol(0, 255, 255), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckDbgMgr::drawAxis(ckMat::UNIT.translate(0.0f, 0.0f, -10.0f), 10.0f, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        for (s32 i = 0; i < 100; i++)
        {
            ckDbgMgr::drawString(-10.0f, -20.0f, ckCol::FULL, 10, "TEST TEST TEST TEST");
            ckDbgMgr::trace("\tTEST TEST TEST TEST\n");
        }

        ckAssert(ckMemMgr::getCurUsedMemorySize() > used_memory_size);

        ckTaskMgr::updateForSystem();

        used_memory_size = ckMemMgr::getCurUsedMemorySize();

        ckDbgMgr::drawLine(ckVec(1.0f, 2.0f, 3.0f), ckVec(4.0f, 5.0f, 6.0f), ckCol::FULL, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckDbgMgr::drawPolygon(ckVec(-1.0f, -2.0f, -3.0f), ckVec(-4.0f, -5.0f, -6.0f), ckVec(-7.0f, -8.0f, -9.0f), ckCol(255, 0, 0, 128), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckDbgMgr::drawPolygon(ckVec(10.0f, 20.0f, 30.0f), ckVec(40.0f, 50.0f, 60.0f), ckVec(70.0f, 80.0f, 90.0f), ckCol(0, 255, 0, 128), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckDbgMgr::drawBox(ckMat::UNIT.rotateX_s32(30), ckVec(1.0f, 2.0f, 3.0f), ckCol(0, 0, 255, 64), ckCol(255, 255, 0, 96), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
        ckDbgMgr::drawSphere(ckVec(11.0f, 22.0f, 33.0f), 123.0f, ckCol(255, 255, 0), ckCol(0, 255, 255), ckDrawMgr::DEFAULT_2D_SCREEN_ID);
        ckDbgMgr::drawAxis(ckMat::UNIT.translate(0.0f, 0.0f, -10.0f), 10.0f, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

        for (s32 i = 0; i < 100; i++)
        {
            ckDbgMgr::drawString(-10.0f, -20.0f, ckCol::FULL, 10, "TEST TEST TEST TEST");
            ckDbgMgr::trace("\tTEST TEST TEST TEST\n");
        }

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckTaskMgr::updateForSystem();

        ckAssertThrow(ckDbgMgr::drawLine(ckVec::ZERO, ckVec::ZERO, ckCol::ZERO, ckID::ZERO), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawPolygon(ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckCol::ZERO, ckID::ZERO), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawPolygon(ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckVec::ZERO, ckCol::ZERO, ckID::ZERO), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawBox(ckMat::ZERO, ckVec::ZERO, ckCol::ZERO, ckCol::ZERO, ckID::ZERO), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawSphere(ckVec::ZERO, 0.0f, ckCol::ZERO, ckCol::ZERO, ckID::ZERO), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawAxis(ckMat::ZERO, 0.0f, ckID::ZERO), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawString(0.0f, 0.0f, ckCol::ZERO, 0, ""), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::drawString(0.0f, 0.0f, ckCol::ZERO, 1, NULL), ckDbgMgr::ExceptionInvalidArgument);
        ckAssertThrow(ckDbgMgr::trace(NULL), ckDbgMgr::ExceptionInvalidArgument);

        ckDbgMgr::destroySecond();
    }

    /*
        static void dumpVector(const ckVec& vec, const char* name = NULL)
        static void dumpMatrix(const ckMat& mat, const char* name = NULL)
        static void dumpMemory()
        static void dumpTask()
        static void dumpResource()
        static void dumpConfig()
        static void dumpScreen()
        static void dumpTexture()
        static void dumpShader()
    */
    {
        ckDbgMgr::dumpVector(ckVec(1.0f, 2.0f, 3.0f), "VEC");
        ckDbgMgr::dumpMatrix(ckMat::UNIT, "MAT");
        ckDbgMgr::dumpMemory();
        ckDbgMgr::dumpTask();
        ckDbgMgr::dumpResource();
        ckDbgMgr::dumpConfig();
        ckDbgMgr::dumpScreen();
        ckDbgMgr::dumpTexture();
        ckDbgMgr::dumpShader();

        ckDbgMgr::createLast();

        ckDbgMgr::dumpVector(ckVec(1.0f, 2.0f, 3.0f));
        ckDbgMgr::dumpVector(ckVec(4.0f, 5.0f, 6.0f), "TEST1");
        ckDbgMgr::dumpMatrix(ckMat::UNIT);
        ckDbgMgr::dumpMatrix(ckMat::ZERO, "TEST2");
        ckDbgMgr::dumpMemory();
        ckDbgMgr::dumpTask();
        ckDbgMgr::dumpResource();
        ckDbgMgr::dumpConfig();
        ckDbgMgr::dumpScreen();
        ckDbgMgr::dumpTexture();
        ckDbgMgr::dumpShader();

        ckDbgMgr::destroySecond();
    }
}
