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
