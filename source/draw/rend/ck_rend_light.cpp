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


#include "pg_draw_all.h"

#include "pg_util_all.h"


static const pgID s_rend_light_shader_id = pgID_("REND_LIGHT_SHADER");


static const char s_vert_code[] = //
    "uniform mat4 pg_local_to_screen;" //
    "" //
    "attribute vec4 pg_vertex;" //
    "attribute vec4 pg_color;" //
    "attribute vec2 pg_texcoord;" //
    "" //
    "uniform float pg_uni_00, pg_uni_01, pg_uni_02, pg_uni_03;" // final color
    "" //
    "uniform float pg_uni_04, pg_uni_05;" // u param
    "uniform float pg_uni_06, pg_uni_07;" // v param
    "" //
    "uniform float pg_uni_09, pg_uni_10, pg_uni_11;" // ambient color
    "" //
    "uniform int pg_uni_12;" // light number
    "" //
    "uniform float pg_uni_13, pg_uni_14, pg_uni_15;" // light1 direction
    "uniform float pg_uni_16, pg_uni_17, pg_uni_18;" // light1 color
    "" //
    "uniform float pg_uni_19, pg_uni_20, pg_uni_21;" // light2 direction
    "uniform float pg_uni_22, pg_uni_23, pg_uni_24;" // light2 color
    "" //
    "uniform float pg_uni_25, pg_uni_26, pg_uni_27;" // light3 direction
    "uniform float pg_uni_28, pg_uni_29, pg_uni_30;" // light3 color
    "" //
    "attribute vec3 pg_att_00;" // normal
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_Position = pg_local_to_screen * pg_vertex;" //
    "" //
    "    vary_color = vec4(pg_uni_09, pg_uni_10, pg_uni_11, 255.0) / 255.0;" //
    "" //
    "    if (pg_uni_12 >= 1)" //
    "    {" //
    "        float lumi = max(-dot(vec3(pg_uni_13, pg_uni_14, pg_uni_15), pg_att_00), 0.0);" //
    "        vary_color += vec4(pg_uni_16, pg_uni_17, pg_uni_18, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    if (pg_uni_12 >= 2)" //
    "    {" //
    "        float lumi = max(-dot(vec3(pg_uni_19, pg_uni_20, pg_uni_21), pg_att_00), 0.0);" //
    "        vary_color += vec4(pg_uni_22, pg_uni_23, pg_uni_24, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    if (pg_uni_12 >= 3)" //
    "    {" //
    "        float lumi = max(-dot(vec3(pg_uni_25, pg_uni_26, pg_uni_27), pg_att_00), 0.0);" //
    "        vary_color += vec4(pg_uni_28, pg_uni_29, pg_uni_30, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    vary_color = min(vary_color, 1.0) * pg_color;" //
    "" //
    "    vary_texcoord.s = pg_texcoord.s * pg_uni_04 + pg_uni_05;" //
    "    vary_texcoord.t = pg_texcoord.t * pg_uni_06 + pg_uni_07;" //
    "}";


static const char s_frag_code[] = //
    "uniform int pg_uni_08;" // texture format
    "" //
    "uniform sampler2D pg_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    if (pg_uni_08 == 1)" //
    "    {" //
    "        gl_FragColor.rgb = texture2D(pg_tex_00, vary_texcoord.st).rgb * vary_color.rgb;" //
    "        gl_FragColor.a = vary_color.a;" //
    "    }" //
    "    else if (pg_uni_08 == 2)" //
    "    {" //
    "        gl_FragColor = texture2D(pg_tex_00, vary_texcoord.st) * vary_color;" //
    "    }" //
    "    else if (pg_uni_08 == 3)" //
    "    {" //
    "        gl_FragColor.rgb = vary_color.rgb;" //
    "        gl_FragColor.a = texture2D(pg_tex_00, vary_texcoord.st).a * vary_color.a;" //
    "    }" //
    "    else" //
    "    {" //
    "        gl_FragColor = vary_color;" //
    "    }" //
    "}";


void pgRend_Light::init(pgPrim* prim, pgID lts_id)
{
    if (!pgDrawMgr::hasShader(s_rend_light_shader_id))
    {
        pgDrawMgr::newShader(s_rend_light_shader_id, s_vert_code, s_frag_code, 31, 1, 1);
    }

    pgRend::init(prim, sizeof(RendBody), sizeof(RendData));

    setLightSetID(lts_id);
}


void pgRend_Light::init(pgPrim* prim, RendData* rend_data, pgID lts_id)
{
    if (!pgDrawMgr::hasShader(s_rend_light_shader_id))
    {
        pgDrawMgr::newShader(s_rend_light_shader_id, s_vert_code, s_frag_code, 31, 1, 1);
    }

    pgRend::init(prim, sizeof(RendBody), sizeof(RendData), rend_data);

    setLightSetID(lts_id);
}


pgID pgRend_Light::getLightSetID()
{
    RendBody* rend_body = getRendBody<RendBody>();

    return rend_body->lts->getID();
}


void pgRend_Light::setLightSetID(pgID lts_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (lts_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    rend_body->lts = pgDrawMgr::getLightSet(lts_id);
}


pgVec& pgRend_Light::dataN(u16 index)
{
    RendData* rend_data = getRendData<RendData>(index);

    return rend_data->normal;
}


void pgRend_Light::calcNormalAsTriangles(bool is_smoothing)
{
    pgPrim* prim = getPrim();

    pgUtil::calcNormalAsTriangles(getRendData<pgVec>(0), reinterpret_cast<const pgPrim::PrimData*>(&prim->dataPos(0)), prim->getMaxDataNum(), is_smoothing);
}


pgID pgRend_Light::getClassID()
{
    return pgID_("pgRend_Light");
}


void pgRend_Light::initData(void* data, u16 data_num)
{
    RendData* rend_data = static_cast<RendData*>(data);

    for (s32 i = 0; i < data_num; i++)
    {
        rend_data[i].normal = pgVec::Z_UNIT;
    }
}


void pgRend_Light::render(const pgMat& view)
{
    if (pgDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void pgRend_Light::render_soft(const pgMat& view)
{
    pgPrim* prim = getPrim();
    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        reset shader
    */
    renderSetShader(NULL);

    /*
        alloc buffer
    */
    pgTex* tex = renderGetPrimTextureN(prim);

    pgVec* pos_buf;
    pgCol* col_buf;
    r32* uv_buf;
    pgVec* normal_buf;

    renderAllocBuffer(&pos_buf, &col_buf, &uv_buf, &normal_buf, prim, false, true, (tex && renderIsTextureUVAdjustNeeded(tex)), false);

    /*
        setup color
    */
    pgCol final_col = renderGetPrimFinalColor(prim);

    if (final_col == pgCol::FULL)
    {
        for (s32 i = 0; i < cur_data_num; i++)
        {
            col_buf[i] = prim->dataCol(i);
        }
    }
    else
    {
        renderCalcColorBuffer(col_buf, prim);
    }

    pgLts* lts = getRendBody<RendBody>()->lts;
    pgMat prim_world = renderGetPrimWorld(prim);

    lts->findNearLight(prim_world.trans);

    u8 near_lit_num = lts->getNearLightNum();
    pgVec near_lit_dir[pgLts::MAX_NEAR_LIGHT_NUM];
    pgCol near_lit_col[pgLts::MAX_NEAR_LIGHT_NUM];

    for (s32 i = 0; i < near_lit_num; i++)
    {
        near_lit_dir[i] = lts->getNearLightDir(i).toLocalOf_noTrans(prim_world);
        near_lit_col[i] = lts->getNearLightColor(i);
    }

    pgCol amb_col = lts->getAmbientColor();
    RendData* rend_data = getRendData<RendData>(0);

    for (s32 i = 0; i < cur_data_num; i++)
    {
        pgCol lit_col = amb_col;

        for (s32 j = 0; j < near_lit_num; j++)
        {
            lit_col += near_lit_col[j] * -near_lit_dir[j].dot(rend_data[i].normal);
        }

        lit_col.a = 255;

        col_buf[i] *= lit_col;
    }

    renderSetColorPointer(sizeof(pgCol), reinterpret_cast<const u8*>(col_buf));

    /*
        setup texture
    */
    if (tex)
    {
        renderSetTexture(prim);

        if (uv_buf)
        {
            renderCalcUVBuffer(uv_buf, prim);
            renderSetTexCoordPointer(sizeof(r32) * 2, uv_buf);
        }
        else
        {
            renderSetTexCoordPointer(prim);
        }
    }
    else
    {
        renderSetTexture(NULL, NULL, NULL, false);
        renderSetTexCoordPointer(0, NULL);
    }

    /*
        draw primitives
    */
    renderSetVertexPointer(prim);
    renderDrawArrays(prim->getPrimMode(), 0, cur_data_num);
}


void pgRend_Light::render_shader(const pgMat& view)
{
    pgPrim* prim = getPrim();
    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    pgShd* shd = pgDrawMgr::getShader(s_rend_light_shader_id);

    if (shd->isValid())
    {
        renderSetShader(shd);
    }
    else
    {
        render_soft(view);
        return;
    }

    /*
        setup color
    */
    pgCol final_col = renderGetPrimFinalColor(prim);

    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 0), final_col.r);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 1), final_col.g);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 2), final_col.b);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 3), final_col.a);

    pgLts* lts = getRendBody<RendBody>()->lts;
    pgCol amb_col = lts->getAmbientColor();

    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 9), amb_col.r);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 10), amb_col.g);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 11), amb_col.b);

    renderSetColorPointer(0, NULL);
    renderSetAttribPointer_color(shd, prim);

    pgMat prim_world = renderGetPrimWorld(prim);

    lts->findNearLight(prim_world.trans);

    renderSetUniform_s32(renderGetShaderUniformLocation(shd, 12), lts->getNearLightNum());

    for (s32 i = 0; i < lts->getNearLightNum(); i++)
    {
        pgVec lit_dir = lts->getNearLightDir(i).toLocalOf_noTrans(prim_world);
        pgCol lit_col = lts->getNearLightColor(i);

        s32 index = i * 6 + 13;

        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 0), lit_dir.x);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 1), lit_dir.y);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 2), lit_dir.z);

        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 3), lit_col.r);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 4), lit_col.g);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, index + 5), lit_col.b);
    }

    RendData* rend_data = getRendData<RendData>(0);

    renderSetAttribPointer_r32(renderGetShaderAttribLocation(shd, 0), 3, sizeof(RendData), reinterpret_cast<const r32*>(&rend_data->normal));

    /*
        setup texture
    */
    renderSetTexture(prim);
    renderSetTexCoordPointer(0, NULL);

    pgTex* tex = renderGetPrimTextureN(prim);

    if (tex)
    {
        r32 u_param_a, u_param_b, v_param_a, v_param_b;
        renderGetTextureParam(&u_param_a, &u_param_b, &v_param_a, &v_param_b, tex);

        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 4), u_param_a);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 5), u_param_b);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 6), v_param_a);
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, 7), v_param_b);

        switch (tex->getFormat())
        {
        case pgTex::FORMAT_RGB:
        case pgTex::FORMAT_PNG_RGB:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 1);
            break;

        case pgTex::FORMAT_RGBA:
        case pgTex::FORMAT_PNG_RGBA:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 2);
            break;

        case pgTex::FORMAT_ALPHA:
        case pgTex::FORMAT_PNG_ALPHA:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 3);
            break;
        }

        renderSetAttribPointer_texCoord(shd, prim);
    }
    else
    {
        renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 0);
    }

    /*
        draw primitives
    */
    renderSetUniform_localToScreen(shd);

    renderSetVertexPointer(0, NULL);
    renderSetAttribPointer_vertex(shd, prim);

    renderDrawArrays(prim->getPrimMode(), 0, cur_data_num);

    /*
        disable attributes
    */
    this->renderDisableAttribPointers(shd);
}
