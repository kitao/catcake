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
    @ingroup pgDraw
    A 32-bit color which is represented by red, green, blue and alpha components.
*/
struct PG_API pgCol
{
    static const pgCol ZERO; //!< The color whose all components are 0s.
    static const pgCol FULL; //!< The color whose all components are 255s.

    u8 r, g, b, a;

    /*!
        Constructs a pgCol.
    */
    pgCol();

    /*!
        Constructs and initializes a pgCol from the specified pgCol.
        @param[in] col A pgCol.
    */
    pgCol(const pgCol& col);

    /*!
        Constructs and initializes a pgCol from the specified components.
        @param[in] r_ A red component.
        @param[in] g_ A green component.
        @param[in] b_ A blue component.
        @param[in] a_ An alpha component. If not specified, 255 is used.
    */
    pgCol(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        Sets all components from the specified components.
        @param[in] r_ A red component.
        @param[in] g_ A green component.
        @param[in] b_ A blue component.
        @param[in] a_ An alpha component. If not specified, 255 is used.
    */
    void set(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        Returns whether this pgCol is equal to the right hand side pgCol.
        @param[in] col The right hand side pgCol.
        @return Whether this pgCol is equal to the right hand side pgCol.
    */
    bool operator==(pgCol col) const;

    /*!
        Returns whether this pgCol is unequal to the right hand side pgCol.
        @param[in] col The right hand side pgCol.
        @return Whether this pgCol is unequal to the right hand side pgCol.
    */
    bool operator!=(pgCol col) const;

    /*!
        Returns a pgCol which is the sum of this pgCol and the right hand side pgCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side pgCol.
        @return A pgCol which is the sum of two pgCols.
    */
    pgCol operator+(pgCol col) const;

    /*!
        Adds the right hand side pgCol to this pgCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side pgCol.
    */
    void operator+=(pgCol col);

    /*!
        Returns a pgCol which is the right hand side pgCol subtracted from this pgCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side pgCol.
        @return A pgCol which is the right hand side pgCol subtracted from this pgCol.
    */
    pgCol operator-(pgCol col) const;

    /*!
        Subtracts the right hand side pgCol from this pgCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side pgCol.
    */
    void operator-=(pgCol col);

    /*!
        Returns a pgCol which is this pgCol multiplied with right hand side pgCol and divided by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side pgCol.
        @return A pgCol which is this pgCol multiplied with right hand side pgCol and divided by 255.
    */
    pgCol operator*(pgCol col) const;

    /*!
        Multiplies this pgVec with the right hand side pgCol and divides by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side pgCol.
    */
    void operator*=(pgCol col);

    /*!
        Returns a pgCol which is this pgCol multiplied with right hand side scalar and divided by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] The right hand side scalar.
        @return A pgCol which is this pgCol multiplied with right hand side scalar and divided by 255.
    */
    pgCol operator*(r32 s) const;

    /*!
        Returns a pgCol which is the right hand side pgCol multiplied with the left hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The left hand side scalar.
        @param[in] col The right hand side pgCol.
        @return A pgCol which is the right hand side pgCol multiplied with the left hand side scalar.
    */
    PG_API friend pgCol operator*(r32 s, pgCol col);

    /*!
        Multiplies this pgCol with the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] The right hand side scalar.
    */
    void operator*=(r32 s);

    /*!
        Returns a pgCol which is this pgCol divided by the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
        @return A pgCol which is this pgCol divided by the right hand side scalar.
    */
    pgCol operator/(r32 s) const;

    /*!
        Divides this pgCol by the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
    */
    void operator/=(r32 s);

    /*!
        Returns a pgCol which is interpolated between this pgCol and an another pgCol by the specified ratio, using linear interpolation.
        param[in] to An another pgCol.
        @param[in] ratio The value which indicates how far to interpolate between two pgCols. This value is clamped between 0.0f and 1.0f.
        @return A pgCol which is interpolated between two pgCols.
    */
    pgCol interp(pgCol to, r32 ratio) const;
};
