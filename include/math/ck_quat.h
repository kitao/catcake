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
    A quaternion.
*/
struct CK_API ckQuat
{
    r32 x; //!< The x-coordinate.
    r32 y; //!< The y-coordinate.
    r32 z; //!< The z-coordinate.
    r32 w; //!< The w-coordinate.

    /*!
        Constructs a ckQuat.
    */
    ckQuat();

    /*!
        Constructs and initialize a ckQaut from the specified xyzw coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate.
        @param[in] w_ The w-coordinate.
    */
    ckQuat(r32 x_, r32 y_, r32 z_, r32 w_);

    /*!
        Sets all components from the specified xyzw coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate.
        @param[in] w_ The w-coordinate.
    */
    void set(r32 x_, r32 y_, r32 z_, r32 w_);

    /*!
        Returns a ckQuat which is interpolated between this ckQuat and an another ckQuat by the specified ratio, using spherical linear interpolation.
        @param[in] to An another ckQuat.
        @param[in] ratio The value which indicates how far to interpolate between two ckQuats. This value is clamped between 0.0f and 1.0f.
        @return A ckQuat which is interpolated between two ckQuats.
    */
    ckQuat slerp(const ckQuat& to, r32 ratio) const;

    /*!
        Returns a ckMat which is converted from the axes of this ckQuat and the translation of the specified ckVec.
        @param[in] trans The translation.
        @return A ckMat which is converted from the axes of this ckQuat and the translation of the specified ckVec.
    */
    ckMat toMat(const ckVec& trans) const;

    /*!
        Returns a ckQuat which is converted from the specified ckMat.
        @param[in] mat A ckMat, whose translation is ignored.
        @return A ckQuat which is converted from the specified ckMat.
    */
    static ckQuat fromMat(const ckMat& mat);
};
