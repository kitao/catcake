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


extern "C"
{
#include <ft2build.h>
#include FT_OUTLINE_H
#include FT_MODULE_H
}

#include "ck_low_level_api.h"

#include "ck_mem_all.h" // for ckMalloc, ckFree


static FT_Library s_freetype;
static FT_Memory s_memory;


static void* myMalloc(FT_Memory, long size)
{
    return ckMalloc(size);
}


static void myFree(FT_Memory, void* ptr)
{
    ckFree(ptr);
}


static void* myRealloc(FT_Memory, long, long new_size, void* ptr)
{
    return ckRealloc(ptr, new_size);
}


bool ckLowLevelAPI::createFreeType()
{
    s_memory = static_cast<FT_Memory>(ckMalloc(sizeof(*s_memory)));
    s_memory->user = NULL;
    s_memory->alloc = myMalloc;
    s_memory->free = myFree;
    s_memory->realloc = myRealloc;

    if (FT_New_Library(s_memory, &s_freetype))
    {
        ckFree(s_memory);
        return false;
    }

    FT_Add_Default_Modules(s_freetype);

    return true;
}


bool ckLowLevelAPI::destroyFreeType()
{
    if (FT_Done_Library(s_freetype))
    {
        return false;
    }

    ckFree(s_memory);

    return true;
}


void* ckLowLevelAPI::newFreeTypeFont(const void* data, u32 data_size)
{
    FT_Face face0;

    if (FT_New_Memory_Face(s_freetype, static_cast<const FT_Byte*>(data), data_size, 0, &face0))
    {
        return NULL;
    }

    u32 face_num = face0->num_faces;

    void* font_info = ckMalloc(sizeof(u32) + sizeof(FT_Face) * face_num);

    *static_cast<u32*>(font_info) = face_num;

    FT_Face* faces = reinterpret_cast<FT_Face*>(static_cast<u32*>(font_info) + 1);

    faces[0] = face0;

    for (u32 i = 1; i < face_num; i++)
    {
        if (FT_New_Memory_Face(s_freetype, static_cast<const FT_Byte*>(data), data_size, i, &faces[i]))
        {
            return NULL;
        }
    }

    return font_info;
}


bool ckLowLevelAPI::deleteFreeTypeFont(void* font_info)
{
    u32 face_num = *static_cast<u32*>(font_info);
    FT_Face* faces = reinterpret_cast<FT_Face*>(static_cast<u32*>(font_info) + 1);

    for (u32 i = 0; i < face_num; i++)
    {
        if (FT_Done_Face(faces[i]))
        {
            return false;
        }
    }

    ckFree(font_info);

    return true;
}


struct RasterInfo
{
    void* image;
    u16 width;
    s32 pen_x, pen_y;
};


static void rasterSpanFunc(int y, int count, const FT_Span* spans, void* user)
{
    RasterInfo* raster_info = static_cast<RasterInfo*>(user);

    y = raster_info->pen_y - (y + 1);

    u8* dest_start = static_cast<u8*>(raster_info->image) + raster_info->width * y + raster_info->pen_x;

    for (s32 i = 0; i < count; i++)
    {
        const FT_Span* span = &spans[i];
        u8* dest = dest_start + span->x;
        u16 len = span->len;
        u8 c = span->coverage;

        for (s32 j = 0; j < len; j++)
        {
            *dest = c;

            dest++;
        }
    }
}


s16 ckLowLevelAPI::drawFreeTypeFont(void* image, u16 image_width, u16 image_height, //
    void* font_info, u32 font_index, u16 font_size, s16 x, s16 y, const wchar_t* str)
{
    FT_Face face = *(reinterpret_cast<FT_Face*>(static_cast<u32*>(font_info) + 1) + font_index);

    FT_Size_RequestRec size_req;
    size_req.type = FT_SIZE_REQUEST_TYPE_CELL;
    size_req.width = 0;
    size_req.height = font_size << 6;
    size_req.horiResolution = 0;
    size_req.vertResolution = 0;

    if (FT_Request_Size(face, &size_req))
    {
        return -1;
    }

    if (image)
    {
        s32 pen_x = x;
        s32 pen_y = y + static_cast<s32>(face->ascender * face->size->metrics.y_ppem / face->units_per_EM);

        RasterInfo raster_info;
        raster_info.image = image;
        raster_info.width = image_width;

        FT_Raster_Params raster_params = {};
        raster_params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_CLIP;
        raster_params.gray_spans = static_cast<FT_Raster_Span_Func>(rasterSpanFunc);
        raster_params.user = &raster_info;
        raster_params.clip_box.yMin = pen_y - image_height;
        raster_params.clip_box.yMax = pen_y;

        FT_GlyphSlot glyph_slot = face->glyph;

        for ( ; *str != L'\0'; str++)
        {
            if (FT_Load_Char(face, *str, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
            {
                return -1;
            }

            if (glyph_slot->format != FT_GLYPH_FORMAT_OUTLINE)
            {
                return -1;
            }

            raster_params.clip_box.xMin = -pen_x;
            raster_params.clip_box.xMax = image_width - pen_x;

            raster_info.pen_x = pen_x;
            raster_info.pen_y = pen_y;

            if (FT_Outline_Render(s_freetype, &glyph_slot->outline, &raster_params))
            {
                return -1;
            }

            pen_x += glyph_slot->advance.x >> 6;
        }

        return pen_x - x;
    }
    else
    {
        s32 pen_x = 0;
        FT_GlyphSlot glyph_slot = face->glyph;

        for ( ; *str != L'\0'; str++)
        {
            if (FT_Load_Char(face, *str, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
            {
                return -1;
            }

            pen_x += glyph_slot->advance.x >> 6;
        }

        return pen_x;
    }
}
