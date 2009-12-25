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
    @ingroup ckGen
    An enum type with specified size.
    @tparam D The data type to store a value of the enum type.
    @tparam T The enum type stored in the data type.
*/
template<class D, class T> class ckType
{
public:
    /*!
        Constructs and initializes a ckType to zero.
    */
    ckType()
    {
        m_type = 0;
    }

    /*!
        Constructs and initializes a ckType from the specified type.
        @param[in] type A type.
    */
    ckType(T type)
    {
        m_type = static_cast<D>(type);
    }

    /*!
        Assigns the right hand side type to this ckType.
        @param[in] type The right hand side type.
        @return A reference to this ckType.
    */
    ckType<D, T>& operator=(T type)
    {
        m_type = static_cast<D>(type);

        return *this;
    }

    /*!
        Returns whether this ckType is equal to the right hand side ckType.
        @param[in] type The right hand side ckType.
        @return Whether this ckType is equal to the right hand side ckType.
    */
    bool operator==(ckType<D, T> type) const
    {
        return (getType() == type.getType());
    }

    /*!
        Returns whether this ckType is equal to the right hand side type.
        @param[in] type The right hand side type.
        @return Whether this ckType is equal to the right hand side type.
    */
    bool operator==(T type) const
    {
        return (getType() == type);
    }

    /*!
        Returns whether the left hand side type is equal to the right hand side ckType.
        @param[in] type1 The left hand side type.
        @param[in] type2 The right hand side ckType.
        @return Whether the left hand side type is equal to the right hand side ckType.
    */
    friend bool operator==(T type1, ckType<D, T> type2)
    {
        return (type1 == type2.getType());
    }

    /*!
        Returns whether this ckType is unequal to the right hand side ckType.
        @param[in] type The right hand side ckType.
        @return Whether this ckType is unequal to the right hand side ckType.
    */
    bool operator!=(ckType<D, T> type) const
    {
        return (getType() != type.getType());
    }

    /*!
        Returns whether this ckType is unequal to the right hand side type.
        @param[in] type The right hand side type.
        @return Whether this ckType is unequal to the right hand side type.
    */
    bool operator!=(T type) const
    {
        return (getType() != type);
    }

    /*!
        Returns whether the left hand side type is unequal to the right hand side ckType.
        @param[in] type1 The left hand side type.
        @param[in] type2 The right hand side ckType.
        @return Whether the left hand side type is unequal to the right hand side ckType.
    */
    friend bool operator!=(T type1, ckType<D, T> type2)
    {
        return (type1 != type2.getType());
    }

    /*!
        Returns the value of this ckType as the enum type.
        @return The value of this ckType as the enum type.
    */
    T getType() const
    {
        return static_cast<T>(m_type);
    }

    /*!
        Returns the value of this ckType as the data type.
        @return The value of this ckType as the data type.
    */
    D getValue() const
    {
        return m_type;
    }

private:
    D m_type;
};


//! @cond
template<class D> class ckType<D, bool>
{
public:
    ckType()
    {
        m_type = 0;
    }

    ckType(bool type)
    {
        m_type = static_cast<D>(type);
    }

    ckType<D, bool>& operator=(bool type)
    {
        m_type = static_cast<D>(type);

        return *this;
    }

    bool operator==(ckType<D, bool> type) const
    {
        return (getType() == type.getType());
    }

    bool operator==(bool type) const
    {
        return (getType() == type);
    }

    friend bool operator==(bool type1, ckType<D, bool> type2)
    {
        return (type1 == type2.getType());
    }

    bool operator!=(ckType<D, bool> type) const
    {
        return (getType() != type.getType());
    }

    bool operator!=(bool type) const
    {
        return (getType() != type);
    }

    friend bool operator!=(bool type1, ckType<D, bool> type2)
    {
        return (type1 != type2.getType());
    }

    bool getType() const
    {
        return m_type ? true : false;
    }

    D getValue() const
    {
        return m_type;
    }

private:
    D m_type;
};
//! @endcond
