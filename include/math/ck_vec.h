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


struct pgMat;


/*!
    @ingroup pgMath
    A 3-element vector which is represented by xyz coordinates.
*/
struct PG_API pgVec
{
    static const pgVec ZERO; //!< The zero vector.
    static const pgVec X_UNIT; //!< The orthonormal x-axis.
    static const pgVec Y_UNIT; //!< The orthonormal y-axis.
    static const pgVec Z_UNIT; //!< The orthonormal z-axis.

    r32 x; //!< The x-coordinate.
    r32 y; //!< The y-coordinate.
    r32 z; //!< The z-coordinate.

    /*!
        Constructs a pgVec.
    */
    pgVec();

    /*!
        Constructs and initializes a pgVec from the specified xyz coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate. If not specified, 0.0f is used.
    */
    pgVec(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        Sets all components from the specified xyz coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate. If not specified, 0.0f is used.
    */
    void set(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        Returns a pgVec which is the sum of this pgVec and the right hand side pgVec.
        @param[in] vec The right hand side pgVec.
        @return A pgVec which is the sum of two pgVecs.
    */
    pgVec operator+(const pgVec& vec) const;

    /*!
        Adds the right hand side pgVec to this pgVec.
        @param[in] vec The right hand side pgVec.
    */
    void operator+=(const pgVec& vec);

    /*!
        Returns a pgVec which is the negative of this pgVec.
        @return A pgVec which is the negative of this pgVec.
    */
    pgVec operator-() const;

    /*!
        Returns a pgVec which is the right hand side pgVec subtracted from this pgVec.
        @param[in] vec The right hand side pgVec.
        @return A pgVec which is the right hand side pgVec subtracted from this pgVec.
    */
    pgVec operator-(const pgVec& vec) const;

    /*!
        Subtracts the right hand side pgVec from this pgVec.
        @param[in] vec The right hand side pgVec.
    */
    void operator-=(const pgVec& vec);

    /*!
        Returns a pgVec which is this pgVec multiplied with the right hand side scalar.
        @param[in] s The right hand side scalar.
        @return A pgVec which is this pgVec multiplied with the right hand side scalar.
    */
    pgVec operator*(r32 s) const;

    /*!
        Returns a pgVec which is the right hand side pgVec multiplied with the left hand side scalar.
        @param[in] s The left hand side scalar.
        @param[in] vec The right hand side pgVec.
        @return A pgVec which is the right hand side pgVec multiplied with the left hand side scalar.
    */
    PG_API friend pgVec operator*(r32 s, const pgVec& vec);

    /*!
        Multiplies this pgVec with the right hand side scalar.
        @param[in] s The right hand side scalar.
    */
    void operator*=(r32 s);

    /*!
        Returns a pgVec which is this pgVec divided by the right hand side scalar.
        @param[in] s The right hand side scalar.
        @return A pgVec which is this pgVec divided by the right hand side scalar.
    */
    pgVec operator/(r32 s) const;

    /*!
        Divides this pgVec by the right hand side scalar.
        @param[in] s The right hand side scalar.
    */
    void operator/=(r32 s);

    /*!
        Returns the length of this pgVec.
        @return The length of this pgVec.
    */
    r32 length() const;

    /*!
        Returns the squared length of this pgVec.
        @return The squared length of this pgVec.
    */
    r32 sqLength() const;

    /*!
        Returns the distance between this pgVec and an another pgVec.
        @param[in] vec An another pgVec.
        @return The distance between two pgVecs.
    */
    r32 dist(const pgVec& vec) const;

    /*!
        Returns the squared distance between this pgVec and an another pgVec.
        @param[in] vec An another pgVec.
        @return The squared distance between two pgVecs.
    */
    r32 sqDist(const pgVec& vec) const;

    /*!
        Returns the inner product of this pgVec and an another pgVec.
        @param[in] vec An another pgVec.
        @return The inner product of two pgVecs.
    */
    r32 dot(const pgVec& vec) const;

    /*!
        Returns the outer product of this pgVec and an another pgVec.
        @param[in] vec An another pgVec.
        @return The outer product of two pgVecs.
    */
    pgVec cross(const pgVec& vec) const;

    /*!
        Returns a pgVec which is this pgVec rotated around the orthonormal x-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A pgVec which is this pgVec rotated around the orthonormal x-axis.
    */
    pgVec rotateX_r32(r32 deg) const;

    /*!
        Returns a pgVec which is this pgVec rotated around the orthonormal y-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A pgVec which is this pgVec rotated around the orthonormal y-axis.
    */
    pgVec rotateY_r32(r32 deg) const;

    /*!
        Returns a pgVec which is this pgVec rotated around the orthonormal z-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A pgVec which is this pgVec rotated around the orthonormal z-axis.
    */
    pgVec rotateZ_r32(r32 deg) const;

    /*!
        Returns a pgVec which is this pgVec rotated around the orthonormal x-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A pgVec which is this pgVec rotated around the orthonormal x-axis.
    */
    pgVec rotateX_s32(s32 deg) const;

    /*!
        Returns a pgVec which is this pgVec rotated around the orthonormal y-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A pgVec which is this pgVec rotated around the orthonormal y-axis.
    */
    pgVec rotateY_s32(s32 deg) const;

    /*!
        Returns a pgVec which is this pgVec rotated around the orthonormal z-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A pgVec which is this pgVec rotated around the orthonormal z-axis.
    */
    pgVec rotateZ_s32(s32 deg) const;

    /*!
        Returns a pgVec which is the normalized this pgVec.
        @return A pgVec which is the normalized this pgVec.
    */
    pgVec normalize() const;

    /*!
        Returns a pgVec which is interpolated between this pgVec and an another pgVec by the specified ratio, using linear interpolation.
        @param[in] to An another pgVec.
        @param[in] ratio The value which indicates how far to interpolate between two pgVecs. This value is clamped between 0.0f and 1.0f.
        @return A pgVec which is interpolated between two pgVecs.
    */
    pgVec interp(const pgVec& to, r32 ratio) const;

    /*!
        Returns a pgVec which is interpolated between this pgVec and an another pgVec by the specified ratio, using spherical linear interpolation.
        @param[in] to An another pgVec.
        @param[in] ratio The value which indicates how far to interpolate between two pgVecs. This value is clamped between 0.0f and 1.0f.
        @return A pgVec which is interpolated between two pgVecs.
    */
    pgVec slerp(const pgVec& to, r32 ratio) const;

    /*!
        Returns a pgVec which is this pgVec converted from in the world coordinate system to in the local coordinate system of the specified pgMat.
        @param[in] mat The origin pgMat of the local coordinate system.
        @return A pgVec which is this pgVec in the local coordinate system.
    */
    pgVec toLocalOf(const pgMat& mat) const;

    /*!
        Returns a pgVec which is this pgVec converted from in the local coordinate system of the specified pgMat to in the world coordinate system.
        @param[in] mat The origin pgMat of the local coordinate system.
        @return A pgVec which is this pgVec in the world coordinate system.
    */
    pgVec toGlobalFrom(const pgMat& mat) const;

    /*!
        Returns a pgVec which is this pgVec converted from in the world coordinate system to
        in the local coordinate system of the specified pgMat whose translation is ignored.
        @param[in] mat The origin pgMat of the local coordinate system, whose translation is ignored.
        @return A pgVec which is this pgVec in the local coordinate system.
    */
    pgVec toLocalOf_noTrans(const pgMat& mat) const;

    /*!
        Returns a pgVec which is this pgVec converted from in the local coordinate system of
        the specified pgMat whose translation is ignored to in the world coordinate system.
        @param[in] mat The origin pgMat of the local coordinate system, whose translation is ignored.
        @return A pgVec which is this pgVec in the world coordinate system.
    */
    pgVec toGlobalFrom_noTrans(const pgMat& mat) const;

    /*!
        Converts this pgVec to a 4-element array.
        @param[out] r32x4 A destination of the converted 4-element array.
        @param[in] w The 4th coordinate of this pgVec.
    */
    void toR32x4(r32* r32x4, r32 w) const;

    /*!
        Returns a pgVec which is converted from a 4-element array.
        @param[in] r32x4 A 4-element array to be converted.
        @return A pgVec which is converted from the 4-element array.
    */
    static pgVec fromR32x4(const r32* r32x4);

    /*!
        Multiplies a 16-element array by a 4-element array as a matrix and a vector respectively.
        @param[out] res_r32x4 The destination of the multiplied 4-element array.
        @param[in] lhs_r32x16 The left hand side 16-element array.
        @param[in] rhs_r32x4 The right hand side 4-element array.
    */
    static void mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4);
};
