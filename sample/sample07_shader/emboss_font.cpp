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


#include "pogolyn.h"


static const char s_vert_code[] = //
    "uniform mat4 pg_local_to_screen;" //
    "" //
    "attribute vec4 pg_vertex;" //
    "attribute vec4 pg_color;" //
    "attribute vec2 pg_texcoord;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main(void)" //
    "{" //
    "	gl_Position = pg_local_to_screen * pg_vertex;" //
    "" //
    "	vary_color = pg_color;" //
    "	vary_texcoord = pg_texcoord;" //
    "}";


static const char s_frag_code[] = //
    "uniform float pg_uni_00, pg_uni_01;" // texture size
    "uniform float pg_uni_02, pg_uni_03;" // texcoord offset
    "" //
    "uniform sampler2D pg_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "	const vec2 uv1 = vary_texcoord;" //
    "	const vec2 uv2 = uv1 + vec2(pg_uni_02 / pg_uni_00, pg_uni_03 / pg_uni_01);" //
    "" //
    "	gl_FragColor.rgb = vary_color.rgb;" //
    "	gl_FragColor.a = (texture2D(pg_tex_00, uv1).a - texture2D(pg_tex_00, uv2).a) * vary_color.a;" //
    "}";


class EmbossFont : public pgTask
{
public:
    EmbossFont();
    virtual ~EmbossFont();

private:
    virtual void onUpdate();

    pgSprt m_bg_sprt;

    pgFont m_font;
    pgPrim m_font_prim;
    pgRend_Shader m_font_rend;

    bool m_is_uv_rot;
    s32 m_uv_rot;
};


void newEmbossFont()
{
    pgNewTask(EmbossFont);
}


EmbossFont::EmbossFont() : pgTask(ORDER_ZERO)
{
    pgDrawMgr::newShader(pgID_("emboss.shd"), s_vert_code, s_frag_code, 4, 0, 1);

    m_bg_sprt.init(1, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_bg_sprt.setTextureID(pgID_("bg_512x512.png"));
    m_bg_sprt.setDataSize(0, 640.0f, 480.0f);

    pgDrawMgr::setFont(pgID_("stonsans.ttf"), 0);
    pgDrawMgr::setFontSize(49);

    m_font.init(512, 64);
    r32 draw_width = m_font.drawString(0, 0, pgDrawMgr::isShaderAvailable() ? "Pogolyn Shader Sample" : "Shader Not Available");

    m_font_prim.init(pgPrim::MODE_TRIANGLE_FAN, 4, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
    m_font_prim.setTextureID(m_font.getTextureID());
    m_font_prim.setPreset_defaultBlendHalf();
    m_font_prim.setDataRect(0, pgVec::ZERO, 512.0f, 64.0f, pgCol::FULL, 0.0f, 0.0f, 1.0f, 1.0f);
    m_font_prim.local().trans.set((512.0f - draw_width) / 2.0f, -7.0f, 10.0f);

    m_font_rend.init(&m_font_prim, pgID_("emboss.shd"));
    m_font_rend.uniform(0) = 512.0f;
    m_font_rend.uniform(1) = 64.0f;
    m_font_rend.uniform(2) = 0.0f;
    m_font_rend.uniform(3) = 0.0f;

    m_is_uv_rot = true;
    m_uv_rot = 0;
}


EmbossFont::~EmbossFont()
{
    pgDrawMgr::deleteShader(pgID_("emboss.shd"));
}


void EmbossFont::onUpdate()
{
    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_F))
    {
        pgSysMgr::toggleFullScreen(640, 480);
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_Q))
    {
        pgEndPogolyn();
    }

    if (pgKeyMgr::isPressed(pgKeyMgr::KEY_U))
    {
        m_is_uv_rot = !m_is_uv_rot;
    }

    if (m_is_uv_rot)
    {
        m_uv_rot += 2;

        if (m_uv_rot > 360)
        {
            m_uv_rot -= 360;
        }
    }

    m_font_rend.uniform(2) = pgMath::cos_s32(m_uv_rot) * 2.0f;
    m_font_rend.uniform(3) = pgMath::sin_s32(m_uv_rot) * 1.0f;
}
