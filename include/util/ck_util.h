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
    @ingroup ckUtil
    The utility class, which provides various utility functions.
*/
class CK_API ckUtil
{
public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidData);

    /*!
        Returns the length of the specified string.
        @param[in] str A string.
        @return The length of the specified string.
    */
    static u32 strlen(const char* str);

    /*!
        Returns the length of the specified wchar string.
        @param[in] str A wchar string.
        @return The length of the specified wchar string.
    */
    static u32 wcslen(const wchar_t* str);

    /*!
        Converts a string to a wchar string.
        @param[out] buffer The output buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] str A string.
    */
    static void charToWchar(wchar_t* buf, u32 buf_size, const char* str);

    /*!
        Returns the base name part of the file name.
        @param[in] filename A file name.
        @return The base name part of the file name.
    */
    static const char* getBasename(const char* filename);

    /*!
        Returns the extension of the file name.
        @param[in] filename A file name.
        @return The extension of the file name.
    */
    static const char* getExtension(const char* filename);

    /*!
        Calculates the normals as triangles from the specified vertices.
        @param[out] normal The calculated normals.
        @param[in] prim_data A primitive data.
        @param[in] vert_num The number of the vertices.
        @param[in] Whether smoothing is on.
    */
    static void calcNormalAsTriangles(ckVec* normal, const ckPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing);

    /*!
        Reads the information of the png image.
        @param[out] width The width of an image.
        @param[out] height The height of an image.
        @param[out] format The pixel format.
        @param[in] data The data of A png image.
        @param[in] data The size of the data.
    */
    static bool readPNGInfo(u16* width, u16* height, ckTex::TexFormat* format, const void* data, u32 data_size);

    /*!
        Decodes the png image.
        @param[out] buf The destination buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] buf_line_size The size of buffer per line.
        @param[in] data The data of A png image.
        @param[in] data The size of the data.
    */
    static bool readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size);

    /*!
        Loads windows font. This medhod is only for Windows.
        @param[in] filename The file name of a windows font.
    */
    static void loadWindowsFont(const char* filename);

    /*!
        Loads a shader.
        @param[in] shd_id The ID of a shader.
        @param[in] vert_file The file name of a vertex shader.
        @param[in] frag_file The file name of a fragment shader.
        @param[in] uni_num The number of the uniforms.
        @param[in] att_num The number of the attributes.
        @param[in] tex_num The number of the textures.
    */
    static void loadShader(ckID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num);

    /*!
        Loads a pixel art model.
        @param[in] res_id The resource ID of a pixel art model.
        @param[in] ckl_file The file name of a configuration script.
        @param[in] png_file The file name of a png image.
        @param[in] scale The scaling factor of a pixel art model.
    */
    static void loadPixelArtModelAs(ckID res_id, const char* ckl_file, const char* png_file, r32 scale);

    /*!
        Imports a 3DS model.
        @param[in] filename The file name.
        @param[in] tex_id The texture ID.
        @param[in] has_normal Whether a 3DS model has normals.
        @param[in] is_smoothing_normal Whether smoothing is on.
        @param[in] scale THe scaling factor of a 3DS model.
    */
    static void import3DS(const char* filename, ckID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale);

    /*!
        Imports a 3DS model as the specified resource ID.
        @param[in] res_id The resource ID of a 3DS model.
        @param[in] filename The file name.
        @param[in] tex_id The texture ID.
        @param[in] has_normal Whether a 3DS model has normals.
        @param[in] is_smoothing_normal Whether smoothing is on.
        @param[in] scale THe scaling factor of a 3DS model.
    */
    static void import3DSAs(ckID res_id, const char* filename, ckID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale);
};
