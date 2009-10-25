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


#if defined(WIN32)
#include <windows.h>
#include <GL/gl.h>
#include <glext.h>
#elif defined(PG_LINUX)
#include <GL/glx.h>
#elif defined(PG_MACOSX)
#include <GLUT/glut.h>
#elif defined(PG_IPHONE)
#include <OpenGLES/ES1/gl.h>
#else
#error
#endif

#include "pg_low_level_api.h"

#include "pg_math_all.h" // for pgMat::MulR32x16


#if !defined(PG_GLES1) && defined(PG_IPHONE)
#define PG_GLES1
#endif

#if !defined(PG_GLSL) && !defined(PG_GLES1)
#define PG_GLSL
#endif


#if defined(WIN32) || defined(PG_LINUX)

PFNGLACTIVETEXTUREARBPROC pglActiveTexture;
PFNGLATTACHOBJECTARBPROC pglAttachShader;
PFNGLCOMPILESHADERARBPROC pglCompileShader;
PFNGLCREATEPROGRAMOBJECTARBPROC pglCreateProgram;
PFNGLCREATESHADEROBJECTARBPROC pglCreateShader;
PFNGLDELETEOBJECTARBPROC pglDeleteObject;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC pglDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC pglEnableVertexAttribArray;
PFNGLGETATTRIBLOCATIONARBPROC pglGetAttribLocation;
PFNGLGETINFOLOGARBPROC pglGetInfoLog;
PFNGLGETOBJECTPARAMETERIVARBPROC pglGetObjectParameteriv;
PFNGLGETUNIFORMLOCATIONARBPROC pglGetUniformLocation;
PFNGLLINKPROGRAMARBPROC pglLinkProgram;
PFNGLSHADERSOURCEARBPROC pglShaderSource;
PFNGLUNIFORM1FARBPROC pglUniform1f;
PFNGLUNIFORM1IARBPROC pglUniform1i;
PFNGLUNIFORMMATRIX4FVARBPROC pglUniformMatrix4fv;
PFNGLUSEPROGRAMOBJECTARBPROC pglUseProgram;
PFNGLVERTEXATTRIBPOINTERARBPROC pglVertexAttribPointer;

#define pglDeleteProgram pglDeleteObject
#define pglDeleteShader pglDeleteObject
#define pglGetProgramInfoLog pglGetInfoLog
#define pglGetProgramiv pglGetObjectParameteriv
#define pglGetShaderInfoLog pglGetInfoLog
#define pglGetShaderiv pglGetObjectParameteriv

#define PGL_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB
#define PGL_FRAGMENT_SHADER GL_FRAGMENT_SHADER_ARB
#define PGL_INFO_LOG_LENGTH GL_OBJECT_INFO_LOG_LENGTH_ARB
#define PGL_LINK_STATUS GL_OBJECT_LINK_STATUS_ARB
#define PGL_TEXTURE0 GL_TEXTURE0_ARB
#define PGL_TEXTURE1 GL_TEXTURE1_ARB
#define PGL_TEXTURE2 GL_TEXTURE2_ARB
#define PGL_VERTEX_SHADER GL_VERTEX_SHADER_ARB

#else // !(WIN32 || PG_LINUX)

#define pglActiveTexture glActiveTexture
#define pglAttachShader glAttachShader
#define pglCompileShader glCompileShader
#define pglCreateProgram glCreateProgram
#define pglCreateShader glCreateShader
#define pglDeleteProgram glDeleteProgram
#define pglDeleteShader glDeleteShader
#define pglDisableVertexAttribArray glDisableVertexAttribArray
#define pglEnableVertexAttribArray glEnableVertexAttribArray
#define pglGetAttribLocation glGetAttribLocation
#define pglGetProgramInfoLog glGetProgramInfoLog
#define pglGetProgramiv glGetProgramiv
#define pglGetShaderInfoLog glGetShaderInfoLog
#define pglGetShaderiv glGetShaderiv
#define pglGetUniformLocation glGetUniformLocation
#define pglLinkProgram glLinkProgram
#define pglShaderSource glShaderSource
#define pglUniform1f glUniform1f
#define pglUniform1i glUniform1i
#define pglUniformMatrix4fv glUniformMatrix4fv
#define pglUseProgram glUseProgram
#define pglVertexAttribPointer glVertexAttribPointer

#define PGL_COMPILE_STATUS GL_COMPILE_STATUS
#define PGL_FRAGMENT_SHADER GL_FRAGMENT_SHADER
#define PGL_INFO_LOG_LENGTH GL_INFO_LOG_LENGTH
#define PGL_LINK_STATUS GL_LINK_STATUS
#define PGL_TEXTURE0 GL_TEXTURE0
#define PGL_TEXTURE1 GL_TEXTURE1
#define PGL_TEXTURE2 GL_TEXTURE2
#define PGL_VERTEX_SHADER GL_VERTEX_SHADER

#endif // WIN32 || PG_LINUX


static bool s_is_shader_available = false;

static pgLowLevelAPI::DepthTest s_depth_test;
static pgLowLevelAPI::BlendMode s_blend_mode;
static bool s_is_draw_rgb, s_is_draw_alpha, s_is_draw_depth;
static bool s_is_backface_culling;
static u32 s_tex_obj[3];
static bool s_is_bilinear[3];
static u32 s_shd_obj;

#ifdef PG_GLSL
static r32 s_model_view[16];
static r32 s_projection[16];
#endif

#ifndef PG_GLES2
static bool s_is_vertex_array_state;
static bool s_is_color_array_state;
static bool s_is_texcoord_array_state;
#endif


static void resetDepthTest()
{
    s_depth_test = pgLowLevelAPI::DEPTH_TEST_ALWAYS;

    pgLowLevelAPI::setDepthTest(pgLowLevelAPI::DEPTH_TEST_GEQUAL);
}


static void resetBlendMode()
{
    s_blend_mode = pgLowLevelAPI::BLEND_HALF;

    pgLowLevelAPI::setBlendMode(pgLowLevelAPI::BLEND_OFF);
}


static void resetWriteMode()
{
    s_is_draw_rgb = false;
    s_is_draw_alpha = true;
    s_is_draw_depth = false;

    pgLowLevelAPI::setWriteMode(true, false, true);
}


static void resetBackfaceCulling()
{
    s_is_backface_culling = true;

    pgLowLevelAPI::setBackfaceCulling(false);
}


static void resetTexture()
{
    s_tex_obj[0] = s_tex_obj[1] = s_tex_obj[2] = 1;
    s_is_bilinear[0] = s_is_bilinear[1] = s_is_bilinear[2] = true;

    pgLowLevelAPI::setTexture(0, 0, 0, false);
}


static void resetShader()
{
    s_shd_obj = 1;

    pgLowLevelAPI::setShader(0);
}


#ifndef PG_GLES2

static void setVertexArrayState(bool is_vertex_array_state)
{
    if (s_is_vertex_array_state != is_vertex_array_state)
    {
        s_is_vertex_array_state = is_vertex_array_state;

        if (s_is_vertex_array_state)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
        }
        else
        {
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }
}


static void setColorArrayState(bool is_color_array_state)
{
    if (s_is_color_array_state != is_color_array_state)
    {
        s_is_color_array_state = is_color_array_state;

        if (s_is_color_array_state)
        {
            glEnableClientState(GL_COLOR_ARRAY);
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
        }
    }
}


static void setTexCoordArrayState(bool is_texcoord_array_state)
{
    if (s_is_texcoord_array_state != is_texcoord_array_state)
    {
        s_is_texcoord_array_state = is_texcoord_array_state;

        if (s_is_texcoord_array_state)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        else
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
}


static void resetVertexArrayState()
{
    s_is_vertex_array_state = true;

    setVertexArrayState(false);
}


static void resetColorArrayState()
{
    s_is_color_array_state = true;

    setColorArrayState(false);
}


static void resetTexCoordArrayState()
{
    s_is_texcoord_array_state = true;

    setTexCoordArrayState(false);
}

#endif // !PG_GLES2


void pgLowLevelAPI::setupShaderAPI(bool is_shader_check)
{
    if (!is_shader_check)
    {
        s_is_shader_available = false;
        return;
    }

#if defined(WIN32) || defined(PG_LINUX)

#ifdef WIN32
#define pglGetProcAddress(x) wglGetProcAddress(x)
#else // PG_LINUX
#define pglGetProcAddress(x) (*glXGetProcAddress)((const GLubyte*)(x))
#endif

    pglActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREARBPROC>(pglGetProcAddress("glActiveTextureARB"));
    pglAttachShader = reinterpret_cast<PFNGLATTACHOBJECTARBPROC>(pglGetProcAddress("glAttachObjectARB"));
    pglCompileShader = reinterpret_cast<PFNGLCOMPILESHADERARBPROC>(pglGetProcAddress("glCompileShaderARB"));
    pglCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMOBJECTARBPROC>(pglGetProcAddress("glCreateProgramObjectARB"));
    pglCreateShader = reinterpret_cast<PFNGLCREATESHADEROBJECTARBPROC>(pglGetProcAddress("glCreateShaderObjectARB"));
    pglDeleteObject = reinterpret_cast<PFNGLDELETEOBJECTARBPROC>(pglGetProcAddress("glDeleteObjectARB"));
    pglDisableVertexAttribArray = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>(pglGetProcAddress("glDisableVertexAttribArrayARB"));
    pglEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC>(pglGetProcAddress("glEnableVertexAttribArrayARB"));
    pglGetAttribLocation = reinterpret_cast<PFNGLGETATTRIBLOCATIONARBPROC>(pglGetProcAddress("glGetAttribLocationARB"));
    pglGetInfoLog = reinterpret_cast<PFNGLGETINFOLOGARBPROC>(pglGetProcAddress("glGetInfoLogARB"));
    pglGetObjectParameteriv = reinterpret_cast<PFNGLGETOBJECTPARAMETERIVARBPROC>(pglGetProcAddress("glGetObjectParameterivARB"));
    pglGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONARBPROC>(pglGetProcAddress("glGetUniformLocationARB"));
    pglLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMARBPROC>(pglGetProcAddress("glLinkProgramARB"));
    pglShaderSource = reinterpret_cast<PFNGLSHADERSOURCEARBPROC>(pglGetProcAddress("glShaderSourceARB"));
    pglUniform1f = reinterpret_cast<PFNGLUNIFORM1FARBPROC>(pglGetProcAddress("glUniform1fARB"));
    pglUniform1i = reinterpret_cast<PFNGLUNIFORM1IARBPROC>(pglGetProcAddress("glUniform1iARB"));
    pglUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVARBPROC>(pglGetProcAddress("glUniformMatrix4fvARB"));
    pglUseProgram = reinterpret_cast<PFNGLUSEPROGRAMOBJECTARBPROC>(pglGetProcAddress("glUseProgramObjectARB"));
    pglVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>(pglGetProcAddress("glVertexAttribPointerARB"));

    if (pglActiveTexture && //
        pglAttachShader && //
        pglCompileShader && //
        pglCreateProgram && //
        pglCreateShader && //
        pglDeleteObject && //
        pglDisableVertexAttribArray && //
        pglEnableVertexAttribArray && //
        pglGetAttribLocation && //
        pglGetInfoLog && //
        pglGetObjectParameteriv && //
        pglGetUniformLocation && //
        pglLinkProgram && //
        pglShaderSource && //
        pglUniform1f && //
        pglUniform1i && //
        pglUniformMatrix4fv && //
        pglUseProgram && //
        pglVertexAttribPointer)
    {
        s_is_shader_available = true;
    }
    else
    {
        s_is_shader_available = false;
    }

#elif defined(PG_GLSL)

    s_is_shader_available = true;

#else // !PG_GLSL

    s_is_shader_available = false;

#endif // WIN32 || PG_LINUX
}


void pgLowLevelAPI::resetDrawState()
{
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

#ifndef PG_GLES2
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif

#if defined(PG_GLES1) || defined(PG_GLES2)
    glClearDepthf(-1.0f);
#else
    glClearDepth(-1.0f);
#endif

    resetDepthTest();
    resetBlendMode();
    resetWriteMode();
    resetBackfaceCulling();
    resetTexture();
    resetShader();

#ifndef PG_GLES2
    resetVertexArrayState();
    resetColorArrayState();
    resetTexCoordArrayState();
#endif
}


bool pgLowLevelAPI::isShaderAvailable()
{
    return s_is_shader_available;
}


void pgLowLevelAPI::setViewport(s16 left, s16 top, u16 width, u16 height)
{
    s16 bottom = getFramebufferHeight() - (top + height);

    glViewport(left, bottom, width, height);
    glScissor(left, bottom, width, height);
}


void pgLowLevelAPI::clearFramebuffer(bool is_clear_color, bool is_clear_depth, const u8* clear_color)
{
    GLbitfield mask = 0;

    if (is_clear_color)
    {
        glClearColor( //
            static_cast<r32>(clear_color[0]) / 255.0f, //
            static_cast<r32>(clear_color[1]) / 255.0f, //
            static_cast<r32>(clear_color[2]) / 255.0f, //
            static_cast<r32>(clear_color[3]) / 255.0f);

        mask |= GL_COLOR_BUFFER_BIT;
    }

    if (is_clear_depth)
    {
        mask |= GL_DEPTH_BUFFER_BIT;
    }

    setWriteMode(is_clear_color, is_clear_color, is_clear_depth);

    glClear(mask);
}


void pgLowLevelAPI::setDepthTest(DepthTest depth_test)
{
    if (s_depth_test != depth_test)
    {
        s_depth_test = depth_test;

        static GLenum gl_depth_test[] =
        {
            GL_ALWAYS, //
            GL_LESS, //
            GL_GREATER, //
            GL_LEQUAL, //
            GL_GEQUAL
        };

        glDepthFunc(gl_depth_test[s_depth_test]);
    }
}


void pgLowLevelAPI::setBlendMode(BlendMode blend_mode)
{
    if (s_blend_mode != blend_mode)
    {
        s_blend_mode = blend_mode;

        switch (s_blend_mode)
        {
        case BLEND_OFF:
            glDisable(GL_BLEND);
            break;

        case BLEND_HALF:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        case BLEND_ADD:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        case BLEND_DEST_ALPHA:
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
            break;
        }
    }
}


void pgLowLevelAPI::setWriteMode(bool is_draw_rgb, bool is_draw_alpha, bool is_draw_depth)
{
    if (s_is_draw_rgb != is_draw_rgb || s_is_draw_alpha != is_draw_alpha)
    {
        s_is_draw_rgb = is_draw_rgb;
        s_is_draw_alpha = is_draw_alpha;

        glColorMask(s_is_draw_rgb, s_is_draw_rgb, s_is_draw_rgb, s_is_draw_alpha);
    }

    if (s_is_draw_depth != is_draw_depth)
    {
        s_is_draw_depth = is_draw_depth;

        glDepthMask(s_is_draw_depth);
    }
}


void pgLowLevelAPI::setBackfaceCulling(bool is_backface_culling)
{
    if (s_is_backface_culling != is_backface_culling)
    {
        s_is_backface_culling = is_backface_culling;

        if (s_is_backface_culling)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }
}


u16 pgLowLevelAPI::getMaxTextureLength()
{
    return 2048;
}


u16 pgLowLevelAPI::getValidTextureLength(u16 length)
{
    if (length >= getMaxTextureLength())
    {
        return getMaxTextureLength();
    }

    u16 valid_length = 8;

    while (length > valid_length)
    {
        valid_length *= 2;
    }

    return valid_length;
}


u16 pgLowLevelAPI::getTexturePixelSize(TextureFormat format)
{
    static const u16 s_pixel_size[] =
    {
        3, 4, 1
    };

    return s_pixel_size[format];
}


u32 pgLowLevelAPI::registerTexture(u16 width, u16 height, TextureFormat format, const void* image)
{
    glEnable(GL_TEXTURE_2D);

    u32 tex_obj;

    glGenTextures(1, &tex_obj);
    glBindTexture(GL_TEXTURE_2D, tex_obj);

    glGetError();

    if (image)
    {
        static const GLint s_gl_internal_format[] =
        {
            GL_RGB, GL_RGBA, GL_ALPHA
        };

        glTexImage2D(GL_TEXTURE_2D, 0, s_gl_internal_format[format], width, height, 0, s_gl_internal_format[format], GL_UNSIGNED_BYTE, image);
    }

    if (glGetError() != GL_NO_ERROR)
    {
        tex_obj = 0;
    }

    resetTexture();

    return tex_obj;
}


void pgLowLevelAPI::unregisterTexture(u32 tex_obj)
{
    glDeleteTextures(1, &tex_obj);

    resetTexture();
}


void pgLowLevelAPI::setTexture(u32 tex_obj1, u32 tex_obj2, u32 tex_obj3, bool is_bilinear)
{
#define SETUP_TEXTURE(index, tex_obj) \
    do \
    { \
        if (s_tex_obj[index] != tex_obj || (s_tex_obj[index] != 0 && s_is_bilinear[index] != is_bilinear)) \
        { \
            s_tex_obj[index] = tex_obj; \
            s_is_bilinear[index] = is_bilinear; \
    \
            if (tex_obj == 0) \
            { \
                glDisable(GL_TEXTURE_2D); \
            } \
            else \
            { \
                glEnable(GL_TEXTURE_2D); \
    \
                glBindTexture(GL_TEXTURE_2D, tex_obj); \
    \
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); \
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); \
    \
                if (is_bilinear) \
                { \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); \
                } \
                else \
                { \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); \
                } \
            } \
        } \
    } \
    while (false)

    SETUP_TEXTURE(0, tex_obj1);

#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglActiveTexture(PGL_TEXTURE1);
        SETUP_TEXTURE(1, tex_obj2);

        pglActiveTexture(PGL_TEXTURE2);
        SETUP_TEXTURE(2, tex_obj3);

        pglActiveTexture(PGL_TEXTURE0);
    }
#endif

#undef SETUP_TEXTURE
}


void pgLowLevelAPI::copyImageFromFramebufferToTexture( //
    u32 tex_obj, u16 tex_width, u16 tex_height, TextureFormat tex_format, u16 left_in_framebuffer, u16 top_in_framebuffer)
{
    if ((tex_obj != 0 && tex_format == FORMAT_RGB) || tex_format == FORMAT_RGBA)
    {
        setTexture(tex_obj, 0, 0, true);

        glCopyTexImage2D(GL_TEXTURE_2D, 0, (tex_format == FORMAT_RGB) ? GL_RGB : GL_RGBA, //
            left_in_framebuffer, getFramebufferHeight() - (top_in_framebuffer + tex_height), tex_width, tex_height, 0);
    }
}


void pgLowLevelAPI::setProjection(const r32* projection)
{
#ifndef PG_GLES2
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
#endif

#ifdef PG_GLSL
    memcpy(s_projection, projection, sizeof(s_projection));
#endif
}


void pgLowLevelAPI::setModelView(const r32* model_view)
{
#ifndef PG_GLES2
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(model_view);
#endif

#ifdef PG_GLSL
    memcpy(s_model_view, model_view, sizeof(s_model_view));
#endif
}


void pgLowLevelAPI::setVertexPointer(u32 stride, const r32* vert)
{
#ifndef PG_GLES2
    if (vert)
    {
        setVertexArrayState(true);
        glVertexPointer(3, GL_FLOAT, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(vert));
    }
    else
    {
        setVertexArrayState(false);
    }
#endif
}


void pgLowLevelAPI::setColorPointer(u32 stride, const u8* color)
{
#ifndef PG_GLES2
    if (color)
    {
        setColorArrayState(true);
        glColorPointer(4, GL_UNSIGNED_BYTE, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(color));
    }
    else
    {
        setColorArrayState(false);
    }
#endif
}


void pgLowLevelAPI::setTexCoordPointer(u32 stride, const r32* uv)
{
#ifndef PG_GLES2
    if (uv)
    {
        setTexCoordArrayState(true);
        glTexCoordPointer(2, GL_FLOAT, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(uv));
    }
    else
    {
        setTexCoordArrayState(false);
    }
#endif
}


void pgLowLevelAPI::drawArrays(DrawMode draw_mode, u16 first, u16 count)
{
    static const GLenum s_gl_draw_mode[] =
    {
        GL_POINTS, //
        GL_LINES, //
        GL_LINE_LOOP, //
        GL_LINE_STRIP, //
        GL_TRIANGLES, //
        GL_TRIANGLE_STRIP, //
        GL_TRIANGLE_FAN, //
    };

    glDrawArrays(s_gl_draw_mode[draw_mode], static_cast<GLint>(first), static_cast<GLsizei>(count));
}


u32 pgLowLevelAPI::registerShader(const char* vert_code, const char* frag_code)
{
#ifdef PG_GLSL

#define PRINT_LOG(type, obj, msg) \
    do \
    { \
        GLint buf_size; \
        pglGet##type##iv(obj, PGL_INFO_LOG_LENGTH, &buf_size); \
    \
        if (buf_size > 1) \
        { \
            char* buf = static_cast<char*>(pgLowLevelAPI::malloc(buf_size)); \
            GLint wrote_size; \
    \
            pglGet##type##InfoLog(obj, buf_size, &wrote_size, buf); \
    \
            pgLowLevelAPI::printf(msg); \
            pgLowLevelAPI::printf("\n"); \
            pgLowLevelAPI::printf(buf); \
    \
            pgLowLevelAPI::free(buf); \
        } \
    } \
    while (false)

    if (s_is_shader_available)
    {
        u32 vert_obj = 0;

        if (vert_code)
        {
            vert_obj = pglCreateShader(PGL_VERTEX_SHADER);
            pglShaderSource(vert_obj, 1, &vert_code, NULL);
            pglCompileShader(vert_obj);

            GLint is_compile;
            pglGetShaderiv(vert_obj, PGL_COMPILE_STATUS, &is_compile);

            if (!is_compile)
            {
                PRINT_LOG(Shader, vert_obj, "*** vertex shader compile error ***");
                pglDeleteShader(vert_obj);

                return 0;
            }
        }

        u32 frag_obj = 0;

        if (frag_code)
        {
            frag_obj = pglCreateShader(PGL_FRAGMENT_SHADER);
            pglShaderSource(frag_obj, 1, &frag_code, NULL);
            pglCompileShader(frag_obj);

            GLint is_compile;
            pglGetShaderiv(frag_obj, PGL_COMPILE_STATUS, &is_compile);

            if (!is_compile)
            {
                PRINT_LOG(Shader, frag_obj, "*** fragment shader compile error ***");
                pglDeleteShader(frag_obj);

                if (vert_code)
                {
                    pglDeleteShader(vert_obj);
                }

                return 0;
            }
        }

        u32 prog_obj = pglCreateProgram();

        if (vert_code)
        {
            pglAttachShader(prog_obj, vert_obj);
            pglDeleteShader(vert_obj);
        }

        if (frag_code)
        {
            pglAttachShader(prog_obj, frag_obj);
            pglDeleteShader(frag_obj);
        }

        pglLinkProgram(prog_obj);

        GLint is_link;
        pglGetProgramiv(prog_obj, PGL_LINK_STATUS, &is_link);

        if (!is_link)
        {
            PRINT_LOG(Program, prog_obj, "*** shader link error ***");
            pglDeleteProgram(prog_obj);

            return 0;
        }

        return prog_obj;
    }
    else
    {
        return 0;
    }

#undef PRINT_LOG

#else

    return 0;

#endif // PG_GLSL
}


void pgLowLevelAPI::unregisterShader(u32 shd_obj)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglDeleteShader(shd_obj);
    }
#endif
}


void pgLowLevelAPI::setShader(u32 shd_obj)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        if (s_shd_obj != shd_obj)
        {
            s_shd_obj = shd_obj;

            pglUseProgram(s_shd_obj);
        }
    }
#endif
}


u32 pgLowLevelAPI::getUniformLocation(u32 shd_obj, const char* name)
{
#ifdef PG_GLSL
    return s_is_shader_available ? pglGetUniformLocation(shd_obj, name) : 0;
#else
    return 0;
#endif
}


void pgLowLevelAPI::setUniform_s32(u32 location, s32 uniform)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglUniform1i(location, uniform);
    }
#endif
}


void pgLowLevelAPI::setUniform_r32(u32 location, r32 uniform)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglUniform1f(location, uniform);
    }
#endif
}


void pgLowLevelAPI::setUniform_localToScreen(u32 location)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        r32 local_to_screen[16];

        pgMat::mulR32x16(local_to_screen, s_projection, s_model_view);

        pglUniformMatrix4fv(location, 1, GL_FALSE, local_to_screen);
    }
#endif
}


u32 pgLowLevelAPI::getAttribLocation(u32 shd_obj, const char* name)
{
#ifdef PG_GLSL
    return s_is_shader_available ? pglGetAttribLocation(shd_obj, name) : 0;
#else
    return 0;
#endif
}


void pgLowLevelAPI::setAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglEnableVertexAttribArray(location);
        pglVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, attrib);
    }
#endif
}


void pgLowLevelAPI::setAttribPointer_color(u32 location, u32 stride, const u8* attrib)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglEnableVertexAttribArray(location);
        pglVertexAttribPointer(location, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, attrib);
    }
#endif
}


void pgLowLevelAPI::disableAttribPointer(u32 location)
{
#ifdef PG_GLSL
    if (s_is_shader_available)
    {
        pglDisableVertexAttribArray(location);
    }
#endif
}
