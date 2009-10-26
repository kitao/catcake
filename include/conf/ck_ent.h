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


class ckConf;


/*!
    @ingroup ckConf
    TODO
*/
class CK_API ckEnt
{
    friend class ckConf;

public:
    static const u32 MAX_NAME_LENGTH = 20;

    enum ValueType
    {
        TYPE_S32, //
        TYPE_R32, //
        TYPE_STRING
    };

    ckEnt* getPrevN() const;
    ckEnt* getNextN() const;

    const ckStr<char, MAX_NAME_LENGTH>& getName() const;
    u16 getValueNum() const;
    ValueType getValueType(u16 index) const;
    s32 getValue_s32(u16 index) const;
    r32 getValue_r32(u16 index) const;
    const char* getValue_string(u16 index) const;

private:
    union Value
    {
        s32 val_s32;
        r32 val_r32;
        const char* val_str;
    };

    ckEnt(const char* name, u16 val_num, ckType<u8, ValueType>* val_type, Value* val);
    void operator=(const ckEnt&);

    ckList<ckEnt>::Item m_item;
    ckStr<char, MAX_NAME_LENGTH> m_name;
    ckID m_name_id;
    u16 m_val_num;
    ckType<u8, ValueType>* m_val_type;
    Value* m_val;
};
