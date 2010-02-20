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


class ckRend;
class ckPrim;
class ckSprt;
class ckScr;
class ckDrawMgr;


/*!
    @ingroup ckDraw
    A texture, an image container for drawing elements.
*/
class CK_API ckTex
{
    friend class ckRend;
    friend class ckPrim;
    friend class ckSprt;
    friend class ckScr;
    friend class ckDrawMgr;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionVolatileTextureUploaded);
    ckDefineException(ExceptionEndEditImageNotCalled);
    //! @endcond

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
        Starts editing the raw image of this texture.
        endEditImage must be called after editing.
        @return The raw image of this texture.
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
        Sets the all pixels of this texture to 0.
        This method can't be called during editing the raw image.
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
        FLAG_EDIT, //
        FLAG_UPLOAD, //
        FLAG_UV_ADJUST
    };

    ckTex(ckID tex_id, u16 width, u16 height, TexFormat format, TexMode mode, const void* image, u32 image_size);
    ~ckTex();
    void operator=(const ckTex&);

    u32 getTexObj();
    void expandAndRegisterTexture_ctx();
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
