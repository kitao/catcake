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
    A font texture, the texture specialized for drawing font.
*/
class CK_API ckFont
{
public:
    //! @cond
    ckDefineException(ExceptionDrawStringFailed);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        Constructs this font texture.
    */
    ckFont();

    /*!
        Destructs this font texture.
    */
    ~ckFont();

    /*!
        Initializes this font texture.
        @param[in] width The width of this font texture.
        @param[in] height The width of this font texture.
    */
    void init(u16 width, u16 height);

    /*!
        Returns the ID of this font texture.
        @return The ID of this font texture.
    */
    ckID getTextureID() const;

    /*!
        Returns the width of this font texture.
        @return The width of this font texture.
    */
    u16 getWidth() const;

    /*!
        Returns the height of this font texture.
        @return The height of this font texture.
    */
    u16 getHeight() const;

    /*!
        Returns the raw image of this font texture as read-only.
        @return The raw image of this font texture.
    */
    const void* getImage() const;

    /*!
        Returns the size of the raw image of this font texture.
        @return The size of the raw image of this font texture.
    */
    u32 getImageSize() const;

    /*!
        Starts editing the raw image of this font texture.
        endEditImage must be called after editing.
        @return The raw image of this font texture.
    */
    void* beginEditImage();

    /*!
        Ends editing the raw image and updates the whole area.
    */
    void endEditImage();

    /*!
        Ends editing the raw image and updates the specified area.
        @param[in] x The left position of the modified area.
        @param[in] y The top position of the modified area.
        @param[in] width The width of the modified area.
        @param[in] height The height of the modified area.
    */
    void endEditImage(u16 x, u16 y, u16 width, u16 height);

    /*!
        Sets the all pixels of this font texture to 0.
        beginEditImage must be called before this method.
    */
    void clearImage();

    /*!
        Renders the specified string.
        @param[in] x The x-coordinate of drawing position.
        @param[in] y The y-coordinate of drawing position.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    u16 drawString(s16 x, s16 y, const char* str, ...);

    /*!
        Renders the specified string.
        @param[in] x The x-coordinate of drawing position.
        @param[in] y The y-coordinate of drawing position.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    u16 drawString(s16 x, s16 y, const wchar_t* str, ...);

private:
    ckTex* m_tex;
};
