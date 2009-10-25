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


class pgRend;
class pgPrim;
class pgSprt;
class pgScr;
class pgDrawMgr;


/*!
    @ingroup pgDraw
    TODO
*/
class PG_API pgTex
{
    friend class pgRend;
    friend class pgPrim;
    friend class pgSprt;
    friend class pgScr;
    friend class pgDrawMgr;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionVolatileTextureUploaded);

    enum TexFormat
    {
        FORMAT_RGB, //
        FORMAT_RGBA, //
        FORMAT_ALPHA, //

        FORMAT_PNG_RGB, //
        FORMAT_PNG_RGBA, //
        FORMAT_PNG_ALPHA
    };

    enum TexMode
    {
        MODE_READ_ONLY, //
        MODE_READ_WRITE, //
        MODE_FRAMEBUFFER, //
        MODE_VOLATILE
    };

    pgTex* getPrevN() const;
    pgTex* getNextN() const;

    pgID getID() const;
    u16 getWidth() const;
    u16 getHeight() const;
    TexFormat getFormat() const;
    TexMode getMode() const;
    const void* getImage() const;
    u32 getImageSize() const;

    void* editImage();
    void clearImage(pgCol col);
    void resizeImage(u16 width, u16 height);

    void setVolatile();

    pgID getProxyTextureID() const;
    void setProxyTextureID(pgID tex_id);

private:
    enum TexFlag
    {
        FLAG_UPLOAD, //
        FLAG_UV_ADJUST
    };

    pgTex(pgID tex_id, u16 width, u16 height, TexFormat format, TexMode mode, const void* image, u32 image_size);
    ~pgTex();
    void operator=(const pgTex&);

    u32 getTexObj();
    void expandAndRegisterTexture_ptx();
    void expandAndRegisterTexture_png();

    pgID m_id;
    u16 m_width;
    u16 m_height;
    pgType<u8, TexFormat> m_format;
    pgType<u8, TexMode> m_mode;
    pgFlag<u8, TexFlag> m_flag;
    const void* m_image;
    u32 m_image_size;
    u32 m_tex_obj;
    r32 m_u_param_a;
    r32 m_u_param_b;
    r32 m_v_param_a;
    r32 m_v_param_b;
    pgTex* m_proxy_tex;
};
