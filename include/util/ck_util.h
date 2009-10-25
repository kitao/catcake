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
    @ingroup pgUtil
    TODO
*/
class PG_API pgUtil
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidData);

    static u32 strlen(const char* str);
    static u32 wcslen(const wchar_t* str);
    static void charToWchar(wchar_t* buf, u32 buf_size, const char* str);

    static const char* getBasename(const char* filename);
    static const char* getExtension(const char* filename);

    static void calcNormalAsTriangles(pgVec* normal, const pgPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing);

    static bool readPNGInfo(u16* width, u16* height, pgTex::TexFormat* format, const void* data, u32 data_size);
    static bool readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size);

    static void loadWindowsFont(const char* filename);

    static void loadShader(pgID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num);

    static void loadPixelArtModel(const char* pxm_file, const char* png_file, r32 scale);
    static void loadPixelArtModelAs(pgID res_id, const char* pxm_file, const char* png_file, r32 scale);

    static void import3DS(const char* filename, pgID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale);
    static void import3DSAs(pgID res_id, const char* filename, pgID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale);
};
