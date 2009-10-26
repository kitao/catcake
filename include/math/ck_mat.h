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
    @ingroup ckMath
    A 3x4 matrix which is represented by four ckVecs.
*/
struct CK_API ckMat
{
    static const ckMat ZERO; //!< The zero matrix.
    static const ckMat UNIT; //!< The unit matrix.

    ckVec x_axis; //!< The 1st row, which is usually used as the x-axis.
    ckVec y_axis; //!< The 2nd row, which is usually used as the y-axis.
    ckVec z_axis; //!< The 3rd row, which is usually used as the z-axis.
    ckVec trans; //!< The 4th row, which is usually used as the translation.

    /*!
        Constructs a ckMat.
    */
    ckMat();

    /*!
        Constructs and initialize a ckMat from the specified four ckVecs.
        @param[in] x_axis_ The x-axis.
        @param[in] y_axis_ The y-axis.
        @param[in] z_axis_ The z-axis.
        @param[in] trans_ The translation.
    */
    ckMat(const ckVec& x_axis_, const ckVec& y_axis_, const ckVec& z_axis_, const ckVec& trans_);

    /*!
        Sets all components from the specified four ckVecs.
        @param[in] x_axis_ The x-axis.
        @param[in] y_axis_ The y-axis.
        @param[in] z_axis_ The z-axis.
        @param[in] trans_ The translation.
    */
    void set(const ckVec& x_axis_, const ckVec& y_axis_, const ckVec& z_axis_, const ckVec& trans_);

    /*!
        Returns whether this ckMat is equal to the unit matrix.
        @return Whether this ckMat is equal to the unit matrix.
    */
    bool isUnit() const;

    /*!
        Returns a ckMat which is this ckMat rotated around its x-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A ckMat which is this ckMat rotated around its x-axis.
    */
    ckMat rotateX_r32(r32 deg) const;

    /*!
        Returns a ckMat which is this ckMat rotated around its y-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A ckMat which is this ckMat rotated around its y-axis.
    */
    ckMat rotateY_r32(r32 deg) const;

    /*!
        Returns a ckMat which is this ckMat rotated around its z-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A ckMat which is this ckMat rotated around its z-axis.
    */
    ckMat rotateZ_r32(r32 deg) const;

    /*!
        Returns a ckMat which is this ckMat rotated around its x-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A ckMat which is this ckMat rotated around its x-axis.
    */
    ckMat rotateX_s32(s32 deg) const;

    /*!
        Returns a ckMat which is this ckMat rotated around its y-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A ckMat which is this ckMat rotated around its y-axis.
    */
    ckMat rotateY_s32(s32 deg) const;

    /*!
        Returns a ckMat which is this ckMat rotated around its z-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A ckMat which is this ckMat rotated around its z-axis.
    */
    ckMat rotateZ_s32(s32 deg) const;

    /*!
        Returns a ckMat which is this ckMat scaled by the specified values.
        @param[in] x_scale The x-axis scale factor.
        @param[in] y_scale The y-axis scale factor.
        @param[in] z_scale The z-axis scale factor. If not specified, 1.0f is used.
        @return A ckMat which is this ckMat scaled by the specified values.
    */
    ckMat scale(r32 x_scale, r32 y_scale, r32 z_scale = 1.0f) const;

    /*!
        Returns a ckMat which is this ckMat translated along its axes for the specified lengths.
        @param[in] x A length of translation along the x-axis.
        @param[in] y A length of translation along the y-axis.
        @param[in] z A length of translation along the z-axis. If not specified, 0.0f is used.
        @return A ckMat which is this ckMat translated along its axes for the specified lengths.
    */
    ckMat translate(r32 x, r32 y, r32 z = 0.0f) const;

    /*!
        Returns a ckMat which is interpolated between this ckMat and an another ckMat by the specified ratio, using spherical linear interpolation.
        @param[in] to An another ckMat.
        @param[in] ratio The value which indicates how far to interpolate between two ckMats. This value is clamped between 0.0f and 1.0f.
        @return A ckMat which is interpolated between two ckMats.
    */
    ckMat slerp(const ckMat& to, r32 ratio) const;

    /*!
        Returns a ckMat whose components except the translation is interpolated between this ckMat and an another ckMat by the specified ratio,
        using spherical linear interpolation, and whose translation is equal to the translation of this ckMat.
        @param[in] to An another ckMat.
        @param[in] ratio The value which indicates how far to interpolate between two ckMats. This value is clamped between 0.0f and 1.0f.
        @return A ckMat whose components except the translation is interpolated between two ckMats.
    */
    ckMat slerp_noTrans(const ckMat& to, r32 ratio) const;

    /*!
        Returns a ckMat which is orthonormalized version of this ckMat.
        @return A ckMat which is orthonormalized version of this ckMat.
    */
    ckMat orthonormal() const;

    /*!
        Returns a ckMat which is this ckMat converted from in the world coordinate system to in the local coordinate system of the specified ckMat.
        @param[in] mat The origin ckMat of the local coordinate system.
        @return A ckMat which is this ckMat in the local coordinate system.
    */
    ckMat toLocalOf(const ckMat& mat) const;

    /*!
        Returns a ckMat which is this ckMat converted from in the local coordinate system of the specified ckMat to in the world coordinate system.
        @param[in] mat The origin ckMat of the local coordinate system.
        @return A ckMat which is this ckMat in the world coordinate system.
    */
    ckMat toGlobalFrom(const ckMat& mat) const;

    /*!
        Returns a ckMat which is this ckMat converted from in the world coordinate system to
        in the local coordinate system of the specified ckMat whose translation is ignored.
        @param[in] mat The origin ckMat of the local coordinate system, whose translation is ignored.
        @return A ckMat which is this ckMat in the local coordinate system.
    */
    ckMat toLocalOf_noTrans(const ckMat& mat) const;

    /*!
        Returns a ckMat which is this ckMat converted from in the local coordinate system of
        the specified ckMat whose translation is ignored to in the world coordinate system.
        @param[in] mat The origin ckMat of the local coordinate system, whose translation is ignored.
        @return A ckMat which is this ckMat in the world coordinate system.
    */
    ckMat toGlobalFrom_noTrans(const ckMat& mat) const;

    /*!
        Returns a ckMat which is the look-at matrix built by the specified values.
        @param[in] from The position of the camera.
        @param[in] to The position of the camera look-at target.
        @param[in] up The up direction of the camera.
        @return A ckMat which is the look-at matrix built by the specified values.
    */
    static ckMat lookAt(const ckVec& from, const ckVec& to, const ckVec& up);

    /*!
        Converts this ckMat to a 16-element array.
        @param[out] r32x16 A destination of the converted 16-element array.
    */
    void toR32x16(r32* r32x16) const;

    /*!
        Returns a ckMat which is converted from a 16-element array.
        @param[in] r32x16 A 16-element array to be converted.
        @return A ckMat which is converted from the 16-element array.
    */
    static ckMat fromR32x16(const r32* r32x16);

    /*!
        Multiplies two 16-element arrays as two matrices.
        @param[out] res_r32x16 The destination of the multiplied 16-element array.
        @param[in] lhs_r32x16 The left hand side 16-element array.
        @param[in] rhs_r32x16 The right hand side 16-element array.
    */
    static void mulR32x16(r32* res_r32x16, const r32* lhs_r32x16, const r32* rhs_r32x16);
};
