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


#include "ck_util_all.h"

#include "ck_res_all.h"
#include "ck_low_level_api.h"


u32 ckUtil::strlen(const char* str)
{
    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    u32 len = 0;

    while (*str)
    {
        str++;
        len++;
    }

    return len;
}


u32 ckUtil::wcslen(const wchar_t* str)
{
    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    u32 len = 0;

    while (*str)
    {
        str++;
        len++;
    }

    return len;
}


void ckUtil::charToWchar(wchar_t* buf, u32 buf_size, const char* str)
{
    if (!buf || buf_size == 0 || !str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    for (u32 i = 0; i < buf_size; i++)
    {
        char c = str[i];

        if (!c)
        {
            buf[i] = L'\0';
            break;
        }

        buf[i] = c;
    }

    buf[buf_size - 1] = L'\0';
}


const char* ckUtil::getBasename(const char* filename)
{
    if (!filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    const char* base = filename;

    for (const char* c = base; *c != '\0'; c++)
    {
        if (*c == '\\' || *c == '/' || *c == ':')
        {
            base = c + 1;
        }
    }

    return base;
}


const char* ckUtil::getExtension(const char* filename)
{
    if (!filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    const char* ext = getBasename(filename);

    for ( ; *ext != '\0'; ext++)
    {
        if (*ext == '.')
        {
            return ext + 1;
        }
    }

    return ext;
}


void ckUtil::calcNormalAsTriangles(ckVec* normal, const ckPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing)
{
    if (!normal || !prim_data || vert_num == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (vert_num < 3)
    {
        for (s32 i = 0; i < vert_num; i++)
        {
            normal[i] = ckVec::Z_UNIT;
        }

        return;
    }

    u16 tri_vert_num = (vert_num / 3) * 3;

    for (s32 i = 0; i < tri_vert_num; i += 3)
    {
        ckVec vec1 = prim_data[i + 1].pos - prim_data[i].pos;
        ckVec vec2 = prim_data[i + 2].pos - prim_data[i].pos;

        normal[i] = normal[i + 1] = normal[i + 2] = vec1.cross(vec2).normalize();
    }

    for (s32 i = tri_vert_num; i < vert_num; i++)
    {
        normal[i] = ckVec::Z_UNIT;
    }

    if (!is_smoothing)
    {
        return;
    }

    u32 mark_buf_size = sizeof(u16) * vert_num;
    u16* mark_buf = static_cast<u16*>(ckMalloc(mark_buf_size));

    ckMemMgr::memset(mark_buf, 0, mark_buf_size);

    for (s32 i = 0; i < vert_num; i++)
    {
        if (mark_buf[i] > 0)
        {
            continue;
        }

        u16 mark = i + 1;
        u16 mark_num = 1;

        const ckVec& pos1 = prim_data[i].pos;
        const ckVec& n1 = normal[i];

        ckVec avgn = n1;

        mark_buf[i] = mark;

        for (s32 j = i + 1; j < vert_num; j++)
        {
            const ckVec& pos2 = prim_data[j].pos;
            const ckVec& n2 = normal[j];

            if (pos1.x == pos2.x && pos1.y == pos2.y && pos1.z == pos2.z)
            {
                r32 inner = n1.dot(n2);

                if (inner > 0.7071f) // cos 45
                {
                    mark_buf[j] = mark;
                    mark_num++;

                    if (inner < 1.0f - ckMath::EPSILON)
                    {
                        avgn += n2;
                    }
                }
            }
        }

        if (mark_num > 1)
        {
            if (avgn.x == 0.0f && avgn.y == 0.0f && avgn.z == 0.0f)
            {
                continue;
            }

            avgn = avgn.normalize();

            for (s32 j = i; j < vert_num; j++)
            {
                if (mark_buf[j] == mark)
                {
                    normal[j] = avgn;
                }
            }
        }
    }

    ckFree(mark_buf);
}


bool ckUtil::readPNGInfo(u16* width, u16* height, ckTex::TexFormat* format, const void* data, u32 data_size)
{
    if (!width || !height || !format || !data || data_size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::TextureFormat texture_format;

    bool res = ckLowLevelAPI::readPNGInfo(width, height, &texture_format, data, data_size);

    *format = static_cast<ckTex::TexFormat>(ckTex::FORMAT_PNG_RGB + texture_format);

    return res;
}


bool ckUtil::readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size)
{
    if (!buf || buf_size == 0 || buf_line_size == 0 || !data || data_size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ckLowLevelAPI::readPNGImage(buf, buf_size, buf_line_size, data, data_size);
}


void ckUtil::loadWindowsFont(const char* filename)
{
    if (!filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    char buf[256];
    ckLowLevelAPI::getWindowsFontDirectory(buf, 256);

    ckStr<char, 255> str = buf;
    str += filename;

    ckResMgr::loadResource(str.getString(), true);
}


void ckUtil::loadShader(ckID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num)
{
    ckID vert_id = ckID::genID();
    char* vert_code = NULL;

    if (vert_file)
    {
        ckResMgr::loadResourceAs(vert_id, vert_file, false);
        ckRes res = ckResMgr::getResource(vert_id);
        vert_code = static_cast<char*>(ckMalloc(res.getDataSize() + 1));

        ckMemMgr::memcpy(vert_code, res.getData<void>(), res.getDataSize());
        vert_code[res.getDataSize()] = '\0';

        ckResMgr::removeResource(vert_id);
    }

    ckID frag_id = ckID::genID();
    char* frag_code = NULL;

    if (frag_file)
    {
        ckResMgr::loadResourceAs(frag_id, frag_file, false);
        ckRes res = ckResMgr::getResource(frag_id);
        frag_code = static_cast<char*>(ckMalloc(res.getDataSize() + 1));

        ckMemMgr::memcpy(frag_code, res.getData<void>(), res.getDataSize());
        frag_code[res.getDataSize()] = '\0';

        ckResMgr::removeResource(frag_id);
    }

    ckDrawMgr::newShader(shd_id, vert_code, frag_code, uni_num, att_num, tex_num);

    if (vert_code)
    {
        ckFree(vert_code);
    }

    if (frag_code)
    {
        ckFree(frag_code);
    }
}
