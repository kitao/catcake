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
    A 3x4 matrix which is represented by four pgVecs.
*/
struct PG_API pgMat
{
    static const pgMat ZERO; //!< The zero matrix.
    static const pgMat UNIT; //!< The unit matrix.

    pgVec x_axis; //!< The 1st row, which is usually used as the x-axis.
    pgVec y_axis; //!< The 2nd row, which is usually used as the y-axis.
    pgVec z_axis; //!< The 3rd row, which is usually used as the z-axis.
    pgVec trans; //!< The 4th row, which is usually used as the translation.

    /*!
        Constructs a pgMat.
    */
    pgMat();

    /*!
        Constructs and initialize a pgMat from the specified four pgVecs.
        @param[in] x_axis_ The x-axis.
        @param[in] y_axis_ The y-axis.
        @param[in] z_axis_ The z-axis.
        @param[in] trans_ The translation.
    */
    pgMat(const pgVec& x_axis_, const pgVec& y_axis_, const pgVec& z_axis_, const pgVec& trans_);

    /*!
        Sets all components from the specified four pgVecs.
        @param[in] x_axis_ The x-axis.
        @param[in] y_axis_ The y-axis.
        @param[in] z_axis_ The z-axis.
        @param[in] trans_ The translation.
    */
    void set(const pgVec& x_axis_, const pgVec& y_axis_, const pgVec& z_axis_, const pgVec& trans_);

    /*!
        Returns whether this pgMat is equal to the unit matrix.
        @return Whether this pgMat is equal to the unit matrix.
    */
    bool isUnit() const;

    /*!
        Returns a pgMat which is this pgMat rotated around its x-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A pgMat which is this pgMat rotated around its x-axis.
    */
    pgMat rotateX_r32(r32 deg) const;

    /*!
        Returns a pgMat which is this pgMat rotated around its y-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A pgMat which is this pgMat rotated around its y-axis.
    */
    pgMat rotateY_r32(r32 deg) const;

    /*!
        Returns a pgMat which is this pgMat rotated around its z-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A pgMat which is this pgMat rotated around its z-axis.
    */
    pgMat rotateZ_r32(r32 deg) const;

    /*!
        Returns a pgMat which is this pgMat rotated around its x-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A pgMat which is this pgMat rotated around its x-axis.
    */
    pgMat rotateX_s32(s32 deg) const;

    /*!
        Returns a pgMat which is this pgMat rotated around its y-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A pgMat which is this pgMat rotated around its y-axis.
    */
    pgMat rotateY_s32(s32 deg) const;

    /*!
        Returns a pgMat which is this pgMat rotated around its z-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A pgMat which is this pgMat rotated around its z-axis.
    */
    pgMat rotateZ_s32(s32 deg) const;

    /*!
        Returns a pgMat which is this pgMat scaled by the specified values.
        @param[in] x_scale The x-axis scale factor.
        @param[in] y_scale The y-axis scale factor.
        @param[in] z_scale The z-axis scale factor. If not specified, 1.0f is used.
        @return A pgMat which is this pgMat scaled by the specified values.
    */
    pgMat scale(r32 x_scale, r32 y_scale, r32 z_scale = 1.0f) const;

    /*!
        Returns a pgMat which is this pgMat translated along its axes for the specified lengths.
        @param[in] x A length of translation along the x-axis.
        @param[in] y A length of translation along the y-axis.
        @param[in] z A length of translation along the z-axis. If not specified, 0.0f is used.
        @return A pgMat which is this pgMat translated along its axes for the specified lengths.
    */
    pgMat translate(r32 x, r32 y, r32 z = 0.0f) const;

    /*!
        Returns a pgMat which is interpolated between this pgMat and an another pgMat by the specified ratio, using spherical linear interpolation.
        @param[in] to An another pgMat.
        @param[in] ratio The value which indicates how far to interpolate between two pgMats. This value is clamped between 0.0f and 1.0f.
        @return A pgMat which is interpolated between two pgMats.
    */
    pgMat slerp(const pgMat& to, r32 ratio) const;

    /*!
        Returns a pgMat whose components except the translation is interpolated between this pgMat and an another pgMat by the specified ratio,
        using spherical linear interpolation, and whose translation is equal to the translation of this pgMat.
        @param[in] to An another pgMat.
        @param[in] ratio The value which indicates how far to interpolate between two pgMats. This value is clamped between 0.0f and 1.0f.
        @return A pgMat whose components except the translation is interpolated between two pgMats.
    */
    pgMat slerp_noTrans(const pgMat& to, r32 ratio) const;

    /*!
        Returns a pgMat which is orthonormalized version of this pgMat.
        @return A pgMat which is orthonormalized version of this pgMat.
    */
    pgMat orthonormal() const;

    /*!
        Returns a pgMat which is this pgMat converted from in the world coordinate system to in the local coordinate system of the specified pgMat.
        @param[in] mat The origin pgMat of the local coordinate system.
        @return A pgMat which is this pgMat in the local coordinate system.
    */
    pgMat toLocalOf(const pgMat& mat) const;

    /*!
        Returns a pgMat which is this pgMat converted from in the local coordinate system of the specified pgMat to in the world coordinate system.
        @param[in] mat The origin pgMat of the local coordinate system.
        @return A pgMat which is this pgMat in the world coordinate system.
    */
    pgMat toGlobalFrom(const pgMat& mat) const;

    /*!
        Returns a pgMat which is this pgMat converted from in the world coordinate system to
        in the local coordinate system of the specified pgMat whose translation is ignored.
        @param[in] mat The origin pgMat of the local coordinate system, whose translation is ignored.
        @return A pgMat which is this pgMat in the local coordinate system.
    */
    pgMat toLocalOf_noTrans(const pgMat& mat) const;

    /*!
        Returns a pgMat which is this pgMat converted from in the local coordinate system of
        the specified pgMat whose translation is ignored to in the world coordinate system.
        @param[in] mat The origin pgMat of the local coordinate system, whose translation is ignored.
        @return A pgMat which is this pgMat in the world coordinate system.
    */
    pgMat toGlobalFrom_noTrans(const pgMat& mat) const;

    /*!
        Returns a pgMat which is the look-at matrix built by the specified values.
        @param[in] from The position of the camera.
        @param[in] to The position of the camera look-at target.
        @param[in] up The up direction of the camera.
        @return A pgMat which is the look-at matrix built by the specified values.
    */
    static pgMat lookAt(const pgVec& from, const pgVec& to, const pgVec& up);

    /*!
        Converts this pgMat to a 16-element array.
        @param[out] r32x16 A destination of the converted 16-element array.
    */
    void toR32x16(r32* r32x16) const;

    /*!
        Returns a pgMat which is converted from a 16-element array.
        @param[in] r32x16 A 16-element array to be converted.
        @return A pgMat which is converted from the 16-element array.
    */
    static pgMat fromR32x16(const r32* r32x16);

    /*!
        Multiplies two 16-element arrays as two matrices.
        @param[out] res_r32x16 The destination of the multiplied 16-element array.
        @param[in] lhs_r32x16 The left hand side 16-element array.
        @param[in] rhs_r32x16 The right hand side 16-element array.
    */
    static void mulR32x16(r32* res_r32x16, const r32* lhs_r32x16, const r32* rhs_r32x16);
};
