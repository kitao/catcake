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
    The draw manager singleton.
*/
class CK_API ckDrawMgr
{
    friend class ckTex;
    friend class ckFont;
    friend class ckShd;
    friend class ckLts;
    friend class ckScr;

public:
    //! @cond
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
    //! @endcond

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
        The ID of the default no-texture shader.
    */
    static const ckID DEFAULT_NO_TEXTURE_SHADER_ID;

    /*!
        The ID of the default rgb-texture shader.
    */
    static const ckID DEFAULT_RGB_TEXTURE_SHADER_ID;

    /*!
        The ID of the default rgba-texture shader.
    */
    static const ckID DEFAULT_RGBA_TEXTURE_SHADER_ID;

    /*!
        The ID of the default alpha-texture shader
    */
    static const ckID DEFAULT_ALPHA_TEXTURE_SHADER_ID;

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
        Returns the valid length of the specified texture length.
        @return The valid length of the specified texture length.
    */
    static u16 getValidTextureLength(u16 length);

    /*!
        Returns the byte size of the specified pixel format.
        @return The byte size of the specified pixel format.
    */
    static u16 getTexturePixelSize(ckTex::TexFormat format);

    /*!
        Returns whether ckDrawMgr singleton is created.
        @return Whether ckDrawMgr singleton is created.
    */
    static bool isCreated();

    /*!
        Creates ckDrawMgr singleton.
    */
    static void createAfterRes();

    /*!
        Destroys ckDrawMgr singleton.
    */
    static void destroyBeforeRes();

    /*!
        Returns whether rendering process is enabled.
        @return Whether rendering process is enabled.
    */
    static bool isRender();

    /*!
        Determines whether the rendering process is enabled.
        @param[in] is_render Whether the rendering process is enabled.
    */
    static void setRender(bool is_render);

    /*!
        Returns whether the specified screen exists.
        @param[in] scr_id A screen ID.
        @return Whether the specified screen exists.
    */
    static bool hasScreen(ckID scr_id);

    /*!
        Returns the specified screen. If the specified screen doesn't exist, returns NULL.
        The new screen is linked to the last of the screen list.
        @param[in] scr_id A screen ID.
        @return A screen.
    */
    static ckScr* getScreen(ckID scr_id);

    /*!
        Creates new screen which has the specified ID.
        @param[in] scr_id A screen ID.
        @return The instance of new screen.
    */
    static ckScr* newScreen(ckID scr_id);

    /*!
        Deletes the specified screen.
        @param[in] scr_id A screen ID.
    */
    static void deleteScreen(ckID scr_id);

    /*!
        Returns the first screen. If the first screen doesn't exist, returns NULL.
        @return The first screen.
    */
    static ckScr* getFirstScreenN();

    /*!
        Returns the last screen. If the last screen doesn't exist, return NULL.
        @return The last screen.
    */
    static ckScr* getLastScreenN();


    /*!
        Returns whether the specified texture exists.
        @param[in] tex_id A texture ID.
        @return Whether the specified texture exists.
    */
    static bool hasTexture(ckID tex_id);

    /*!
        Returns the specified texture. If the specified texture doesn't exist, returns NULL.
        @param[in] tex_id A texture ID.
        @return The specified texture.
    */
    static ckTex* getTexture(ckID tex_id);

    /*!
        Creates new texture which has the specified ID.
        @param[in] tex_id A texture ID.
        @param[in] width The width of a texture.
        @param[in] height The height of a texture.
        @param[in] format The format of a texture.
        @return The instance of new texture.
    */
    static ckTex* newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format);

    /*!
        Creates new texture from the specified raw-image.
        @param[in] tex_id A texture ID.
        @param[in] width The width of a texture.
        @param[in] height The height of a texture.
        @param[in] format The format of a texture.
        @param[in] image The pointer of the raw-image.
        @param[in] image_size The size of the raw-image.
        @return The instance of new texture.
    */
    static ckTex* newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format, const void* image, u32 image_size);

    /*!
        Deletes the specified texture.
        @param[in] tex_id A texture ID.
    */
    static void deleteTexture(ckID tex_id);

    /*!
        Returns the first texture. If the first texture doesn't exist, returns NULL.
        @return The first texture.
    */
    static ckTex* getFirstTextureN();

    /*!
        Returns the last texture. If the last texture doesn't exist, return NULL.
        @return The last texture.
    */
    static ckTex* getLastTextureN();

    /*!
        Returns the number of the indices of the specified font.
        @param[in] font_id A font ID.
        @return The number of the indices of the specified font.
    */
    static u16 getFontIndexNum(ckID font_id);

    /*!
        Returns the ID of the current font.
        @return The ID of the current font.
    */
    static ckID getFontID();

    /*!
        Returns the current font-index.
        @return The current font-index.
    */
    static u16 getFontIndex();

    /*!
        Sets the current font and font-index.
        @param[in] font_id A font ID.
        @param[in] font_index A font-index.
    */
    static void setFont(ckID font_id, u16 font_index);

    /*!
        Returns the current font size.
        @return The current font size.
    */
    static u16 getFontSize();

    /*!
        Sets the current font size.
        Font size represents the max height of the fonts in pixel.
        @param[in] font_size The size of font.
    */
    static u16 setFontSize(u16 font_size);

    /*!
        Returns the width of the specified string when rendered.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    static u16 calcFontDrawWidth(const char* str, ...);

    /*!
        Returns the width of the specified string when rendered.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    static u16 calcFontDrawWidth(const wchar_t* str, ...);

    /*!
        Returns whether the specified shader exists.
        @param[in] shd_id A shader ID.
        @return Whether the specified shader exists.
    */
    static bool hasShader(ckID shd_id);

    /*!
        Returns the shader.
        @param[in] shd_id A shader ID.
        @return A shader.
    */
    static ckShd* getShader(ckID shd_id);

    /*!
        Creates a new shader.
        @param[in] shd_id A shader ID.
        @param[in] vert_code Vertex shader code.
        @param[in] frag_code fragment shader code.
        @param[in] uni_num The number of an uniforms.
        @param[in] att_num The number of an attributes.
        @param[in] tex_num The number of textures.
        @return A shader.
    */
    static ckShd* newShader(ckID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);

    /*!
        Deletes the specified shader.
        @param[in] shd_id A shader ID.
    */
    static void deleteShader(ckID shd_id);

    /*!
        Returns the first shader. If the first texture doesn't exist, returns NULL.
        @return The first shader.
    */
    static ckShd* getFirstShaderN();

    /*!
        Returns the last shader. If the last texture doesn't exist, returns NULL.
        @return The last shader.
    */
    static ckShd* getLastShaderN();

    /*!
        Returns whether the specified light-set exits.
        @param[in] lts_id A light-set ID.
        @return Whether the specified light-set exits.
    */
    static bool hasLightSet(ckID lts_id);

    /*!
        Returns the specified light-set.
        @param[in] lts_id A light-set ID.
        @return The specified light-set.
    */
    static ckLts* getLightSet(ckID lts_id);

    /*!
        Creates a new light-set.
        @param[in] lts_id A light-set ID.
        @return A light-set.
    */
    static ckLts* newLightSet(ckID lts_id);

    /*!
        Deletes the specified light-set.
        @param[in] lts_id A light-set ID.
    */
    static void deleteLightSet(ckID lts_id);

    /*!
        Returns the first light-set. If the first light-set doesn't exist, returns NULL.
        @return The first light-set.
    */
    static ckLts* getFirstLightSetN();

    /*!
        Returns the last light-set. If the last light-set doesn't exist, returns NULL.
        @return The last light-set.
    */
    static ckLts* getLastLightSetN();

    /*!
        Deletes the all texture objects. This method is only for system.
    */
    static void deleteAllVramObjForSystem();

    /*!
        Updates the framebuffer. This methos is only for system.
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
