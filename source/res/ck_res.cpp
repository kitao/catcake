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


#include "ck_res_all.h"


ckRes::ckRes()
{
    m_data = NULL;
    m_data_size = 0;
    m_exinfo = NULL;
    m_is_auto_free = false;

    // m_id and m_ext are initialized in its own constructor
}


ckID ckRes::getID() const
{
    return m_id;
}


ckStr<char, 3> ckRes::getExtension() const
{
    return m_ext;
}


u32 ckRes::getDataSize() const
{
    return m_data_size;
}


bool ckRes::isAutoFree() const
{
    return m_is_auto_free;
}


void ckRes::init(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo, bool is_auto_free)
{
    m_id = id;
    m_ext = ext;
    m_data = data;
    m_data_size = data_size;
    m_exinfo = exinfo;
    m_is_auto_free = is_auto_free;
}


ckStr<char, 3> ckRes::toUppercase(ckStr<char, 3> str)
{
    for (s32 i = 0; i < str.getLength(); i++)
    {
        char c = str[i];

        if (c >= 'a' && c <= 'z')
        {
            str[i] = 'A' + (c - 'a');
        }
    }

    return str;
}
