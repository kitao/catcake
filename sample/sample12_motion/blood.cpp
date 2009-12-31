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


class Blood : public ckTask
{
public:
    Blood(const ckVec& pos);

private:
    virtual void onUpdate();

    ckVec m_speed;
    ckMdl m_blood_mdl;
};


void newBlood(const ckVec& pos)
{
    ckNewTask(Blood)(pos);
}


Blood::Blood(const ckVec& pos) : ckTask(ORDER_ZERO)
{
    m_speed.set(ckMath::rand(-2.0f, 2.0f, 0.1f), ckMath::rand(0.0f, 2.0f, 0.1f), ckMath::rand(-1.0f, -2.0f, 0.1f));

    m_blood_mdl.init(ckID_("blood.mdl"), ckDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_blood_mdl.setLightSetID(ckDrawMgr::DEFAULT_LIGHT_SET_ID);
    m_blood_mdl.getRootDraw()->local().trans = pos;
}


void Blood::onUpdate()
{
    m_blood_mdl.getRootDraw()->local() = m_blood_mdl.getRootDraw()->local().rotateX_s32(3);
    m_blood_mdl.getRootDraw()->local().trans += m_speed;

    m_speed.y -= 0.1f;

    if (m_blood_mdl.getRootDraw()->local().trans.y < 0.0f)
    {
        ckDeleteTask(this);
    }
}
