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


struct ckMat;


/*!
    @ingroup ckMath
    A 3-element vector which is represented by xyz coordinates.
*/
struct CK_API ckVec
{
    static const ckVec ZERO; //!< The zero vector.
    static const ckVec X_UNIT; //!< The orthonormal x-axis.
    static const ckVec Y_UNIT; //!< The orthonormal y-axis.
    static const ckVec Z_UNIT; //!< The orthonormal z-axis.

    r32 x; //!< The x-coordinate.
    r32 y; //!< The y-coordinate.
    r32 z; //!< The z-coordinate.

    /*!
        Constructs a ckVec.
    */
    ckVec();

    /*!
        Constructs and initializes a ckVec from the specified xyz coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate. If not specified, 0.0f is used.
    */
    ckVec(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        Sets all components from the specified xyz coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate. If not specified, 0.0f is used.
    */
    void set(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        Returns a ckVec which is the sum of this ckVec and the right hand side ckVec.
        @param[in] vec The right hand side ckVec.
        @return A ckVec which is the sum of two ckVecs.
    */
    ckVec operator+(const ckVec& vec) const;

    /*!
        Adds the right hand side ckVec to this ckVec.
        @param[in] vec The right hand side ckVec.
    */
    void operator+=(const ckVec& vec);

    /*!
        Returns a ckVec which is the negative of this ckVec.
        @return A ckVec which is the negative of this ckVec.
    */
    ckVec operator-() const;

    /*!
        Returns a ckVec which is the right hand side ckVec subtracted from this ckVec.
        @param[in] vec The right hand side ckVec.
        @return A ckVec which is the right hand side ckVec subtracted from this ckVec.
    */
    ckVec operator-(const ckVec& vec) const;

    /*!
        Subtracts the right hand side ckVec from this ckVec.
        @param[in] vec The right hand side ckVec.
    */
    void operator-=(const ckVec& vec);

    /*!
        Returns a ckVec which is this ckVec multiplied with the right hand side scalar.
        @param[in] s The right hand side scalar.
        @return A ckVec which is this ckVec multiplied with the right hand side scalar.
    */
    ckVec operator*(r32 s) const;

    /*!
        Returns a ckVec which is the right hand side ckVec multiplied with the left hand side scalar.
        @param[in] s The left hand side scalar.
        @param[in] vec The right hand side ckVec.
        @return A ckVec which is the right hand side ckVec multiplied with the left hand side scalar.
    */
    CK_API friend ckVec operator*(r32 s, const ckVec& vec);

    /*!
        Multiplies this ckVec with the right hand side scalar.
        @param[in] s The right hand side scalar.
    */
    void operator*=(r32 s);

    /*!
        Returns a ckVec which is this ckVec divided by the right hand side scalar.
        @param[in] s The right hand side scalar.
        @return A ckVec which is this ckVec divided by the right hand side scalar.
    */
    ckVec operator/(r32 s) const;

    /*!
        Divides this ckVec by the right hand side scalar.
        @param[in] s The right hand side scalar.
    */
    void operator/=(r32 s);

    /*!
        Returns the length of this ckVec.
        @return The length of this ckVec.
    */
    r32 length() const;

    /*!
        Returns the squared length of this ckVec.
        @return The squared length of this ckVec.
    */
    r32 sqLength() const;

    /*!
        Returns the distance between this ckVec and an another ckVec.
        @param[in] vec An another ckVec.
        @return The distance between two ckVecs.
    */
    r32 dist(const ckVec& vec) const;

    /*!
        Returns the squared distance between this ckVec and an another ckVec.
        @param[in] vec An another ckVec.
        @return The squared distance between two ckVecs.
    */
    r32 sqDist(const ckVec& vec) const;

    /*!
        Returns the inner product of this ckVec and an another ckVec.
        @param[in] vec An another ckVec.
        @return The inner product of two ckVecs.
    */
    r32 dot(const ckVec& vec) const;

    /*!
        Returns the outer product of this ckVec and an another ckVec.
        @param[in] vec An another ckVec.
        @return The outer product of two ckVecs.
    */
    ckVec cross(const ckVec& vec) const;

    /*!
        Returns a ckVec which is this ckVec rotated around the orthonormal x-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A ckVec which is this ckVec rotated around the orthonormal x-axis.
    */
    ckVec rotateX_r32(r32 deg) const;

    /*!
        Returns a ckVec which is this ckVec rotated around the orthonormal y-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A ckVec which is this ckVec rotated around the orthonormal y-axis.
    */
    ckVec rotateY_r32(r32 deg) const;

    /*!
        Returns a ckVec which is this ckVec rotated around the orthonormal z-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A ckVec which is this ckVec rotated around the orthonormal z-axis.
    */
    ckVec rotateZ_r32(r32 deg) const;

    /*!
        Returns a ckVec which is this ckVec rotated around the orthonormal x-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A ckVec which is this ckVec rotated around the orthonormal x-axis.
    */
    ckVec rotateX_s32(s32 deg) const;

    /*!
        Returns a ckVec which is this ckVec rotated around the orthonormal y-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A ckVec which is this ckVec rotated around the orthonormal y-axis.
    */
    ckVec rotateY_s32(s32 deg) const;

    /*!
        Returns a ckVec which is this ckVec rotated around the orthonormal z-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A ckVec which is this ckVec rotated around the orthonormal z-axis.
    */
    ckVec rotateZ_s32(s32 deg) const;

    /*!
        Returns a ckVec which is the normalized this ckVec.
        @return A ckVec which is the normalized this ckVec.
    */
    ckVec normalize() const;

    /*!
        Returns a ckVec which is interpolated between this ckVec and an another ckVec by the specified ratio, using linear interpolation.
        @param[in] to An another ckVec.
        @param[in] ratio The value which indicates how far to interpolate between two ckVecs. This value is clamped between 0.0f and 1.0f.
        @return A ckVec which is interpolated between two ckVecs.
    */
    ckVec interp(const ckVec& to, r32 ratio) const;

    /*!
        Returns a ckVec which is interpolated between this ckVec and an another ckVec by the specified ratio, using spherical linear interpolation.
        @param[in] to An another ckVec.
        @param[in] ratio The value which indicates how far to interpolate between two ckVecs. This value is clamped between 0.0f and 1.0f.
        @return A ckVec which is interpolated between two ckVecs.
    */
    ckVec slerp(const ckVec& to, r32 ratio) const;

    /*!
        Returns a ckVec which is this ckVec converted from in the world coordinate system to in the local coordinate system of the specified ckMat.
        @param[in] mat The origin ckMat of the local coordinate system.
        @return A ckVec which is this ckVec in the local coordinate system.
    */
    ckVec toLocalOf(const ckMat& mat) const;

    /*!
        Returns a ckVec which is this ckVec converted from in the local coordinate system of the specified ckMat to in the world coordinate system.
        @param[in] mat The origin ckMat of the local coordinate system.
        @return A ckVec which is this ckVec in the world coordinate system.
    */
    ckVec toGlobalFrom(const ckMat& mat) const;

    /*!
        Returns a ckVec which is this ckVec converted from in the world coordinate system to
        in the local coordinate system of the specified ckMat whose translation is ignored.
        @param[in] mat The origin ckMat of the local coordinate system, whose translation is ignored.
        @return A ckVec which is this ckVec in the local coordinate system.
    */
    ckVec toLocalOf_noTrans(const ckMat& mat) const;

    /*!
        Returns a ckVec which is this ckVec converted from in the local coordinate system of
        the specified ckMat whose translation is ignored to in the world coordinate system.
        @param[in] mat The origin ckMat of the local coordinate system, whose translation is ignored.
        @return A ckVec which is this ckVec in the world coordinate system.
    */
    ckVec toGlobalFrom_noTrans(const ckMat& mat) const;

    /*!
        Converts this ckVec to a 4-element array.
        @param[out] r32x4 A destination of the converted 4-element array.
        @param[in] w The 4th coordinate of this ckVec.
    */
    void toR32x4(r32* r32x4, r32 w) const;

    /*!
        Returns a ckVec which is converted from a 4-element array.
        @param[in] r32x4 A 4-element array to be converted.
        @return A ckVec which is converted from the 4-element array.
    */
    static ckVec fromR32x4(const r32* r32x4);

    /*!
        Multiplies a 16-element array by a 4-element array as a matrix and a vector respectively.
        @param[out] res_r32x4 The destination of the multiplied 4-element array.
        @param[in] lhs_r32x16 The left hand side 16-element array.
        @param[in] rhs_r32x4 The right hand side 4-element array.
    */
    static void mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4);
};
