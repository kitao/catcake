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
    Provides mathematical functions.
*/
class PG_API pgMath
{
public:
    static const r32 EPSILON; //!< If the difference of two numbers is less than this value, two numbers are considered equal.
    static const r32 PI; //!< Pi.
    static const r32 DEG_TO_RAD; //!< Is equal to Pi/180.
    static const r32 RAD_TO_DEG; //!< Is equal to 180/Pi.

    /*!
        Returns the absolute value of a value.
        @tparam T The type of a value.
        @param[in] x A value.
        @return The absolute value of the value.
    */
    template<class T> static T abs(T x)
    {
        return (x >= 0) ? x : -x;
    }

    /*!
        Returns the minimum of two values.
        @tparam T The type of two values.
        @param[in] a A value.
        @param[in] b An another value.
        @return The minimum of two values.
    */
    template<class T> static T min(T a, T b)
    {
        return (a <= b) ? a : b;
    }

    /*!
        Returns the maximum of two values.
        @tparam T The type of two values.
        @param[in] a A value.
        @param[in] b An another value.
        @return The maximum of two values.
    */
    template<class T> static T max(T a, T b)
    {
        return (a >= b) ? a : b;
    }

    /*!
        Returns the clamped value between two thresholds.
        @tparam T The type of a value and thresholds.
        @param[in] x A value.
        @param[in] min The lower clamp threshold.
        @param[in] max The upper clamp threshold.
        @return The clamped value between two thresholds.
    */
    template<class T> static T clamp(T x, T min, T max)
    {
        return (x < min) ? min : ((x > max) ? max : x);
    }

    /*!
        Returns the square root of a value.
        @param[in] x A value. This value must be greater than or equal to zero.
        @return The square root of the value.
    */
    static r32 sqrt(r32 x);

    /*!
        Returns the trigonometric sine of an r32 angle.
        @param[in] deg An r32 angle in degrees.
        @return The sine of the r32 angle.
    */
    static r32 sin_r32(r32 deg);

    /*!
        Returns the trigonometric cosine of an r32 angle.
        @param[in] deg An r32 angle in degrees.
        @return The cosine of the r32 angle.
    */
    static r32 cos_r32(r32 deg);

    /*!
        Returns the trigonometric sine of an s32 angle.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle in degrees.
        @return The sine of the s32 angle.
    */
    static r32 sin_s32(s32 deg);

    /*!
        Returns the trigonometric cosine of an s32 angle.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle in degrees.
        @return The cosine of the s32 angle.
    */
    static r32 cos_s32(s32 deg);

    /*!
        Returns the arc sine of a value.
        @param[in] x A value.
        @return The arc sine of the value, expressed in degrees.
    */
    static r32 asin(r32 x);

    /*!
        Returns the arc cosine of a value.
        @param[in] x A value.
        @return The arc cosine of the value, expressed in degrees.
    */
    static r32 acos(r32 x);

    /*!
        Returns the arc tangent of y/x, using the signs of both arguments to determine the quadrant of the return value.
        @param[in] y The y-coordinate.
        @param[in] x The x-coordinate.
        @return The arc tangent of y/x, expressed in degrees.
    */
    static r32 atan2(r32 y, r32 x);

    /*!
        Sets the seed of the random number generator.
        @param[in] seed The initial seed.
    */
    static void srand(u32 seed);

    /*!
        Returns a pseudo-random number between two specified values.
        @param[in] from A range of the generated numbers.
        @param[in] to An another range of the generated numbers.
        @return A pseudo-random number between two specified values.
    */
    static s32 rand(s32 from, s32 to);

    /*!
        Returns a pseudo-random number between two specified values with the specified interval.
        @param[in] from A range of the generated numbers.
        @param[in] to An another range of the generated numbers. This value may be changed depending on the specified interval.
        @param[in] interval The interval of the generated numbers.
        @return A pseudo-random number between two specified values with the specified interval.
    */
    static r32 rand(r32 from, r32 to, r32 interval);

    /*!
        Returns the interpolated value between two values by the specified ratio.
        @param[in] from The source value.
        @param[in] to The destination value.
        @param[in] ratio The value which indicates how far to interpolate between two values. This value is clamped between 0.0f and 1.0f.
        @return The interpolated value between two values by the specified ratio.
    */
    static r32 interp(r32 from, r32 to, r32 ratio);
};
