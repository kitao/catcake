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


#ifndef CK_LOW_LEVEL_API_H_
#define CK_LOW_LEVEL_API_H_


#include "ck_def.h"


class ckLowLevelAPI
{
public:
    /*
        Math
    */
    static r32 sqrt(r32 x);
    static r32 sin(r32 rad);
    static r32 cos(r32 rad);
    static r32 asin(r32 x);
    static r32 acos(r32 x);
    static r32 atan2(r32 y, r32 x);
    static void srand(s32 seed);
    static s32 rand();

    /*
        Memory
    */
    static void* malloc(u32 size);
    static void* realloc(void* ptr, u32 size);
    static void free(void* ptr);
    static void memset(void* buf, u8 value, u32 size);
    static void memcpy(void* dest, const void* src, u32 size);

    /*
        System
    */
    static bool createApplication(const char* title, u16 width, u16 height, u16 sys_flag);
    static void destroyApplication();
    static void startApplication(bool (*update_func)(void));

    static u16 getFramebufferWidth();
    static u16 getFramebufferHeight();
    static void updateFramebufferSize();
    static bool isFramebufferSizeChanged();
    static void swapFramebuffer();
    static bool isFullScreen();
    static bool toggleFullScreen(u16 width, u16 height);

    typedef void (*KeyEventHandler)(u8 keycode, bool is_down);
    typedef void (*MouseEventHandler)(s16 mouse_x, s16 mouse_y);
    typedef void (*ExtraEventHandler)(u8 index, r32 value);

    static void setKeyEventHandler(KeyEventHandler handler);
    static void setMouseEventHandler(MouseEventHandler handler);
    static void setExtraEventHandler(ExtraEventHandler handler);
    static void setMousePos(s16 mouse_x, s16 mouse_y);
    static bool isMouseVisible();
    static void setMouseVisible(bool is_visible);

    static u64 getUsecTime();
    static void sleepUsec(u64 usec);
    static void exit(s32 status);
    static void error(const char* msg);
    static void readLittleEndian(void* dest, const void* src, u32 size);
    static void writeLittleEndian(void* dest, const void* src, u32 size);

    static void* newThread(void (*start_func)(void*), void* user_param);
    static void deleteThread(void* thread_handler);
    static void joinThread(void* thread_handler);
    static void* newMutex();
    static void deleteMutex(void* mutex_handler);
    static void lockMutex(void* mutex_handler);
    static void unlockMutex(void* mutex_handler);

    static void setInitialDirectory(s32 argc, char** argv);
    static void getWindowsFontDirectory(char* buf, u32 buf_size);

    /*
        IO
    */
    enum FileMode
    {
        FILE_MODE_READ, //
        FILE_MODE_WRITE
    };

    static void printf(const char* msg);
    static void wprintf(const wchar_t* msg);
    static void vsprintf(char* buf, u32 buf_size, const char* format, void* arg);
    static void vswprintf(wchar_t* buf, u32 buf_size, const wchar_t* format, void* arg);

    static void* openFile(const char* filename, FileMode file_mode);
    static s32 getFileSize(void* file_handler);
    static bool readFile(void* buf, u32 offset, u32 size, void* file_handler);
    static bool writeFile(u32 offset, const void* buf, u32 size, void* file_handler);
    static void closeFile(void* file_handler);

    /*
        Draw
    */
    enum DepthTest
    {
        DEPTH_TEST_ALWAYS, //
        DEPTH_TEST_LESS, //
        DEPTH_TEST_GREATER, //
        DEPTH_TEST_LEQUAL, //
        DEPTH_TEST_GEQUAL
    };

    enum BlendMode
    {
        BLEND_OFF, //
        BLEND_HALF, //
        BLEND_ADD, //
        BLEND_DEST_ALPHA
    };

    enum DrawMode
    {
        DRAW_POINTS, //
        DRAW_LINES, //
        DRAW_LINE_LOOP, //
        DRAW_LINE_STRIP, //
        DRAW_TRIANGLES, //
        DRAW_TRIANGLE_STRIP, //
        DRAW_TRIANGLE_FAN
    };

    enum TextureFormat
    {
        FORMAT_RGB, //
        FORMAT_RGBA, //
        FORMAT_ALPHA
    };

    static void setupShaderAPI(bool is_shader_check);
    static void resetDrawState();
    static bool isShaderAvailable();
    static void setViewport(s16 left, s16 top, u16 width, u16 height);
    static void clearFramebuffer(bool is_clear_color, bool is_clear_depth, const u8* clear_color);

    static void setDepthTest(DepthTest depth_test);
    static void setBlendMode(BlendMode blend_mode);
    static void setWriteMode(bool is_draw_rgb, bool is_draw_alpha, bool is_draw_depth);
    static void setBackfaceCulling(bool is_backface_culling);

    static u16 getMaxTextureLength();
    static u16 getValidTextureLength(u16 length);
    static u16 getTexturePixelSize(TextureFormat format);
    static u32 registerTexture(u16 width, u16 height, TextureFormat format, const void* image);
    static void unregisterTexture(u32 tex_obj);
    static void setTexture(u32 tex_obj1, u32 tex_obj2, u32 tex_obj3, bool is_bilinear);
    static void copyImageFromFramebufferToTexture( //
        u32 tex_obj, u16 tex_width, u16 tex_height, TextureFormat tex_format, u16 left_in_framebuffer, u16 top_in_framebuffer);

    static void setProjection(const r32* projection);
    static void setModelView(const r32* model_view);

    static void setVertexPointer(u32 stride, const r32* vert);
    static void setColorPointer(u32 stride, const u8* color);
    static void setTexCoordPointer(u32 stride, const r32* uv);

    static void drawArrays(DrawMode draw_mode, u16 first, u16 count);

    static u32 registerShader(const char* vert_code, const char* frag_code);
    static void unregisterShader(u32 shd_obj);
    static void setShader(u32 shd_obj);
    static u32 getUniformLocation(u32 shd_obj, const char* name);
    static void setUniform_s32(u32 location, s32 uniform);
    static void setUniform_r32(u32 location, r32 uniform);
    static void setUniform_localToScreen(u32 location);
    static u32 getAttribLocation(u32 shd_obj, const char* name);
    static void setAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib);
    static void setAttribPointer_color(u32 location, u32 stride, const u8* attrib);
    static void disableAttribPointer(u32 location);

    /*
        Sound
    */
    typedef bool (*SoundMixFunction)(void* snd_mix_buf);

    static bool isSoundDeviceOpen();
    static bool openSoundDevice(u8 channel_num, u16 sample_rate, u16 snd_mix_buf_msec, SoundMixFunction snd_mix_func);
    static void closeSoundDevice();

    static u8 getSoundDeviceChannelNum();
    static u16 getSoundDeviceSampleRate();
    static u16 getSoundMixBufferMsec();
    static u32 getSoundMixBufferSize();
    static u32 getSoundMixBufferSampleNum();

    static void lockSoundMixMutex();
    static void unlockSoundMixMutex();

    /*
        Utility
    */
    static bool readPNGInfo(u16* width, u16* height, TextureFormat* format, const void* data, u32 data_size);
    static bool readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size);

    static bool createFreeType();
    static bool destroyFreeType();
    static void* newFreeTypeFont(const void* data, u32 data_size);
    static bool deleteFreeTypeFont(void* font_info);
    static s16 drawFreeTypeFont(void* image, u16 image_width, u16 image_height, //
        void* font_info, u32 font_index, u16 font_size, s16 x, s16 y, const wchar_t* str);
};


#endif // !CK_LOW_LEVEL_API_H_
