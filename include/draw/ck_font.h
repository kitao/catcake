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
    A font texture, the texture specialized for drawing font.
*/
class CK_API ckFont
{
public:
    ckDefineException(ExceptionDrawStringFailed);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);

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
        Returns the raw image of this font texture for editting.
        @return The raw image of this font texture.
    */
    void* editImage();

    /*!
        Sets the all pixels of this font texture to 0.
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
