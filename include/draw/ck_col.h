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
    @ingroup ckDraw
    A 32-bit color which is represented by red, green, blue and alpha components.
*/
struct CK_API ckCol
{
    static const ckCol ZERO; //!< The color whose all components are 0s.
    static const ckCol FULL; //!< The color whose all components are 255s.

    u8 r, g, b, a;

    /*!
        Constructs a ckCol.
    */
    ckCol();

    /*!
        Constructs and initializes a ckCol from the specified ckCol.
        @param[in] col A ckCol.
    */
    ckCol(const ckCol& col);

    /*!
        Constructs and initializes a ckCol from the specified components.
        @param[in] r_ A red component.
        @param[in] g_ A green component.
        @param[in] b_ A blue component.
        @param[in] a_ An alpha component. If not specified, 255 is used.
    */
    ckCol(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        Sets all components from the specified components.
        @param[in] r_ A red component.
        @param[in] g_ A green component.
        @param[in] b_ A blue component.
        @param[in] a_ An alpha component. If not specified, 255 is used.
    */
    void set(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        Returns whether this ckCol is equal to the right hand side ckCol.
        @param[in] col The right hand side ckCol.
        @return Whether this ckCol is equal to the right hand side ckCol.
    */
    bool operator==(ckCol col) const;

    /*!
        Returns whether this ckCol is unequal to the right hand side ckCol.
        @param[in] col The right hand side ckCol.
        @return Whether this ckCol is unequal to the right hand side ckCol.
    */
    bool operator!=(ckCol col) const;

    /*!
        Returns a ckCol which is the sum of this ckCol and the right hand side ckCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side ckCol.
        @return A ckCol which is the sum of two ckCols.
    */
    ckCol operator+(ckCol col) const;

    /*!
        Adds the right hand side ckCol to this ckCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side ckCol.
    */
    void operator+=(ckCol col);

    /*!
        Returns a ckCol which is the right hand side ckCol subtracted from this ckCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side ckCol.
        @return A ckCol which is the right hand side ckCol subtracted from this ckCol.
    */
    ckCol operator-(ckCol col) const;

    /*!
        Subtracts the right hand side ckCol from this ckCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side ckCol.
    */
    void operator-=(ckCol col);

    /*!
        Returns a ckCol which is this ckCol multiplied with right hand side ckCol and divided by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side ckCol.
        @return A ckCol which is this ckCol multiplied with right hand side ckCol and divided by 255.
    */
    ckCol operator*(ckCol col) const;

    /*!
        Multiplies this ckVec with the right hand side ckCol and divides by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side ckCol.
    */
    void operator*=(ckCol col);

    /*!
        Returns a ckCol which is this ckCol multiplied with right hand side scalar and divided by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] The right hand side scalar.
        @return A ckCol which is this ckCol multiplied with right hand side scalar and divided by 255.
    */
    ckCol operator*(r32 s) const;

    /*!
        Returns a ckCol which is the right hand side ckCol multiplied with the left hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The left hand side scalar.
        @param[in] col The right hand side ckCol.
        @return A ckCol which is the right hand side ckCol multiplied with the left hand side scalar.
    */
    CK_API friend ckCol operator*(r32 s, ckCol col);

    /*!
        Multiplies this ckCol with the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] The right hand side scalar.
    */
    void operator*=(r32 s);

    /*!
        Returns a ckCol which is this ckCol divided by the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
        @return A ckCol which is this ckCol divided by the right hand side scalar.
    */
    ckCol operator/(r32 s) const;

    /*!
        Divides this ckCol by the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
    */
    void operator/=(r32 s);

    /*!
        Returns a ckCol which is interpolated between this ckCol and an another ckCol by the specified ratio, using linear interpolation.
        param[in] to An another ckCol.
        @param[in] ratio The value which indicates how far to interpolate between two ckCols. This value is clamped between 0.0f and 1.0f.
        @return A ckCol which is interpolated between two ckCols.
    */
    ckCol interp(ckCol to, r32 ratio) const;
};
