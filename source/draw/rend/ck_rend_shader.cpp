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


void ckRend_Shader::init(ckPrim* prim, ckID shd_id)
{
    if (shd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckShd* shd = ckDrawMgr::getShader(shd_id);

    u32 rend_body_size = sizeof(RendBody) + shd->getUniformNum() * sizeof(r32);
    u32 rend_data_size = shd->getAttribNum() * sizeof(r32);

    ckRend::init(prim, rend_body_size, rend_data_size);

    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->shd = shd;
    rend_body->tex2 = NULL;
    rend_body->tex3 = NULL;
    rend_body->uni = reinterpret_cast<r32*>(rend_body + 1);
    rend_body->att = reinterpret_cast<r32*>(getRendData<u8>(0));
    rend_body->rend_data_size = rend_data_size;
}


void ckRend_Shader::init(ckPrim* prim, r32* rend_data, ckID shd_id)
{
    if (shd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckShd* shd = ckDrawMgr::getShader(shd_id);

    u32 rend_body_size = sizeof(RendBody) + shd->getUniformNum() * sizeof(r32);
    u32 rend_data_size = shd->getAttribNum() * sizeof(r32);

    ckRend::init(prim, rend_body_size, rend_data_size, rend_data);

    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->shd = shd;
    rend_body->tex2 = NULL;
    rend_body->tex3 = NULL;
    rend_body->uni = reinterpret_cast<r32*>(rend_body + 1);
    rend_body->att = reinterpret_cast<r32*>(getRendData<u8>(0));
    rend_body->rend_data_size = rend_data_size;
}


ckID ckRend_Shader::getShaderID()
{
    RendBody* rend_body = getRendBody<RendBody>();

    return rend_body->shd->getID();
}


ckID ckRend_Shader::get2ndTextureID()
{
    RendBody* rend_body = getRendBody<RendBody>();
    ckTex* tex2 = rend_body->tex2;

    return tex2 ? tex2->getID() : ckID::ZERO;
}


void ckRend_Shader::set2ndTextureID(ckID tex_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->tex2 = (tex_id != ckID::ZERO) ? ckDrawMgr::getTexture(tex_id) : NULL;
}


ckID ckRend_Shader::get3rdTextureID()
{
    RendBody* rend_body = getRendBody<RendBody>();
    ckTex* tex3 = rend_body->tex3;

    return tex3 ? tex3->getID() : ckID::ZERO;
}


r32& ckRend_Shader::uniform(u8 uni_index)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index >= rend_body->shd->getUniformNum())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return rend_body->uni[uni_index];
}


void ckRend_Shader::setUniformVec(u8 uni_index, const ckVec& vec)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 2 >= rend_body->shd->getUniformNum())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = vec.x;
    *(uni + 1) = vec.y;
    *(uni + 2) = vec.z;
}


void ckRend_Shader::setUniformCol3(u8 uni_index, ckCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 2 >= rend_body->shd->getUniformNum())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = col.r;
    *(uni + 1) = col.g;
    *(uni + 2) = col.b;
}


void ckRend_Shader::setUniformCol4(u8 uni_index, ckCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();

    if (uni_index + 3 >= rend_body->shd->getUniformNum())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    r32* uni = &rend_body->uni[uni_index];

    *(uni + 0) = col.r;
    *(uni + 1) = col.g;
    *(uni + 2) = col.b;
    *(uni + 3) = col.a;
}


r32& ckRend_Shader::attrib(u16 data_index, u8 att_index)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index >= att_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return rend_body->att[att_num * data_index + att_index];
}


void ckRend_Shader::setAttribVec(u16 data_index, u8 att_index, const ckVec& vec)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 2 >= att_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = vec.x;
    *(att + 1) = vec.y;
    *(att + 2) = vec.z;
}


void ckRend_Shader::setAttribCol3(u16 data_index, u8 att_index, ckCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 2 >= att_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = col.r;
    *(att + 1) = col.g;
    *(att + 2) = col.b;
}


void ckRend_Shader::setAttribCol4(u16 data_index, u8 att_index, ckCol col)
{
    RendBody* rend_body = getRendBody<RendBody>();
    u8 att_num = rend_body->shd->getAttribNum();

    if (data_index >= getPrim()->getMaxDataNum() || att_index + 3 >= att_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    r32* att = &rend_body->att[att_num * data_index + att_index];

    *(att + 0) = col.r;
    *(att + 1) = col.g;
    *(att + 2) = col.b;
    *(att + 3) = col.a;
}


void ckRend_Shader::set3rdTextureID(ckID tex_id)
{
    RendBody* rend_body = getRendBody<RendBody>();

    rend_body->tex3 = (tex_id != ckID::ZERO) ? ckDrawMgr::getTexture(tex_id) : NULL;
}


ckID ckRend_Shader::getClassID()
{
    return ckID_("ckRend_Shader");
}


void ckRend_Shader::initData(void* data, u16 data_num) {}


void ckRend_Shader::render(const ckMat& view)
{
    ckPrim* prim = getPrim();

    if (!ckDrawMgr::isShaderAvailable())
    {
        renderCallPrimRenderWithDestroyingBuffer(prim, view);
        return;
    }

    u16 cur_data_num = prim->getCurDataNum();

    if (cur_data_num == 0)
    {
        return;
    }

    /*
        setup shader
    */
    RendBody* rend_body = getRendBody<RendBody>();
    ckShd* shd = rend_body->shd;

    if (shd->isValid())
    {
        renderSetShader(shd);
    }
    else
    {
        renderCallPrimRenderWithDestroyingBuffer(prim, view);
        return;
    }

    /*
        alloc buffer
    */
    ckTex* tex = renderGetPrimTextureN(prim);
    ckCol final_col = renderGetPrimFinalColor(prim);

    ckVec* pos_buf;
    ckCol* col_buf;
    r32* uv_buf;
    ckVec* normal_buf;

    renderAllocBuffer(&pos_buf, &col_buf, &uv_buf, &normal_buf, prim, false, (final_col != ckCol::FULL), (tex && renderIsTextureUVAdjustNeeded(tex)), false);

    /*
        setup color
    */
    renderSetColorPointer(0, NULL);

    if (col_buf)
    {
        renderCalcColorBuffer(col_buf, prim);
        renderSetAttribPointer_color(shd, sizeof(ckCol), reinterpret_cast<const u8*>(col_buf));
    }
    else
    {
        renderSetAttribPointer_color(shd, prim);
    }

    /*
        setup texture
    */
    renderSetTexture(tex, rend_body->tex2, rend_body->tex3, prim->isDrawFlag(ckDraw::FLAG_BILINEAR));
    renderSetTexCoordPointer(0, NULL);

    if (tex)
    {
        if (uv_buf)
        {
            renderCalcUVBuffer(uv_buf, prim);
            renderSetAttribPointer_texCoord(shd, sizeof(r32) * 2, uv_buf);
        }
        else
        {
            renderSetAttribPointer_texCoord(shd, prim);
        }
    }

    /*
        setup param
    */
    for (s32 i = 0; i < shd->getUniformNum(); i++)
    {
        renderSetUniform_r32(renderGetShaderUniformLocation(shd, i), rend_body->uni[i]);
    }

    for (s32 i = 0; i < shd->getAttribNum(); i++)
    {
        u32 att_loc = renderGetShaderAttribLocation(shd, i);

        renderSetAttribPointer_r32(att_loc, 1, rend_body->rend_data_size, &rend_body->att[i]);
    }

    if (shd->getTextureNum() > 0)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 0), 0);
    }

    if (shd->getTextureNum() > 1)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 1), 1);
    }

    if (shd->getTextureNum() > 2)
    {
        renderSetUniform_s32(renderGetShaderTextureLocation(shd, 2), 2);
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
    renderDisableAttribPointers(shd);
}
