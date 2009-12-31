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
