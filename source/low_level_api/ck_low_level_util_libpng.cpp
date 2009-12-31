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
#include <png.h>
}

#include "ck_low_level_api.h"

#include "ck_mem_all.h" // for ckMalloc, ckFree


static png_voidp myMalloc(png_structp, png_size_t size)
{
    return ckMalloc(static_cast<u32>(size));
}


static void myFree(png_structp, png_voidp ptr)
{
    ckFree(ptr);
}


struct MemoryStream
{
    png_bytep data;
    png_size_t size;
    png_size_t pos;
};


static void readData(png_structp png_ptr, png_bytep dest, png_size_t read_size)
{
    MemoryStream* ms = static_cast<MemoryStream*>(png_get_io_ptr(png_ptr));

    if (ms->pos + read_size > ms->size)
    {
        png_error(png_ptr, "out of data");
    }

    ckLowLevelAPI::memcpy(dest, ms->data + ms->pos, static_cast<u32>(read_size));
    ms->pos += read_size;
}


static bool readPNG(u16* width, u16* height, ckLowLevelAPI::TextureFormat* format, //
    void* buf, size_t buf_size, size_t buf_line_size, const void* data, u32 data_size)
{
    const u32 PNG_SIGNATURE_SIZE = 8;

    if (!png_check_sig(static_cast<png_bytep>(const_cast<void*>(data)), PNG_SIGNATURE_SIZE))
    {
        return false;
    }

    png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL, NULL, myMalloc, myFree);

    if (!png_ptr)
    {
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        return false;
    }

    png_bytepp row_ptrs = NULL;

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        if (row_ptrs)
        {
            ckFree(row_ptrs);
        }

        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        return false;
    }

    MemoryStream ms;
    ms.data = static_cast<png_bytep>(const_cast<void*>(data));
    ms.size = data_size;
    ms.pos = PNG_SIGNATURE_SIZE;

    png_set_read_fn(png_ptr, &ms, readData);
    png_set_sig_bytes(png_ptr, PNG_SIGNATURE_SIZE);

    png_read_info(png_ptr, info_ptr);

    png_uint_32 width_u32, height_u32;
    int bit_depth, color_type;

    png_get_IHDR(png_ptr, info_ptr, &width_u32, &height_u32, &bit_depth, &color_type, int_p_NULL, int_p_NULL, int_p_NULL);

    if (width)
    {
        *width = static_cast<u16>(width_u32);
    }

    if (height)
    {
        *height = static_cast<u16>(height_u32);
    }

    png_set_expand(png_ptr);

    if (bit_depth == 16)
    {
        png_set_strip_16(png_ptr);
    }

    if ((color_type == PNG_COLOR_TYPE_GRAY && png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) || //
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);

    png_byte channels = png_get_channels(png_ptr, info_ptr);

    if (format)
    {
        if (channels == 3)
        {
            *format = ckLowLevelAPI::FORMAT_RGB;
        }
        else if (channels == 4)
        {
            *format = ckLowLevelAPI::FORMAT_RGBA;
        }
        else if (channels == 1)
        {
            *format = ckLowLevelAPI::FORMAT_ALPHA;
        }
        else
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
            return false;
        }
    }

    if (buf)
    {
        if (width_u32 * height_u32 * channels > buf_size)
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
            return false;
        }

        row_ptrs = static_cast<png_bytepp>(ckMalloc(sizeof(png_bytep) * height_u32));

        for (u32 i = 0; i < height_u32; i++)
        {
            row_ptrs[i] = static_cast<u8*>(buf) + i * buf_line_size;
        }

        png_read_image(png_ptr, row_ptrs);
        png_read_end(png_ptr, info_ptr);

        ckFree(row_ptrs);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    return true;
}


bool ckLowLevelAPI::readPNGInfo(u16* width, u16* height, TextureFormat* format, const void* data, u32 data_size)
{
    return readPNG(width, height, format, NULL, 0, 0, data, data_size);
}


bool ckLowLevelAPI::readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size)
{
    return readPNG(NULL, NULL, NULL, buf, buf_size, buf_line_size, data, data_size);
}
