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
    TODO
*/
class PG_API pgDrawMgr
{
    friend class pgTex;
    friend class pgFont;
    friend class pgShd;
    friend class pgLts;
    friend class pgScr;

public:
    pgDefineException(ExceptionCalcFontDrawWidthFailed);
    pgDefineException(ExceptionCreateFreeTypeFailed);
    pgDefineException(ExceptionDestroyFreeTypeFailed);
    pgDefineException(ExceptionFontFinalizerFailed);
    pgDefineException(ExceptionFontInitializerFailed);
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionNotFound);
    pgDefineException(ExceptionNotInitialized);
    pgDefineException(ExceptionSameIDExists);
    pgDefineException(ExceptionTextureInitializerFailed);

    /*!
        The ID of the invisible screen.
    */
    static const pgID INVISIBLE_SCREEN_ID;

    /*!
        The ID of the default 3D screen.
    */
    static const pgID DEFAULT_3D_SCREEN_ID;

    /*!
        The ID of the defualt 2D screen.
    */
    static const pgID DEFAULT_2D_SCREEN_ID;

    /*!
        The ID of the default light set.
    */
    static const pgID DEFAULT_LIGHT_SET_ID;

    /*!
        The ID of the default shader.
    */
    static const pgID DEFAULT_SHADER_ID;

    /*!
        The maximum rendering interval time [msec].
    */
    static const u32 MAX_RENDER_INTERVAL_MSEC_TIME = 100;

    /*!
        Returns whether programmable shader is available.
        @return Whether programmable shader is available.
    */
    static bool isShaderAvailable();

    /*!
        Returns the maximum pixel length of a side of a texture.
        @return The maximum pixel length of a side of a texture.
    */
    static u16 getMaxTextureLength();

    /*!
        TODO
    */
    static u16 getValidTextureLength(u16 length);

    /*!
        TODO
    */
    static u16 getTexturePixelSize(pgTex::TexFormat format);

    /*!
        TODO
    */
    static bool isCreated();

    /*!
        TODO
    */
    static void createAfterRes();

    /*!
        TODO
    */
    static void destroyBeforeRes();

    /*!
        TODO
    */
    static bool isRender();

    /*!
        TODO
    */
    static void setRender(bool is_render);


    /*!
        TODO
    */
    static bool hasScreen(pgID scr_id);

    /*!
        TODO
    */
    static pgScr* getScreen(pgID scr_id);

    /*!
        TODO
    */
    static pgScr* newScreen(pgID scr_id);

    /*!
        TODO
    */
    static void deleteScreen(pgID scr_id);

    /*!
        TODO
    */
    static pgScr* getFirstScreenN();

    /*!
        TODO
    */
    static pgScr* getLastScreenN();


    /*!
        TODO
    */
    static bool hasTexture(pgID tex_id);

    /*!
        TODO
    */
    static pgTex* getTexture(pgID tex_id);

    /*!
        TODO
    */
    static pgTex* newTexture(pgID tex_id, u16 width, u16 height, pgTex::TexFormat format);

    /*!
        TODO
    */
    static pgTex* newTexture(pgID tex_id, u16 width, u16 height, pgTex::TexFormat format, const void* image, u32 image_size);

    /*!
        TODO
    */
    static void deleteTexture(pgID tex_id);

    /*!
        TODO
    */
    static pgTex* getFirstTextureN();

    /*!
        TODO
    */
    static pgTex* getLastTextureN();

    /*!
        TODO
    */
    static u16 getFontIndexNum(pgID font_id);

    /*!
        TODO
    */
    static pgID getFontID();

    /*!
        TODO
    */
    static u16 getFontIndex();

    /*!
        TODO
    */
    static void setFont(pgID font_id, u16 font_index);

    /*!
        TODO
    */
    static u16 getFontSize();

    /*!
        TODO
    */
    static u16 setFontSize(u16 font_size);

    /*!
        TODO
    */
    static u16 calcFontDrawWidth(const char* str, ...);

    /*!
        TODO
    */
    static u16 calcFontDrawWidth(const wchar_t* str, ...);

    /*!
        TODO
    */
    static bool hasShader(pgID shd_id);

    /*!
        TODO
    */
    static pgShd* getShader(pgID shd_id);

    /*!
        TODO
    */
    static pgShd* newShader(pgID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);

    /*!
        TODO
    */
    static void deleteShader(pgID shd_id);

    /*!
        TODO
    */
    static pgShd* getFirstShaderN();

    /*!
        TODO
    */
    static pgShd* getLastShaderN();

    /*!
        TODO
    */
    static bool hasLightSet(pgID lts_id);

    /*!
        TODO
    */
    static pgLts* getLightSet(pgID lts_id);

    /*!
        TODO
    */
    static pgLts* newLightSet(pgID lts_id);

    /*!
        TODO
    */
    static void deleteLightSet(pgID lts_id);

    /*!
        TODO
    */
    static pgLts* getFirstLightSetN();

    /*!
        TODO
    */
    static pgLts* getLastLightSetN();

    /*!
        TODO
    */
    static void deleteAllVramObjForSystem();

    /*!
        TODO
    */
    static void renderForSystem();

private:
    static const u32 SCREEN_HASH_SIZE = 10;
    static const u32 TEXTURE_HASH_SIZE = 10;
    static const u32 SHADER_HASH_SIZE = 10;
    static const u32 LIGHTSET_HASH_SIZE = 10;
    static const u32 FONT_NAME_BUFFER_SIZE = 64;

    pgDrawMgr();
    ~pgDrawMgr();
    void operator=(const pgDrawMgr&);

    static pgDrawMgr* instance();

    static void renderScreen(pgScr* scr, pgDraw** sort_list, const pgMat& view);
    static void sortList(pgDraw** sorted_start, pgDraw** sorted_end, pgDraw* target_list);

    static void textureInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void textureFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    static void fontInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void fontFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    pgMap<pgID, pgScr*> m_scr_map;
    pgMap<pgID, pgTex*> m_tex_map;
    pgMap<pgID, pgShd*> m_shd_map;
    pgMap<pgID, pgLts*> m_lts_map;
    bool m_is_render;
    pgID m_font_id;
    u16 m_font_index;
    u16 m_font_size;
    void* m_font_info;

    static pgDrawMgr* m_instance;
};
