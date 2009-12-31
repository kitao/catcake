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


#include "ck_draw_all.h"

#include "ck_util_all.h"


static const ckID s_rend_light_shader_id = ckID_("REND_LIGHT_SHADER");


static const char s_vert_code[] = //
    "uniform mat4 ck_local_to_screen;" //
    "" //
    "attribute vec4 ck_vertex;" //
    "attribute vec4 ck_color;" //
    "attribute vec2 ck_texcoord;" //
    "" //
    "uniform float ck_uni_00, ck_uni_01, ck_uni_02, ck_uni_03;" // final color
    "" //
    "uniform float ck_uni_04, ck_uni_05;" // u param
    "uniform float ck_uni_06, ck_uni_07;" // v param
    "" //
    "uniform float ck_uni_09, ck_uni_10, ck_uni_11;" // ambient color
    "" //
    "uniform int ck_uni_12;" // light number
    "" //
    "uniform float ck_uni_13, ck_uni_14, ck_uni_15;" // light1 direction
    "uniform float ck_uni_16, ck_uni_17, ck_uni_18;" // light1 color
    "" //
    "uniform float ck_uni_19, ck_uni_20, ck_uni_21;" // light2 direction
    "uniform float ck_uni_22, ck_uni_23, ck_uni_24;" // light2 color
    "" //
    "uniform float ck_uni_25, ck_uni_26, ck_uni_27;" // light3 direction
    "uniform float ck_uni_28, ck_uni_29, ck_uni_30;" // light3 color
    "" //
    "attribute vec3 ck_att_00;" // normal
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_Position = ck_local_to_screen * ck_vertex;" //
    "" //
    "    vary_color = vec4(ck_uni_09, ck_uni_10, ck_uni_11, 255.0) / 255.0;" //
    "" //
    "    if (ck_uni_12 >= 1)" //
    "    {" //
    "        float lumi = max(-dot(vec3(ck_uni_13, ck_uni_14, ck_uni_15), ck_att_00), 0.0);" //
    "        vary_color += vec4(ck_uni_16, ck_uni_17, ck_uni_18, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    if (ck_uni_12 >= 2)" //
    "    {" //
    "        float lumi = max(-dot(vec3(ck_uni_19, ck_uni_20, ck_uni_21), ck_att_00), 0.0);" //
    "        vary_color += vec4(ck_uni_22, ck_uni_23, ck_uni_24, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    if (ck_uni_12 >= 3)" //
    "    {" //
    "        float lumi = max(-dot(vec3(ck_uni_25, ck_uni_26, ck_uni_27), ck_att_00), 0.0);" //
    "        vary_color += vec4(ck_uni_28, ck_uni_29, ck_uni_30, 255.0) * (lumi / 255.0);" //
    "    }" //
    "" //
    "    vary_color = min(vary_color, 1.0) * ck_color;" //
    "" //
    "    vary_texcoord.s = ck_texcoord.s * ck_uni_04 + ck_uni_05;" //
    "    vary_texcoord.t = ck_texcoord.t * ck_uni_06 + ck_uni_07;" //
    "}";


static const char s_frag_code[] = //
    "uniform int ck_uni_08;" // texture format
    "" //
    "uniform sampler2D ck_tex_00;" //
    "" //
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    if (ck_uni_08 == 1)" //
    "    {" //
    "        gl_FragColor.rgb = texture2D(ck_tex_00, vary_texcoord.st).rgb * vary_color.rgb;" //
    "        gl_FragColor.a = vary_color.a;" //
    "    }" //
    "    else if (ck_uni_08 == 2)" //
    "    {" //
    "        gl_FragColor = texture2D(ck_tex_00, vary_texcoord.st) * vary_color;" //
    "    }" //
    "    else if (ck_uni_08 == 3)" //
    "    {" //
    "        gl_FragColor.rgb = vary_color.rgb;" //
    "        gl_FragColor.a = texture2D(ck_tex_00, vary_texcoord.st).a * vary_color.a;" //
    "    }" //
    "    else" //
    "    {" //
    "        gl_FragColor = vary_color;" //
    "    }" //
    "}";


void ckRend_Light::init(ckPrim* prim, ckID lts_id)
{
    if (!ckDrawMgr::hasShader(s_rend_light_shader_id))
    {
        ckDrawMgr::newShader(s_rend_light_shader_id, s_vert_code, s_frag_code, 31, 1, 1);
    }

    ckRend::init(prim, sizeof(RendBody), sizeof(RendData));

    setLightSetID(lts_id);
}


void ckRend_Light::init(ckPrim* prim, RendData* rend_data, ckID lts_id)
{
    if (!ckDrawMgr::hasShader(s_rend_light_shader_id))
    {
        ckDrawMgr::newShader(s_rend_light_shader_id, s_vert_code, s_frag_code, 31, 1, 1);
    }

    ckRend::init(prim, sizeof(RendBody), sizeof(RendData), rend_data);

    setLightSetID(lts_id);
}


ckID ckRend_Light::getLightSetID()
{
    RendBody* rend_body = getRendBody<RendBody>();

    return rend_body->lts->getID();
}


void ckRend_Light::setLightSetID(ckID lts_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (lts_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    rend_body->lts = ckDrawMgr::getLightSet(lts_id);
}


ckVec& ckRend_Light::dataN(u16 index)
{
    RendData* rend_data = getRendData<RendData>(index);

    return rend_data->normal;
}


void ckRend_Light::calcNormalAsTriangles(bool is_smoothing)
{
    ckPrim* prim = getPrim();

    ckUtil::calcNormalAsTriangles(getRendData<ckVec>(0), reinterpret_cast<const ckPrim::PrimData*>(&prim->dataPos(0)), prim->getMaxDataNum(), is_smoothing);
}


ckID ckRend_Light::getClassID()
{
    return ckID_("ckRend_Light");
}


void ckRend_Light::initData(void* data, u16 data_num)
{
    RendData* rend_data = static_cast<RendData*>(data);

    for (s32 i = 0; i < data_num; i++)
    {
        rend_data[i].normal = ckVec::Z_UNIT;
    }
}


void ckRend_Light::render(const ckMat& view)
{
    if (ckDrawMgr::isShaderAvailable())
    {
        render_shader(view);
    }
    else
    {
        render_soft(view);
    }
}


void ckRend_Light::render_soft(const ckMat& view)
{
    ckPrim* prim = getPrim();
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
    ckTex* tex = renderGetPrimTextureN(prim);

    ckVec* pos_buf;
    ckCol* col_buf;
    r32* uv_buf;
    ckVec* normal_buf;

    renderAllocBuffer(&pos_buf, &col_buf, &uv_buf, &normal_buf, prim, false, true, (tex && renderIsTextureUVAdjustNeeded(tex)), false);

    /*
        setup color
    */
    ckCol final_col = renderGetPrimFinalColor(prim);

    if (final_col == ckCol::FULL)
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

    ckLts* lts = getRendBody<RendBody>()->lts;
    ckMat prim_world = renderGetPrimWorld(prim);

    lts->findNearLight(prim_world.trans);

    u8 near_lit_num = lts->getNearLightNum();
    ckVec near_lit_dir[ckLts::MAX_NEAR_LIGHT_NUM];
    ckCol near_lit_col[ckLts::MAX_NEAR_LIGHT_NUM];

    for (s32 i = 0; i < near_lit_num; i++)
    {
        near_lit_dir[i] = lts->getNearLightDir(i).toLocalOf_noTrans(prim_world);
        near_lit_col[i] = lts->getNearLightColor(i);
    }

    ckCol amb_col = lts->getAmbientColor();
    RendData* rend_data = getRendData<RendData>(0);

    for (s32 i = 0; i < cur_data_num; i++)
    {
        ckCol lit_col = amb_col;

        for (s32 j = 0; j < near_lit_num; j++)
        {
            lit_col += near_lit_col[j] * -near_lit_dir[j].dot(rend_data[i].normal);
        }

        lit_col.a = 255;

        col_buf[i] *= lit_col;
    }

    renderSetColorPointer(sizeof(ckCol), reinterpret_cast<const u8*>(col_buf));

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


void ckRend_Light::render_shader(const ckMat& view)
{
    ckPrim* prim = getPrim();
    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    ckShd* shd = ckDrawMgr::getShader(s_rend_light_shader_id);

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
    ckCol final_col = renderGetPrimFinalColor(prim);

    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 0), final_col.r);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 1), final_col.g);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 2), final_col.b);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 3), final_col.a);

    ckLts* lts = getRendBody<RendBody>()->lts;
    ckCol amb_col = lts->getAmbientColor();

    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 9), amb_col.r);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 10), amb_col.g);
    renderSetUniform_r32(renderGetShaderUniformLocation(shd, 11), amb_col.b);

    renderSetColorPointer(0, NULL);
    renderSetAttribPointer_color(shd, prim);

    ckMat prim_world = renderGetPrimWorld(prim);

    lts->findNearLight(prim_world.trans);

    renderSetUniform_s32(renderGetShaderUniformLocation(shd, 12), lts->getNearLightNum());

    for (s32 i = 0; i < lts->getNearLightNum(); i++)
    {
        ckVec lit_dir = lts->getNearLightDir(i).toLocalOf_noTrans(prim_world);
        ckCol lit_col = lts->getNearLightColor(i);

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

    ckTex* tex = renderGetPrimTextureN(prim);

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
        case ckTex::FORMAT_RGB:
        case ckTex::FORMAT_PNG_RGB:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 1);
            break;

        case ckTex::FORMAT_RGBA:
        case ckTex::FORMAT_PNG_RGBA:
            renderSetUniform_s32(renderGetShaderUniformLocation(shd, 8), 2);
            break;

        case ckTex::FORMAT_ALPHA:
        case ckTex::FORMAT_PNG_ALPHA:
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
