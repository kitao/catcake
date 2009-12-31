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
