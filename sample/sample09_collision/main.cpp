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


#include "catcake_main.h"


void newCatcakeLogo(const char** cmd, u8 cmd_num);
void newCollisionChecker();


static const char* s_cmd[] =
{
    "BORDER DRAG: RESIZE WINDOW", //
    "MOUSE WHEEL: ZOOM CAMERA", //
    "L-BUTTON DRAG: MOVE OBJECT/CAMERA", //
    "M-BUTTON DRAG: ZOOM OBJECT/CAMERA", //
    "R-BUTTON DRAG: ROTATE OBJECT/CAMERA", //
    "A: CHANGE OBJECT1 TYPE", //
    "B: CHANGE OBJECT2 TYPE", //
    "P: TOGGLE PUSH BACK", //
    "R: RESET POSITION", //
    "Q: QUIT"
};


ckMain()
{
    ckCreateCatcake("Sample09 - Collision", 800, 600, 60, ckSysMgr::FLAG_VARIABLE_SIZE);

    ckResMgr::loadResource("../data/catcake_logo_71x14.png", true);

    newCatcakeLogo(s_cmd, sizeof(s_cmd) / sizeof(char*));
    newCollisionChecker();

    ckStartCatcake();
    ckDestroyCatcake();
}
