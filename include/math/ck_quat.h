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
