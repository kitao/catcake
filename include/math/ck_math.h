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
    Provides mathematical functions.
*/
class CK_API ckMath
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
