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
    TODO
*/
class CK_API ckDrawMgr
{
    friend class ckTex;
    friend class ckFont;
    friend class ckShd;
    friend class ckLts;
    friend class ckScr;

public:
    ckDefineException(ExceptionCalcFontDrawWidthFailed);
    ckDefineException(ExceptionCreateFreeTypeFailed);
    ckDefineException(ExceptionDestroyFreeTypeFailed);
    ckDefineException(ExceptionFontFinalizerFailed);
    ckDefineException(ExceptionFontInitializerFailed);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionNotInitialized);
    ckDefineException(ExceptionSameIDExists);
    ckDefineException(ExceptionTextureInitializerFailed);

    /*!
        The ID of the invisible screen.
    */
    static const ckID INVISIBLE_SCREEN_ID;

    /*!
        The ID of the default 3D screen.
    */
    static const ckID DEFAULT_3D_SCREEN_ID;

    /*!
        The ID of the defualt 2D screen.
    */
    static const ckID DEFAULT_2D_SCREEN_ID;

    /*!
        The ID of the default light set.
    */
    static const ckID DEFAULT_LIGHT_SET_ID;

    /*!
        The ID of the default shader.
    */
    static const ckID DEFAULT_SHADER_ID;

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
    static u16 getTexturePixelSize(ckTex::TexFormat format);

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
    static bool hasScreen(ckID scr_id);

    /*!
        TODO
    */
    static ckScr* getScreen(ckID scr_id);

    /*!
        TODO
    */
    static ckScr* newScreen(ckID scr_id);

    /*!
        TODO
    */
    static void deleteScreen(ckID scr_id);

    /*!
        TODO
    */
    static ckScr* getFirstScreenN();

    /*!
        TODO
    */
    static ckScr* getLastScreenN();


    /*!
        TODO
    */
    static bool hasTexture(ckID tex_id);

    /*!
        TODO
    */
    static ckTex* getTexture(ckID tex_id);

    /*!
        TODO
    */
    static ckTex* newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format);

    /*!
        TODO
    */
    static ckTex* newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format, const void* image, u32 image_size);

    /*!
        TODO
    */
    static void deleteTexture(ckID tex_id);

    /*!
        TODO
    */
    static ckTex* getFirstTextureN();

    /*!
        TODO
    */
    static ckTex* getLastTextureN();

    /*!
        TODO
    */
    static u16 getFontIndexNum(ckID font_id);

    /*!
        TODO
    */
    static ckID getFontID();

    /*!
        TODO
    */
    static u16 getFontIndex();

    /*!
        TODO
    */
    static void setFont(ckID font_id, u16 font_index);

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
    static bool hasShader(ckID shd_id);

    /*!
        TODO
    */
    static ckShd* getShader(ckID shd_id);

    /*!
        TODO
    */
    static ckShd* newShader(ckID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);

    /*!
        TODO
    */
    static void deleteShader(ckID shd_id);

    /*!
        TODO
    */
    static ckShd* getFirstShaderN();

    /*!
        TODO
    */
    static ckShd* getLastShaderN();

    /*!
        TODO
    */
    static bool hasLightSet(ckID lts_id);

    /*!
        TODO
    */
    static ckLts* getLightSet(ckID lts_id);

    /*!
        TODO
    */
    static ckLts* newLightSet(ckID lts_id);

    /*!
        TODO
    */
    static void deleteLightSet(ckID lts_id);

    /*!
        TODO
    */
    static ckLts* getFirstLightSetN();

    /*!
        TODO
    */
    static ckLts* getLastLightSetN();

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

    ckDrawMgr();
    ~ckDrawMgr();
    void operator=(const ckDrawMgr&);

    static ckDrawMgr* instance();

    static void renderScreen(ckScr* scr, ckDraw** sort_list, const ckMat& view);
    static void sortList(ckDraw** sorted_start, ckDraw** sorted_end, ckDraw* target_list);

    static void textureInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void textureFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    static void fontInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void fontFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    ckMap<ckID, ckScr*> m_scr_map;
    ckMap<ckID, ckTex*> m_tex_map;
    ckMap<ckID, ckShd*> m_shd_map;
    ckMap<ckID, ckLts*> m_lts_map;
    bool m_is_render;
    ckID m_font_id;
    u16 m_font_index;
    u16 m_font_size;
    void* m_font_info;

    static ckDrawMgr* m_instance;
};
