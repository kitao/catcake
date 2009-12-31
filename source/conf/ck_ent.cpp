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


#include "ck_conf_all.h"

#include "ck_private_macro.h"


ckEnt* ckEnt::getPrevN() const
{
    ckList<ckEnt>::Item* prev = m_item.getPrevN();

    return prev ? prev->getSelf() : NULL;
}


ckEnt* ckEnt::getNextN() const
{
    ckList<ckEnt>::Item* next = m_item.getNextN();

    return next ? next->getSelf() : NULL;
}


const ckStr<char, ckEnt::MAX_NAME_LENGTH>& ckEnt::getName() const
{
    return m_name;
}


u16 ckEnt::getValueNum() const
{
    return m_val_num;
}


ckEnt::ValueType ckEnt::getValueType(u16 index) const
{
    if (index >= m_val_num)
    {
        return TYPE_S32;
    }

    return m_val_type[index].getType();
}


s32 ckEnt::getValue_s32(u16 index) const
{
    if (index >= m_val_num)
    {
        return 0;
    }

    ValueType type = m_val_type[index].getType();

    switch (type)
    {
    case TYPE_S32:
        return m_val[index].val_s32;

    case TYPE_R32:
        return static_cast<s32>(m_val[index].val_r32);

    default:
        return 0;
    }
}


r32 ckEnt::getValue_r32(u16 index) const
{
    if (index >= m_val_num)
    {
        return 0.0f;
    }

    ValueType type = m_val_type[index].getType();

    switch (type)
    {
    case TYPE_S32:
        return static_cast<r32>(m_val[index].val_s32);

    case TYPE_R32:
        return m_val[index].val_r32;

    default:
        return 0.0f;
    }
}


const char* ckEnt::getValue_string(u16 index) const
{
    if (index >= m_val_num)
    {
        return "";
    }

    ValueType type = m_val_type[index].getType();

    switch (type)
    {
    case TYPE_STRING:
        return m_val[index].val_str;

    default:
        return "";
    }
}


ckEnt::ckEnt(const char* name, u16 val_num, ckType<u8, ValueType>* val_type, Value* val)
{
    m_name = name;
    m_name_id = ckID::genID(m_name.getString());
    m_val_num = val_num;
    m_val_type = val_type;
    m_val = val;

    m_item.init(this);
}


CK_DEFINE_OPERATOR_EQUAL(ckEnt)
