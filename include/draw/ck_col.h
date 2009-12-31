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
    @ingroup ckDraw
    A color, which is represented by red, green, blue and alpha components.
*/
struct CK_API ckCol
{
    static const ckCol ZERO; //!< The color whose all components are 0s.
    static const ckCol FULL; //!< The color whose all components are 255s.

    u8 r; //!< Red component.
    u8 g; //!< Green component.
    u8 b; //!< Blue component.
    u8 a; //!< Alpha component.

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
        @param[in] s The right hand side scalar.
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
        @param[in] s The right hand side scalar.
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
        @param[in] to An another ckCol.
        @param[in] ratio The value which indicates how far to interpolate between two ckCols. This value is clamped between 0.0f and 1.0f.
        @return A ckCol which is interpolated between two ckCols.
    */
    ckCol interp(ckCol to, r32 ratio) const;
};
