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


#include "ck_gen_all.h"


const ckID ckID::ZERO;

u32 ckID::m_cur_id = 1;


ckID::ckID()
{
    m_id = 0;
}


bool ckID::operator==(ckID id) const
{
    return (m_id == id.m_id);
}


bool ckID::operator!=(ckID id) const
{
    return (m_id != id.m_id);
}


u32 ckID::operator%(u32 n) const
{
    return m_id % n;
}


u32 ckID::getValue() const
{
    return m_id;
}


ckID ckID::genID()
{
    if (m_cur_id > MAX_ID)
    {
        ckThrow(ExceptionOutOfID);
    }

    ckID id;

    id.m_id = m_cur_id++;

    return id;
}


ckID ckID::genID(const char* str)
{
    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckID id;

    if (*str == '\0')
    {
        id.m_id = 0;
    }
    else
    {
        id.m_id = 0;

        for ( ; *str != '\0'; str++)
        {
            id.m_id = id.m_id * 37 + *str;
        }

        id.m_id |= 1 << (BIT_NUM - 1);
    }

    return id;
}


ckID ckID::genIDForSystem(u32 value)
{
    ckID id;

    id.m_id = value;

    return id;
}


void ckID::setCurIDForSystem(u32 value)
{
    if (value <= 0 || value > MAX_ID)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    m_cur_id = value;
}


s32 ckID::ThrowTooLongStringExceptionForSystem()
{
    ckThrow(ExceptionTooLongString);

    return 0;
}
