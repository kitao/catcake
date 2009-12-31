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


#include <stdarg.h>

#include "ck_draw_all.h"

#include "ck_sys_all.h"
#include "ck_task_all.h"
#include "ck_res_all.h"
#include "ck_util_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


const ckID ckDrawMgr::INVISIBLE_SCREEN_ID = ckID_("INVISIBLE_SCREEN");
const ckID ckDrawMgr::DEFAULT_3D_SCREEN_ID = ckID_("DEFAULT_3D_SCREEN");
const ckID ckDrawMgr::DEFAULT_2D_SCREEN_ID = ckID_("DEFAULT_2D_SCREEN");
const ckID ckDrawMgr::DEFAULT_LIGHT_SET_ID = ckID_("DEFAULT_LIGHT_SET");
const ckID ckDrawMgr::DEFAULT_SHADER_ID = ckID_("DEFAULT_SHADER");

ckDrawMgr* ckDrawMgr::m_instance = NULL;


static const char s_default_vert_code[] = //
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
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_Position = ck_local_to_screen * ck_vertex;" //
    "" //
    "    vary_color = ck_color * vec4(ck_uni_00, ck_uni_01, ck_uni_02, ck_uni_03) / 255.0;" //
    "" //
    "    vary_texcoord.s = ck_texcoord.s * ck_uni_04 + ck_uni_05;" //
    "    vary_texcoord.t = ck_texcoord.t * ck_uni_06 + ck_uni_07;" //
    "}";


static const char s_default_frag_code[] = //
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


bool ckDrawMgr::isShaderAvailable()
{
    return ckLowLevelAPI::isShaderAvailable();
}


u16 ckDrawMgr::getMaxTextureLength()
{
    return ckLowLevelAPI::getMaxTextureLength();
}


u16 ckDrawMgr::getValidTextureLength(u16 length)
{
    return ckLowLevelAPI::getValidTextureLength(length);
}


u16 ckDrawMgr::getTexturePixelSize(ckTex::TexFormat format)
{
    switch (format)
    {
    case ckTex::FORMAT_PNG_RGB:
        format = ckTex::FORMAT_RGB;
        break;

    case ckTex::FORMAT_PNG_RGBA:
        format = ckTex::FORMAT_RGBA;
        break;

    case ckTex::FORMAT_PNG_ALPHA:
        format = ckTex::FORMAT_ALPHA;
        break;

    default:
        break;
    }

    return ckLowLevelAPI::getTexturePixelSize(static_cast<ckLowLevelAPI::TextureFormat>(format));
}


CK_DEFINE_MANAGER_IS_CREATED(ckDrawMgr)


void ckDrawMgr::createAfterRes()
{
    destroyBeforeRes();

    m_instance = ckNew(ckDrawMgr);

    newScreen(INVISIBLE_SCREEN_ID);
    newScreen(DEFAULT_3D_SCREEN_ID);
    newScreen(DEFAULT_2D_SCREEN_ID);

    ckScr* scr0d = getScreen(INVISIBLE_SCREEN_ID);
    scr0d->setActive(false);

    ckScr* scr3d = getScreen(DEFAULT_3D_SCREEN_ID);
    scr3d->setClearColor(ckCol::ZERO);

    ckScr* scr2d = getScreen(DEFAULT_2D_SCREEN_ID);
    scr2d->setClearMode(false, true);
    scr2d->setPerspective(false);

    newLightSet(DEFAULT_LIGHT_SET_ID);

    ckLowLevelAPI::resetDrawState();

    newShader(DEFAULT_SHADER_ID, s_default_vert_code, s_default_frag_code, 9, 0, 1);
}


CK_DEFINE_MANAGER_DESTROY(ckDrawMgr, BeforeRes)


bool ckDrawMgr::isRender()
{
    return instance()->m_is_render;
}


void ckDrawMgr::setRender(bool is_render)
{
    instance()->m_is_render = is_render;
}


bool ckDrawMgr::hasScreen(ckID scr_id)
{
    ckDrawMgr* ins = instance();

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_scr_map.getN(scr_id) ? true : false;
}


ckScr* ckDrawMgr::getScreen(ckID scr_id)
{
    ckDrawMgr* ins = instance();

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckScr** scr = ins->m_scr_map.getN(scr_id);

    if (!scr)
    {
        ckThrow(ExceptionNotFound);
    }

    return *scr;
}


ckScr* ckDrawMgr::newScreen(ckID scr_id)
{
    ckDrawMgr* ins = instance();

    if (scr_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_scr_map.getN(scr_id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    return ckNew(ckScr)(scr_id);
}


void ckDrawMgr::deleteScreen(ckID scr_id)
{
    instance();

    if (scr_id == ckID::ZERO || scr_id == INVISIBLE_SCREEN_ID)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckDelete(getScreen(scr_id), ckScr);
}


ckScr* ckDrawMgr::getFirstScreenN()
{
    ckDrawMgr* ins = instance();
    const ckID* scr_id = ins->m_scr_map.getFirstKeyN();

    return scr_id ? *ins->m_scr_map.get(*scr_id) : NULL;
}


ckScr* ckDrawMgr::getLastScreenN()
{
    ckDrawMgr* ins = instance();
    const ckID* scr_id = ins->m_scr_map.getLastKeyN();

    return scr_id ? *ins->m_scr_map.get(*scr_id) : NULL;
}


bool ckDrawMgr::hasTexture(ckID tex_id)
{
    ckDrawMgr* ins = instance();

    if (tex_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_tex_map.getN(tex_id) ? true : false;
}


ckTex* ckDrawMgr::getTexture(ckID tex_id)
{
    ckDrawMgr* ins = instance();

    if (tex_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckTex** tex = ins->m_tex_map.getN(tex_id);

    if (!tex)
    {
        ckThrow(ExceptionNotFound);
    }

    return *tex;
}


ckTex* ckDrawMgr::newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format)
{
    ckDrawMgr* ins = instance();

    if (tex_id == ckID::ZERO || width == 0 || height == 0 || width > getMaxTextureLength() || height > getMaxTextureLength() || //
        (format != ckTex::FORMAT_RGB && format != ckTex::FORMAT_RGBA && format != ckTex::FORMAT_ALPHA))
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(tex_id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    return ckNew(ckTex)(tex_id, width, height, format, ckTex::MODE_READ_WRITE, NULL, 0);
}


ckTex* ckDrawMgr::newTexture(ckID tex_id, u16 width, u16 height, ckTex::TexFormat format, const void* image, u32 image_size)
{
    ckDrawMgr* ins = instance();

    if (tex_id == ckID::ZERO || width == 0 || height == 0 || width > getMaxTextureLength() || height > getMaxTextureLength() || //
        format == ckTex::FORMAT_PNG_RGB || format == ckTex::FORMAT_PNG_RGBA || format == ckTex::FORMAT_PNG_ALPHA || //
        !image || image_size != static_cast<u32>(getTexturePixelSize(format) * width * height))
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(tex_id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    return ckNew(ckTex)(tex_id, width, height, format, ckTex::MODE_READ_ONLY, image, image_size);
}


void ckDrawMgr::deleteTexture(ckID tex_id)
{
    instance();

    if (tex_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckTex* tex = getTexture(tex_id);

    ckDelete(tex, ckTex);
}


ckTex* ckDrawMgr::getFirstTextureN()
{
    ckDrawMgr* ins = instance();
    const ckID* tex_id = ins->m_tex_map.getFirstKeyN();

    return tex_id ? *ins->m_tex_map.get(*tex_id) : NULL;
}


ckTex* ckDrawMgr::getLastTextureN()
{
    ckDrawMgr* ins = instance();
    const ckID* tex_id = ins->m_tex_map.getLastKeyN();

    return tex_id ? *ins->m_tex_map.get(*tex_id) : NULL;
}


u16 ckDrawMgr::getFontIndexNum(ckID font_id)
{
    instance();

    if (font_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(font_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return *res.getExInfo<u32>();
}


ckID ckDrawMgr::getFontID()
{
    return instance()->m_font_id;
}


u16 ckDrawMgr::getFontIndex()
{
    return instance()->m_font_index;
}


void ckDrawMgr::setFont(ckID font_id, u16 font_index)
{
    ckDrawMgr* ins = instance();

    if (font_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(font_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (font_index >= *res.getExInfo<u32>())
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ins->m_font_id = font_id;
    ins->m_font_index = font_index;
    ins->m_font_info = res.getExInfo<void>();
}


u16 ckDrawMgr::getFontSize()
{
    return instance()->m_font_size;
}


u16 ckDrawMgr::setFontSize(u16 font_size)
{
    ckDrawMgr* ins = instance();

    if (font_size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_font_size = font_size;
}


u16 ckDrawMgr::calcFontDrawWidth(const char* str, ...)
{
    ckDrawMgr* ins = instance();

    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!ins->m_font_info)
    {
        ckThrow(ExceptionInvalidCall);
    }

    char buf1[256];
    CK_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    ckUtil::charToWchar(buf2, 256, buf1);

    s32 draw_width = ckLowLevelAPI::drawFreeTypeFont(NULL, 0, 0, ins->m_font_info, ins->m_font_index, ins->m_font_size, 0, 0, buf2);

    if (draw_width < 0)
    {
        ckThrow(ExceptionCalcFontDrawWidthFailed);
    }

    return draw_width;
}


u16 ckDrawMgr::calcFontDrawWidth(const wchar_t* str, ...)
{
    ckDrawMgr* ins = instance();

    if (!str)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!ins->m_font_info)
    {
        ckThrow(ExceptionInvalidCall);
    }

    wchar_t buf[256];
    CK_VSWPRINTF(buf, 256, str);

    s32 draw_width = ckLowLevelAPI::drawFreeTypeFont(NULL, 0, 0, ins->m_font_info, ins->m_font_index, ins->m_font_size, 0, 0, buf);

    if (draw_width < 0)
    {
        ckThrow(ExceptionCalcFontDrawWidthFailed);
    }

    return draw_width;
}


bool ckDrawMgr::hasShader(ckID shd_id)
{
    ckDrawMgr* ins = instance();

    if (shd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_shd_map.getN(shd_id) ? true : false;
}


ckShd* ckDrawMgr::getShader(ckID shd_id)
{
    ckDrawMgr* ins = instance();

    if (shd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckShd** shd = ins->m_shd_map.getN(shd_id);

    if (!shd)
    {
        ckThrow(ExceptionNotFound);
    }

    return *shd;
}


ckShd* ckDrawMgr::newShader(ckID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
{
    ckDrawMgr* ins = instance();

    if (shd_id == ckID::ZERO || !vert_code || !frag_code || uni_num > 100 || att_num > 100 || tex_num > 3)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_shd_map.getN(shd_id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    return ckNew(ckShd)(shd_id, vert_code, frag_code, uni_num, att_num, tex_num);
}


void ckDrawMgr::deleteShader(ckID shd_id)
{
    instance();

    if (shd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckDelete(getShader(shd_id), ckShd);
}


ckShd* ckDrawMgr::getFirstShaderN()
{
    ckDrawMgr* ins = instance();
    const ckID* shd_id = ins->m_shd_map.getFirstKeyN();

    return shd_id ? *ins->m_shd_map.get(*shd_id) : NULL;
}


ckShd* ckDrawMgr::getLastShaderN()
{
    ckDrawMgr* ins = instance();
    const ckID* shd_id = ins->m_shd_map.getLastKeyN();

    return shd_id ? *ins->m_shd_map.get(*shd_id) : NULL;
}


bool ckDrawMgr::hasLightSet(ckID lts_id)
{
    ckDrawMgr* ins = instance();

    if (lts_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_lts_map.getN(lts_id) ? true : false;
}


ckLts* ckDrawMgr::getLightSet(ckID lts_id)
{
    ckDrawMgr* ins = instance();

    if (lts_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLts** lts = ins->m_lts_map.getN(lts_id);

    if (!lts)
    {
        ckThrow(ExceptionNotFound);
    }

    return *lts;
}


ckLts* ckDrawMgr::newLightSet(ckID lts_id)
{
    ckDrawMgr* ins = instance();

    if (lts_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_lts_map.getN(lts_id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    return ckNew(ckLts)(lts_id);
}


void ckDrawMgr::deleteLightSet(ckID lts_id)
{
    instance();

    if (lts_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckDelete(getLightSet(lts_id), ckLts);
}


ckLts* ckDrawMgr::getFirstLightSetN()
{
    ckDrawMgr* ins = instance();
    const ckID* lts_id = ins->m_lts_map.getFirstKeyN();

    return lts_id ? *ins->m_lts_map.get(*lts_id) : NULL;
}


ckLts* ckDrawMgr::getLastLightSetN()
{
    ckDrawMgr* ins = instance();
    const ckID* lts_id = ins->m_lts_map.getLastKeyN();

    return lts_id ? *ins->m_lts_map.get(*lts_id) : NULL;
}


void ckDrawMgr::deleteAllVramObjForSystem()
{
    ckDrawMgr* ins = instance();

    for (const ckID* tex_id = ins->m_tex_map.getFirstKeyN(); tex_id; tex_id = ins->m_tex_map.getNextKeyN(*tex_id))
    {
        ckTex* tex = getTexture(*tex_id);

        if (tex->m_tex_obj != 0)
        {
            ckLowLevelAPI::unregisterTexture(tex->m_tex_obj);
            tex->m_tex_obj = 0;
        }

        tex->m_flag.setOn(ckTex::FLAG_UPLOAD);
    }

    for (const ckID* shd_id = ins->m_shd_map.getFirstKeyN(); shd_id; shd_id = ins->m_shd_map.getNextKeyN(*shd_id))
    {
        ckShd* shd = getShader(*shd_id);

        if (shd->m_shd_obj != 0)
        {
            ckLowLevelAPI::unregisterShader(shd->m_shd_obj);
            shd->m_shd_obj = 0;
        }

        shd->m_flag.setOn(ckShd::FLAG_UPLOAD);
    }
}


void ckDrawMgr::renderForSystem()
{
    ckDrawMgr* ins = instance();

    if (!ins->m_is_render)
    {
        return;
    }

    ckLowLevelAPI::resetDrawState(); // TODO

    for (const ckID* id = ins->m_scr_map.getFirstKeyN(); id; id = ins->m_scr_map.getNextKeyN(*id))
    {
        ckScr* scr = *ins->m_scr_map.get(*id);

        if (!scr->isActive())
        {
            continue;
        }

        ckLowLevelAPI::setViewport( //
            scr->m_left_in_framebuffer, scr->m_top_in_framebuffer, //
            scr->m_width_in_framebuffer, scr->m_height_in_framebuffer);

        ckLowLevelAPI::clearFramebuffer(scr->isClearColor(), scr->isClearDepth(), reinterpret_cast<const u8*>(&scr->m_clear_col));

        scr->setupProjection();

        ckDraw* sort_list = NULL;

        renderScreen(scr, &sort_list, scr->m_view);

        for (u32 i = 0; i < ckScr::GUEST_SCREEN_NUM; i++)
        {
            if (scr->m_guest_id[i] != ckID::ZERO)
            {
                renderScreen(*ins->m_scr_map.get(scr->m_guest_id[i]), &sort_list, scr->m_view);
            }
        }

        if (sort_list)
        {
            ckDraw* dummy;
            sortList(&sort_list, &dummy, sort_list);
        }

        for (ckDraw* draw = sort_list; draw; draw = draw->m_next_sort)
        {
            draw->setupDrawState();
            draw->render(scr->m_view);
        }

        scr->copyScreenTexture();
    }
}


ckDrawMgr::ckDrawMgr()
{
    m_is_render = true;
    m_font_id = ckID::ZERO;
    m_font_index = 0;
    m_font_size = 16;
    m_font_info = NULL;

    m_scr_map.init(SCREEN_HASH_SIZE);
    m_tex_map.init(TEXTURE_HASH_SIZE);
    m_shd_map.init(SHADER_HASH_SIZE);
    m_lts_map.init(LIGHTSET_HASH_SIZE);

    if (!ckLowLevelAPI::createFreeType())
    {
        ckThrow(ExceptionCreateFreeTypeFailed);
    }

    ckResMgr::addType("PNG", textureInitializer, textureFinalizer);
    ckResMgr::addType("TTF", fontInitializer, fontFinalizer);
    ckResMgr::addType("TTC", fontInitializer, fontFinalizer);
    ckResMgr::addType("OTF", fontInitializer, fontFinalizer);
}


ckDrawMgr::~ckDrawMgr()
{
    getScreen(INVISIBLE_SCREEN_ID)->moveLast();

    while (const ckID* scr_id = m_scr_map.getFirstKeyN())
    {
        ckDelete(getScreen(*scr_id), ckScr);
    }

    while (const ckID* tex_id = m_tex_map.getFirstKeyN())
    {
        deleteTexture(*tex_id);
    }

    while (const ckID* shd_id = m_shd_map.getFirstKeyN())
    {
        deleteShader(*shd_id);
    }

    while (const ckID* lts_id = m_lts_map.getFirstKeyN())
    {
        deleteLightSet(*lts_id);
    }

    for (const ckRes* res = ckResMgr::getFirstResourceN(); res; res = ckResMgr::getNextResourceN(res->getID()))
    {
        if (res->getExtension() == "TTF" || res->getExtension() == "TTC" || res->getExtension() == "OTF")
        {
            fontFinalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
    }

    if (!ckLowLevelAPI::destroyFreeType())
    {
        ckThrow(ExceptionDestroyFreeTypeFailed);
    }

    ckResMgr::removeType("PNG");
    ckResMgr::removeType("TTF");
    ckResMgr::removeType("TTC");
    ckResMgr::removeType("OTF");
}


CK_DEFINE_OPERATOR_EQUAL(ckDrawMgr)


CK_DEFINE_MANAGER_INSTANCE(ckDrawMgr)


void ckDrawMgr::renderScreen(ckScr* scr, ckDraw** sort_list, const ckMat& view)
{
    for (ckTree<ckDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        ckDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            draw->setupWorld();
            draw->setupFinalColor();
        }
        else
        {
            tree = tree->getLastDescendant();
        }
    }

    for (ckTree<ckDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        ckDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            if (draw->m_type != ckDraw::TYPE_NODE)
            {
                if (draw->m_draw_flag.isOn(ckDraw::FLAG_BOUND_CLIP) && //
                    scr->canBoundClip_noCalcProjection(draw->m_world, draw->m_bound_max, draw->m_bound_min))
                {
                    continue;
                }

                if (draw->isDrawFlag(ckDraw::FLAG_SORT))
                {
                    draw->setupSortValue(scr->view());

                    draw->m_next_sort = *sort_list;
                    *sort_list = draw;
                }
                else
                {
                    draw->setupDrawState();
                    draw->render(view);
                }
            }
        }
        else
        {
            tree = tree->getLastDescendant();
        }
    }
}


void ckDrawMgr::sortList(ckDraw** sorted_start, ckDraw** sorted_end, ckDraw* target_list)
{
    ckDraw* center = target_list;
    target_list = target_list->m_next_sort;

    if (!target_list)
    {
        center->m_next_sort = NULL;

        *sorted_start = center;
        *sorted_end = center;

        return;
    }

    ckDraw* left_list = NULL;
    ckDraw* right_list = NULL;
    ckDraw* next_sort;

    for (ckDraw* draw = target_list; draw; draw = next_sort)
    {
        next_sort = draw->m_next_sort;

        if (draw->m_sort_value <= center->m_sort_value)
        {
            draw->m_next_sort = left_list;
            left_list = draw;
        }
        else
        {
            draw->m_next_sort = right_list;
            right_list = draw;
        }
    }

    if (left_list)
    {
        ckDraw* start;
        ckDraw* end;

        sortList(&start, &end, left_list);

        *sorted_start = start;
        end->m_next_sort = center;
    }
    else
    {
        *sorted_start = center;
    }

    if (right_list)
    {
        ckDraw* start;
        ckDraw* end;

        sortList(&start, &end, right_list);

        *sorted_end = end;
        center->m_next_sort = start;
    }
    else
    {
        *sorted_end = center;
        center->m_next_sort = NULL;
    }
}


void ckDrawMgr::textureInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    ckDrawMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    u16 width, height;
    ckTex::TexFormat format;

    if (!ckUtil::readPNGInfo(&width, &height, &format, data, data_size))
    {
        ckThrow(ExceptionTextureInitializerFailed);
    }

    ckNew(ckTex)(id, width, height, format, ckTex::MODE_READ_ONLY, data, data_size);
}


void ckDrawMgr::textureFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    ckDrawMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckTex** tex = ins->m_tex_map.getN(id);

    if (tex && (*tex)->getMode() != ckTex::MODE_VOLATILE)
    {
        deleteTexture(id);
    }
}


void ckDrawMgr::fontInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    *exinfo = ckLowLevelAPI::newFreeTypeFont(data, data_size);

    if (!*exinfo)
    {
        ckThrow(ExceptionFontInitializerFailed);
    }
}


void ckDrawMgr::fontFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    if (!ckLowLevelAPI::deleteFreeTypeFont(exinfo))
    {
        ckThrow(ExceptionFontFinalizerFailed);
    }
}
