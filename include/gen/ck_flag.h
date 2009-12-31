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
    A set of bits as a data structure.
    @tparam D The data type to store flag bits.
    @tparam T The enum type whose constant values mean bit number. The each value of the enum constants must be less than the number of bits of the data type.
*/
template<class D, class T> class ckFlag
{
public:
    /*!
        Constructs and initializes a ckFlag whose all bits are off.
    */
    ckFlag()
    {
        clear();
    }

    /*!
        Returns whether this ckFlag is equal to the right hand side ckFlag.
        @param[in] flag The right hand side ckFlag.
        @return Whether this ckFlag is equal to the right hand side ckFlag.
    */
    bool operator==(ckFlag<D, T> flag) const
    {
        return (m_flag == flag.m_flag);
    }

    /*!
        Returns whether this ckFlag is unequal to the right hand side ckFlag.
        @param[in] flag The right hand side ckFlag.
        @return Whether this ckFlag is unequal to the right hand side ckFlag.
    */
    bool operator!=(ckFlag<D, T> flag) const
    {
        return (m_flag != flag.m_flag);
    }

    /*!
        Returns the bits of this ckFlag as the data type.
        @return The bits of this ckFlag as the data type.
    */
    D getValue() const
    {
        return m_flag;
    }

    /*!
        Returns whether the specified flag is on.
        @param[in] flag A flag which is a value of the enum constants.
        @return Whether the specified flag is on.
    */
    bool isOn(T flag) const
    {
        return (m_flag & (1 << flag)) ? true : false;
    }

    /*!
        Returns whether the specified flag is off.
        @param[in] flag A flag which is a value of the enum constants.
        @return Whether the specified flag is off.
    */
    bool isOff(T flag) const
    {
        return (m_flag & (1 << flag)) ? false : true;
    }

    /*!
        Sets the specified flag on.
        @param[in] flag A flag which is a value of the enum constants.
    */
    void setOn(T flag)
    {
        m_flag |= (1 << flag);
    }

    /*!
        Sets the specified flag off.
        @param[in] flag A flag which is a value of the enum constants.
    */
    void setOff(T flag)
    {
        m_flag &= ~(1 << flag);
    }

    /*!
        Sets the specified flag to the specified value.
        @param[in] flag A flag which is a value of the enum constants.
        @param[in] is_on If true, the flag is set on. Otherwise, the flag is set off.
    */
    void set(T flag, bool is_on)
    {
        if (is_on)
        {
            setOn(flag);
        }
        else
        {
            setOff(flag);
        }
    }

    /*!
        Sets all bits of this ckFlag off.
    */
    void clear()
    {
        m_flag = 0;
    }

private:
    D m_flag;
};
