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
