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
    @ingroup ckMath
    A 32-bit fixed point number.
*/
class CK_API ckFix
{
public:
    //! @cond
    ckDefineException(ExceptionOverflow);
    ckDefineException(ExceptionUnderflow);
    //! @endcond

    /*!
        Constructs a ckFix.
    */
    ckFix();

    /*!
        Constructs and initialize a ckFix from the specified s32 value.
        @param[in] n An s32 value.
    */
    ckFix(s32 n);

    /*!
        Constructs and initialize a ckFix from the specified r32 value.
        @param[in] r An r32 value.
    */
    ckFix(r32 r);

    /*!
        Returns whether this ckFix is equal to the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return Whether this ckFix is equal to the right hand side ckFix.
    */
    bool operator==(ckFix fix) const;

    /*!
        Returns whether this ckFix is unequal to the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return Whether this ckFix is unequal to the right hand side ckFix.
    */
    bool operator!=(ckFix fix) const;

    /*!
        Returns whether this ckFix is less than the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return Whether this ckFix is less than the ckFix.
    */
    bool operator<(ckFix fix) const;

    /*!
        Returns whether this ckFix is greater than the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return Whether this ckFix is greater than the ckFix.
    */
    bool operator>(ckFix fix) const;

    /*!
        Returns whether this ckFix is less than or equal to the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return Whether this ckFix is less than or equal to the ckFix.
    */
    bool operator<=(ckFix fix) const;

    /*!
        Returns whether this ckFix is greater than or equal to the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return Whether this ckFix is greater than or equal to the ckFix.
    */
    bool operator>=(ckFix fix) const;

    /*!
        Returns a ckFix which is the sum of this ckFix and the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the sum of two ckFixes.
    */
    ckFix operator+(ckFix fix) const;

    /*!
        Returns a ckFix which is the sum of the left hand side s32 value and the right hand side ckFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side ckFix.
        @return a ckFix which is the sum of the left hand side s32 value and the right hand side ckFix.
    */
    CK_API friend ckFix operator+(s32 n, ckFix fix);

    /*!
        Returns a ckFix which is the sum of the left hand side r32 value and the right hand side ckFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side ckFix.
        @return a ckFix which is the sum of the left hand side r32 value and the right hand side ckFix.
    */
    CK_API friend ckFix operator+(r32 r, ckFix fix);

    /*!
        Adds the right hand side ckFix to this ckFix.
        @param[in] fix The right hand side ckFix.
    */
    void operator+=(ckFix fix);

    /*!
        Returns a ckFix which is the negative of this ckFix.
        @return A ckFix which is the negative of this ckFix.
    */
    ckFix operator-() const;

    /*!
        Returns a ckFix which is the right hand side ckFix subtracted from this ckFix.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the right hand side ckFix subtracted from this ckFix.
    */
    ckFix operator-(ckFix fix) const;

    /*!
        Returns a ckFix which is the right hand side ckFix subtracted from the left hand side s32 value.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the right hand side ckFix subtracted from the left hand side s32 value.
    */
    CK_API friend ckFix operator-(s32 n, ckFix fix);

    /*!
        Returns a ckFix which is the right hand side ckFix subtracted from the left hand side r32 value.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the right hand side ckFix subtracted from the left hand side r32 value.
    */
    CK_API friend ckFix operator-(r32 r, ckFix fix);

    /*!
        Subtracts the right hand side ckFix from this ckFix.
        @param[in] fix The right hand side ckFix.
    */
    void operator-=(ckFix fix);

    /*!
        Returns a ckFix which is this ckFix multiplied with the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is this ckFix multiplied with the right hand side ckFix.
    */
    ckFix operator*(ckFix fix) const;

    /*!
        Returns a ckFix which is the left hand side s32 value multiplied with the right hand side ckFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the left hand side s32 value multiplied with the right hand side ckFix.
    */
    CK_API friend ckFix operator*(s32 n, ckFix fix);

    /*!
        Returns a ckFix which is the left hand side r32 value multiplied with the right hand side ckFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the left hand side r32 value multiplied with the right hand side ckFix.
    */
    CK_API friend ckFix operator*(r32 r, ckFix fix);

    /*!
        Multiplies this ckFix with the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
    */
    void operator*=(ckFix fix);

    /*!
        Returns a ckFix which is this ckFix divided by the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
        @return A ckVec which is this ckFix divided by the right hand side ckFix.
    */
    ckFix operator/(ckFix fix) const;

    /*!
        Returns a ckFix which is the left hand side s32 value divided by the right hand side ckFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the left hand side s32 value devided by the right hand side ckFix.
    */
    CK_API friend ckFix operator/(s32 n, ckFix fix);

    /*!
        Returns a ckFix which is the left hand side r32 value divided by the right hand side ckFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side ckFix.
        @return A ckFix which is the left hand side r32 value divided by the right hand side ckFix.
    */
    CK_API friend ckFix operator/(r32 r, ckFix fix);

    /*!
        Divides this ckFix by the right hand side ckFix.
        @param[in] fix The right hand side ckFix.
    */
    void operator/=(ckFix fix);

    /*!
        Returns an s32 value which is converted from this ckFix.
        @return An s32 value which is converted from this ckFix.
    */
    s32 toS32() const;

    /*!
        Returns an r32 value which is converted from this ckFix.
        @return An r32 value which is converted from this ckFix.
    */
    r32 toR32() const;

    /*!
        Returns the value of this ckFix in the internal format.
        @return The value of thie ckFix in the internal format.
    */
    s32 getValue() const;

    /*!
        Returns a ckFix which is constructed from the specified value in the internal format.
        @param[in] value A value in the internal format.
        @return A ckFix which is constructed from the specified value in the internal format.
    */
    static ckFix fromValue(s32 value);

private:
    static const u32 BIT_NUM = 32;
    static const u32 FRACTION_BIT_NUM = 16;

    static void checkOverflow(s64 value);

    s32 m_value;
};
