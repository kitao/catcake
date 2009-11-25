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


class ckRend;
class ckPrim;
class ckSprt;
class ckScr;
class ckDrawMgr;


/*!
    @ingroup ckDraw
    A texture.
*/
class CK_API ckTex
{
    friend class ckRend;
    friend class ckPrim;
    friend class ckSprt;
    friend class ckScr;
    friend class ckDrawMgr;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionVolatileTextureUploaded);

    /*!
        The texture formats.
    */
    enum TexFormat
    {
        FORMAT_RGB, //!< R8G8B8
        FORMAT_RGBA, //!< R8G8B8A8
        FORMAT_ALPHA, //!< A8

        FORMAT_PNG_RGB, //!< R8G8B8 from PNG
        FORMAT_PNG_RGBA, //!< R8G8B8A8 from PNG
        FORMAT_PNG_ALPHA //!< A8 from PNG
    };

    /*!
        The texture modes.
    */
    enum TexMode
    {
        MODE_READ_ONLY, //!< Read-only
        MODE_READ_WRITE, //!< Read and write.
        MODE_FRAMEBUFFER, //!< Copied from the framebuffer.
        MODE_VOLATILE //!< Vram only.
    };

    /*!
        Returns the previous texture. If the previous texture doesn't exist, returns NULL.
        @return The previous texture.
    */
    ckTex* getPrevN() const;

    /*!
        Returns the next texture. If the next texture doesn't exist, returns NULL.
        @return The next texture.
    */
    ckTex* getNextN() const;

    /*!
        Returns the texture ID.
        @return The texture ID.
    */
    ckID getID() const;

    /*!
        Returns the width of this texture.
        @return The width of this texture.
    */
    u16 getWidth() const;

    /*!
        Returns the height of this texture.
        @return The height of this texture.
    */
    u16 getHeight() const;

    /*!
        Returns the format of this texture.
        @return The format of this texture.
    */
    TexFormat getFormat() const;

    /*!
        Returns the texture mode.
        @return The texture mode.
    */
    TexMode getMode() const;

    /*!
        Returns the raw image of this texture as read-only.
        @return The raw image of this texture.
    */
    const void* getImage() const;

    /*!
        Returns the size of this texture.
        @return The size of this texture.
    */
    u32 getImageSize() const;

    /*!
        Returns the raw image of this texture for editting.
        @return The raw image of this texture.
    */
    void* editImage();

    /*!
        Sets the all pixels of this texture to 0.
    */
    void clearImage(ckCol col);

    /*!
        Changes the size of this texture.
        @param[in] width The width of a texture.
        @param[in] height The height of a texture.
    */
    void resizeImage(u16 width, u16 height);

    /*!
        Deletes the verbose images on the main memory.
    */
    void setVolatile();

    /*!
        Returns the proxy texture ID.
        @return The proxy texture ID.
    */
    ckID getProxyTextureID() const;

    /*!
        Sets the proxy texture.
        @param[in] tex_id A proxy texture ID.
    */
    void setProxyTextureID(ckID tex_id);

private:
    enum TexFlag
    {
        FLAG_UPLOAD, //
        FLAG_UV_ADJUST
    };

    ckTex(ckID tex_id, u16 width, u16 height, TexFormat format, TexMode mode, const void* image, u32 image_size);
    ~ckTex();
    void operator=(const ckTex&);

    u32 getTexObj();
    void expandAndRegisterTexture_ptx();
    void expandAndRegisterTexture_png();

    ckID m_id;
    u16 m_width;
    u16 m_height;
    ckType<u8, TexFormat> m_format;
    ckType<u8, TexMode> m_mode;
    ckFlag<u8, TexFlag> m_flag;
    const void* m_image;
    u32 m_image_size;
    u32 m_tex_obj;
    r32 m_u_param_a;
    r32 m_u_param_b;
    r32 m_v_param_a;
    r32 m_v_param_b;
    ckTex* m_proxy_tex;
};
