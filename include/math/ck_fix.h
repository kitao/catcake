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
    @ingroup pgMath
    A 32-bit fixed point number.
*/
class PG_API pgFix
{
public:
    pgDefineException(ExceptionOverflow);
    pgDefineException(ExceptionUnderflow);

    /*!
        Constructs a pgFix.
    */
    pgFix();

    /*!
        Constructs and initialize a pgFix from the specified s32 value.
        @param[in] n An s32 value.
    */
    pgFix(s32 n);

    /*!
        Constructs and initialize a pgFix from the specified r32 value.
        @param[in] r An r32 value.
    */
    pgFix(r32 r);

    /*!
        Returns whether this pgFix is equal to the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return Whether this pgFix is equal to the right hand side pgFix.
    */
    bool operator==(pgFix fix) const;

    /*!
        Returns whether this pgFix is unequal to the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return Whether this pgFix is unequal to the right hand side pgFix.
    */
    bool operator!=(pgFix fix) const;

    /*!
        Returns whether this pgFix is less than the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return Whether this pgFix is less than the pgFix.
    */
    bool operator<(pgFix fix) const;

    /*!
        Returns whether this pgFix is greater than the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return Whether this pgFix is greater than the pgFix.
    */
    bool operator>(pgFix fix) const;

    /*!
        Returns whether this pgFix is less than or equal to the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return Whether this pgFix is less than or equal to the pgFix.
    */
    bool operator<=(pgFix fix) const;

    /*!
        Returns whether this pgFix is greater than or equal to the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return Whether this pgFix is greater than or equal to the pgFix.
    */
    bool operator>=(pgFix fix) const;

    /*!
        Returns a pgFix which is the sum of this pgFix and the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the sum of two pgFixes.
    */
    pgFix operator+(pgFix fix) const;

    /*!
        Returns a pgFix which is the sum of the left hand side s32 value and the right hand side pgFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side pgFix.
        @return a pgFix which is the sum of the left hand side s32 value and the right hand side pgFix.
    */
    PG_API friend pgFix operator+(s32 n, pgFix fix);

    /*!
        Returns a pgFix which is the sum of the left hand side r32 value and the right hand side pgFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side pgFix.
        @return a pgFix which is the sum of the left hand side r32 value and the right hand side pgFix.
    */
    PG_API friend pgFix operator+(r32 r, pgFix fix);

    /*!
        Adds the right hand side pgFix to this pgFix.
        @param[in] fix The right hand side pgFix.
    */
    void operator+=(pgFix fix);

    /*!
        Returns a pgFix which is the negative of this pgFix.
        @return A pgFix which is the negative of this pgFix.
    */
    pgFix operator-() const;

    /*!
        Returns a pgFix which is the right hand side pgFix subtracted from this pgFix.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the right hand side pgFix subtracted from this pgFix.
    */
    pgFix operator-(pgFix fix) const;

    /*!
        Returns a pgFix which is the right hand side pgFix subtracted from the left hand side s32 value.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the right hand side pgFix subtracted from the left hand side s32 value.
    */
    PG_API friend pgFix operator-(s32 n, pgFix fix);

    /*!
        Returns a pgFix which is the right hand side pgFix subtracted from the left hand side r32 value.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the right hand side pgFix subtracted from the left hand side r32 value.
    */
    PG_API friend pgFix operator-(r32 r, pgFix fix);

    /*!
        Subtracts the right hand side pgFix from this pgFix.
        @param[in] fix The right hand side pgFix.
    */
    void operator-=(pgFix fix);

    /*!
        Returns a pgFix which is this pgFix multiplied with the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is this pgFix multiplied with the right hand side pgFix.
    */
    pgFix operator*(pgFix fix) const;

    /*!
        Returns a pgFix which is the left hand side s32 value multiplied with the right hand side pgFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the left hand side s32 value multiplied with the right hand side pgFix.
    */
    PG_API friend pgFix operator*(s32 n, pgFix fix);

    /*!
        Returns a pgFix which is the left hand side r32 value multiplied with the right hand side pgFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the left hand side r32 value multiplied with the right hand side pgFix.
    */
    PG_API friend pgFix operator*(r32 r, pgFix fix);

    /*!
        Multiplies this pgFix with the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
    */
    void operator*=(pgFix fix);

    /*!
        Returns a pgFix which is this pgFix divided by the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
        @return A pgVec which is this pgFix divided by the right hand side pgFix.
    */
    pgFix operator/(pgFix fix) const;

    /*!
        Returns a pgFix which is the left hand side s32 value divided by the right hand side pgFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the left hand side s32 value devided by the right hand side pgFix.
    */
    PG_API friend pgFix operator/(s32 n, pgFix fix);

    /*!
        Returns a pgFix which is the left hand side r32 value divided by the right hand side pgFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side pgFix.
        @return A pgFix which is the left hand side r32 value divided by the right hand side pgFix.
    */
    PG_API friend pgFix operator/(r32 r, pgFix fix);

    /*!
        Divides this pgFix by the right hand side pgFix.
        @param[in] fix The right hand side pgFix.
    */
    void operator/=(pgFix fix);

    /*!
        Returns an s32 value which is converted from this pgFix.
        @return An s32 value which is converted from this pgFix.
    */
    s32 toS32() const;

    /*!
        Returns an r32 value which is converted from this pgFix.
        @return An r32 value which is converted from this pgFix.
    */
    r32 toR32() const;

    /*!
        Returns the value of this pgFix in the internal format.
        @return The value of thie pgFix in the internal format.
    */
    s32 getValue() const;

    /*!
        Returns a pgFix which is constructed from the specified value in the internal format.
        @param[in] value A value in the internal format.
        @return A pgFix which is constructed from the specified value in the internal format.
    */
    static pgFix fromValue(s32 value);

private:
    static const u32 BIT_NUM = 32;
    static const u32 FRACTION_BIT_NUM = 16;

    static void checkOverflow(s64 value);

    s32 m_value;
};
