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


#include "catcake.h"


class DebugMode : public ckTask
{
public:
    DebugMode();

private:
    virtual void onUpdate();
};


void newDebugMode()
{
    ckNewTask(DebugMode);
}


DebugMode::DebugMode() : ckTask(ORDER_ZERO) {}


void DebugMode::onUpdate()
{
    ckCol col(0, 255, 0);

    switch (ckDbgMgr::getDebugMode())
    {
    case ckDbgMgr::MODE_OFF:
        ckDbgMgr::drawString(-162.0f, 12.0f, col, 2, "Please press 'D' and '1'");
        ckDbgMgr::drawString(-162.0f, -12.0f, col, 2, " to turn on the debug mode.");
        break;

    case ckDbgMgr::MODE_MONITOR:
        ckDbgMgr::drawString(-144.0f, 40.0f, col, 2, "This is \"DEBUG MONITOR\".");
        ckDbgMgr::drawString(-186.0f, -24.0f, col, 2, "Please press 'D' and '1' again.");
        break;

    case ckDbgMgr::MODE_CONSOLE:
        ckDbgMgr::drawString(-144.0f, 90.0f, col, 2, "This is \"DEBUG CONSOLE\".");
        ckDbgMgr::drawString(-198.0f, 58.0f, col, 2, "If you press 'D' and '2'-'8',");
        ckDbgMgr::drawString(-198.0f, 38.0f, col, 2, " you can get various information.");
        ckDbgMgr::drawString(-210.0f, 2.0f, col, 2, "You can also scroll the console");
        ckDbgMgr::drawString(-210.0f, -18.0f, col, 2, " by 'D' and 'PageUp' or 'PageDown'.");
        ckDbgMgr::drawString(-180.0f, -54.0f, col, 2, "Please press 'D' and '1' again");
        ckDbgMgr::drawString(-180.0f, -74.0f, col, 2, " to turn off the debug mode.");
        break;
    }
}
