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
