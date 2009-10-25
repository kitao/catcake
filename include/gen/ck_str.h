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


/*!
    @ingroup pgGen
    A string with fixed length.
    @tparam T The type of the elements of pgStr. Typically, either char or wchar_t is used.
    @tparam N The length of pgStr. This value must be more than zero.
*/
template<class T, u8 N> class pgStr
{
public:
    pgDefineException(ExceptionOutOfRange);

    /*!
        Constructs and initializes a pgStr whose length is zero.
    */
    pgStr()
    {
        m_str[0] = 0;
        m_length = 0;
    }

    /*!
        Constructs and initializes a pgStr from the specified character.
        @param[in] ch A character.
    */
    pgStr(T ch)
    {
        *this = ch;
    }

    /*!
        Constructs and initializes a pgStr from the specified string.
        @param[in] str A string. NULL is allowed.
    */
    pgStr(const T* str)
    {
        *this = str;
    }

    /*!
        Constructs and initializes a pgStr from the specified pgStr.
        @param[in] str A pgStr.
    */
    pgStr(const pgStr<T, N>& str)
    {
        *this = str;
    }

    /*!
        Constructs and initializes a pgStr from the specified pgStr with different length.
        @tparam N2 The length of a pgStr.
        @param[in] str A pgStr with different length.
    */
    template<u8 N2> pgStr(const pgStr<T, N2>& str)
    {
        *this = str;
    }

    /*!
        Assigns the right hand side character to this pgStr.
        @param[in] ch The right hand side character.
        @return A reference to this pgStr.
    */
    pgStr<T, N>& operator=(T ch)
    {
        m_str[0] = ch;
        m_str[1] = 0;

        m_length = 1;

        return *this;
    }

    /*!
        Assigns the right hand side string to this pgStr.
        @param[in] str The right hand side string.
        @return A reference to this pgStr.
    */
    pgStr<T, N>& operator=(const T* str)
    {
        for (m_length = 0; m_length < N; m_length++)
        {
            T t = str[m_length];

            if (t == 0)
            {
                break;
            }

            m_str[m_length] = t;
        }

        m_str[m_length] = 0;

        return *this;
    }

    /*!
        Assigns the right hand side pgStr to this pgStr.
        @param[in] str The right hand side pgStr.
        @return A reference to this pgStr.
    */
    pgStr<T, N>& operator=(const pgStr<T, N>& str)
    {
        m_length = str.m_length;

        pgMemMgr::memcpy(m_str, str.m_str, sizeof(T) * (m_length + 1));

        return *this;
    }

    /*!
        Assigns the right hand side pgStr with different length to this pgStr.
        @tparam N2 The length of the right hand side pgStr.
        @param[in] str The right hand side pgStr with different length.
        @return A referenct to this pgStr.
    */
    template<u8 N2> pgStr<T, N>& operator=(const pgStr<T, N2>& str)
    {
        m_length = (str.getLength() < N) ? str.getLength() : N;

        pgMemMgr::memcpy(m_str, str.getString(), sizeof(T) * (m_length + 1));

        m_str[m_length] = 0;

        return *this;
    }

    /*!
        Returns whether this pgStr is equal to the right hand side character.
        @param[in] ch The right hand side character.
        @return Whether this pgStr is equal to the right hand side character.
    */
    bool operator==(T ch) const
    {
        return (*this == pgStr<T, N>(ch));
    }

    /*!
        Returns whether this pgStr is equal to the right hand side string.
        @param[in] str The right hand side string.
        @return Whether this pgStr is equal to the right hand side string.
    */
    bool operator==(const T* str) const
    {
        for (s32 i = 0; ; i++)
        {
            T t = m_str[i];

            if (str[i] != t)
            {
                return false;
            }

            if (t == 0)
            {
                return true;
            }
        }

        return false;
    }

    /*!
        Returns whether this pgStr is equal to the right hand side pgStr with different length.
        @tparam N2 The length of the right hand side pgStr.
        @param[in] str The right hand side pgStr with defferent length.
        @return Whether this pgStr is equal to the right hand side pgStr.
    */
    template<u8 N2> bool operator==(const pgStr<T, N2>& str) const
    {
        if (m_length == str.getLength())
        {
            const T* str2 = str.getString();

            for (s32 i = 0; ; i++)
            {
                T t = m_str[i];

                if (str2[i] != t)
                {
                    return false;
                }

                if (t == 0)
                {
                    return true;
                }
            }
        }

        return false;
    }

    /*!
        Returns whether this pgStr is unequal to the right hand side character.
        @param[in] ch The right hand side character.
        @return Whether this pgStr is unequal to the right hand side character.
    */
    bool operator!=(T ch) const
    {
        return !(*this == ch);
    }

    /*!
        Returns whether this pgStr is unequal to the right hand side string.
        @param[in] str The right hand side string.
        @return Whether this pgStr is unequal to the right hand side string.
    */
    bool operator!=(const T* str) const
    {
        return !(*this == str);
    }

    /*!
        Returns whether this pgStr is unequal to the right hand side pgStr with different length.
        @tparam N2 The length of the right hand side pgStr.
        @param[in] str The right hand side pgStr with defferent length.
        @return Whether this pgStr is unequal to the right hand side pgStr.
    */
    template<u8 N2> bool operator!=(const pgStr<T, N2>& str) const
    {
        return !(*this == str);
    }

    /*!
        Returns a reference to the character of this pgStr at the position specified by the parameter index.
        @param[in] index The index of the position of the element to be referenced.
        @return A reference to the character of this pgStr at the position specified by the parameter index.
    */
    T& operator[](u8 index)
    {
        if (index >= m_length)
        {
            pgThrow(ExceptionOutOfRange);
        }

        return m_str[index];
    }

    /*!
        Returns a pgStr which is the concatenation of this pgStr and the right hand side character.
        @param[in] ch The right hand side character.
        @return A pgStr which is the concatenation of this pgStr and the right hand side character.
    */
    pgStr<T, N> operator+(T ch) const
    {
        pgStr<T, N> res = *this;

        res += ch;

        return res;
    }

    /*!
        Returns a pgStr which is the concatenation of this pgStr and the right hand side string.
        @param[in] str The right hand side string.
        @return A pgStr which is the concatenation of this pgStr and the right hand side string.
    */
    pgStr<T, N> operator+(const T* str) const
    {
        pgStr<T, N> res = *this;

        res += str;

        return res;
    }

    /*!
        Returns a pgStr which is the concatenation of this pgStr and the right hand side pgStr width differenct length.
        @tparam N2 The length of the right hand side pgStr.
        @param[in] str The right hand side pgStr with different length.
        @return A pgStr which is the concatenation of this pgStr and the right hand side pgStr width differenct length.
    */
    template<u8 N2> pgStr<T, N> operator+(const pgStr<T, N2>& str) const
    {
        pgStr<T, N> res = *this;

        res += str;

        return res;
    }

    /*!
        Concatenates the right hand side character to the end of this pgStr.
        @param[in] ch The right hand side character.
    */
    void operator+=(T ch)
    {
        *this += pgStr<T, N>(ch);
    }

    /*!
        Concatenates the right hand side string to the end of this pgStr.
        @param[in] str The right hand side string.
    */
    void operator+=(const T* str)
    {
        for ( ; m_length < N; m_length++)
        {
            if (*str == 0)
            {
                break;
            }

            m_str[m_length] = *str;

            str++;
        }

        m_str[m_length] = 0;
    }

    /*!
        Concatenates the right hand side pgStr with different length to the end of this pgStr.
        @tparam N2 The length of the right hand side pgStr.
        @param[in] str The right hand side pgStr with different length.
    */
    template<u8 N2> void operator+=(const pgStr<T, N2>& str)
    {
        const T* str2 = str.getString();

        for ( ; m_length < N; m_length++)
        {
            if (*str2 == 0)
            {
                break;
            }

            m_str[m_length] = *str2;

            str2++;
        }

        m_str[m_length] = 0;
    }

    /*!
        Returns a hash value which is less than the specified value.
        This method is used when pgStr is used as the key type of pgMap.
        @param[in] n A value.
        @return A hash value.
    */
    u32 operator%(u32 n) const
    {
        u32 acc = 0;

        for (s32 i = 0; i < m_length; i++)
        {
            acc += m_str[i];
        }

        return acc % n;
    }

    /*!
        Returns the pointer to the c-string of this pgStr.
        @return The pointer to the c-string of this pgStr.
    */
    const T* getString() const
    {
        return m_str;
    }

    /*!
        Returns the length of this pgStr.
        @return The length of this pgStr.
    */
    u8 getLength() const
    {
        return m_length;
    }

    /*!
        Returns the extracted pgStr from this pgStr in the specified range.
        @param[in] index The index of the position of the element to be extracted.
        @param[in] length The length of the extracted pgStr. If not specified, the length of this pgStr is used.
        @return The extracted pgStr from this pgStr in the specified range.
    */
    pgStr<T, N> getSubStr(u8 index, u8 length = 0) const
    {
        pgStr<T, N> res;

        if (length == 0)
        {
            length = N;
        }

        for ( ; res.m_length < length; res.m_length++)
        {
            if (index >= m_length || index >= N)
            {
                break;
            }

            res.m_str[res.m_length] = m_str[index];

            index++;
        }

        res.m_str[res.m_length] = 0;

        return res;
    }

private:
    T m_str[N + 1];
    u8 m_length;
};


template<class T> class pgStr<T, 0>
{
private:
    pgStr() {}
};
