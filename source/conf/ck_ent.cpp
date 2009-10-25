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


#include "pg_conf_all.h"

#include "pg_private_macro.h"


pgEnt* pgEnt::getPrevN() const
{
    pgList<pgEnt>::Item* prev = m_item.getPrevN();

    return prev ? prev->getSelf() : NULL;
}


pgEnt* pgEnt::getNextN() const
{
    pgList<pgEnt>::Item* next = m_item.getNextN();

    return next ? next->getSelf() : NULL;
}


const pgStr<char, pgEnt::MAX_NAME_LENGTH>& pgEnt::getName() const
{
    return m_name;
}


u16 pgEnt::getValueNum() const
{
    return m_val_num;
}


pgEnt::ValueType pgEnt::getValueType(u16 index) const
{
    if (index >= m_val_num)
    {
        return TYPE_S32;
    }

    return m_val_type[index].getType();
}


s32 pgEnt::getValue_s32(u16 index) const
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


r32 pgEnt::getValue_r32(u16 index) const
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


const char* pgEnt::getValue_string(u16 index) const
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


pgEnt::pgEnt(const char* name, u16 val_num, pgType<u8, ValueType>* val_type, Value* val)
{
    m_name = name;
    m_name_id = pgID::genID(m_name.getString());
    m_val_num = val_num;
    m_val_type = val_type;
    m_val = val;

    m_item.init(this);
}


PG_DEFINE_OPERATOR_EQUAL(pgEnt)
