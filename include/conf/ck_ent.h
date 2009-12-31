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


class ckConf;


/*!
    @ingroup ckConf
    An entry of a configuration script.
*/
class CK_API ckEnt
{
    friend class ckConf;

public:
    /*!
        The maximum length of the name.
    */
    static const u32 MAX_NAME_LENGTH = 20;

    /*!
        The value types of an entry.
    */
    enum ValueType
    {
        TYPE_S32, //!< 32-bit singed number.
        TYPE_R32, //!< 32-bit floating point number.
        TYPE_STRING //!< 8-bit string.
    };

    /*!
        Returns the previous entry. If the previous entry doesn't exit, returns NULL.
        @return The previous entry.
    */
    ckEnt* getPrevN() const;

    /*!
        Returns the next entry. If the next entry doesn't exit, returns NULL.
        @return The next entry.
    */
    ckEnt* getNextN() const;

    /*!
        Returns the name of this entry.
        @return The name of this entry.
    */
    const ckStr<char, MAX_NAME_LENGTH>& getName() const;

    /*!
        Returns the number of the values.
        @return The number of the values.
    */
    u16 getValueNum() const;

    /*!
        Returns the type of the specified value.
        @param[in] index The index of a value.
        @return The type of the specified value.
    */
    ValueType getValueType(u16 index) const;

    /*!
        Returns the specified value as 32-bit integer number.
        @param[in] index The index of a value.
        @return The specified value.
    */
    s32 getValue_s32(u16 index) const;

    /*!
        Returns the specified value as 32-bit floating point number.
        @param[in] index The index of a value.
        @return The specified value.
    */
    r32 getValue_r32(u16 index) const;

    /*!
        Returns the specified value as string.
        @param[in] index The index of a value.
        @return The specified value.
    */
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
