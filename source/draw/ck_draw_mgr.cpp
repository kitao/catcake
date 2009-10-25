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


#include <stdarg.h>

#include "pg_draw_all.h"

#include "pg_sys_all.h"
#include "pg_task_all.h"
#include "pg_res_all.h"
#include "pg_util_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


const pgID pgDrawMgr::INVISIBLE_SCREEN_ID = pgID_("INVISIBLE_SCREEN");
const pgID pgDrawMgr::DEFAULT_3D_SCREEN_ID = pgID_("DEFAULT_3D_SCREEN");
const pgID pgDrawMgr::DEFAULT_2D_SCREEN_ID = pgID_("DEFAULT_2D_SCREEN");
const pgID pgDrawMgr::DEFAULT_LIGHT_SET_ID = pgID_("DEFAULT_LIGHT_SET");
const pgID pgDrawMgr::DEFAULT_SHADER_ID = pgID_("DEFAULT_SHADER");

pgDrawMgr* pgDrawMgr::m_instance = NULL;


static const char s_default_vert_code[] = //
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
    "varying vec4 vary_color;" //
    "varying vec2 vary_texcoord;" //
    "" //
    "" //
    "void main()" //
    "{" //
    "    gl_Position = pg_local_to_screen * pg_vertex;" //
    "" //
    "    vary_color = pg_color * vec4(pg_uni_00, pg_uni_01, pg_uni_02, pg_uni_03) / 255.0;" //
    "" //
    "    vary_texcoord.s = pg_texcoord.s * pg_uni_04 + pg_uni_05;" //
    "    vary_texcoord.t = pg_texcoord.t * pg_uni_06 + pg_uni_07;" //
    "}";


static const char s_default_frag_code[] = //
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


bool pgDrawMgr::isShaderAvailable()
{
    return pgLowLevelAPI::isShaderAvailable();
}


u16 pgDrawMgr::getMaxTextureLength()
{
    return pgLowLevelAPI::getMaxTextureLength();
}


u16 pgDrawMgr::getValidTextureLength(u16 length)
{
    return pgLowLevelAPI::getValidTextureLength(length);
}


u16 pgDrawMgr::getTexturePixelSize(pgTex::TexFormat format)
{
    switch (format)
    {
    case pgTex::FORMAT_PNG_RGB:
        format = pgTex::FORMAT_RGB;
        break;

    case pgTex::FORMAT_PNG_RGBA:
        format = pgTex::FORMAT_RGBA;
        break;

    case pgTex::FORMAT_PNG_ALPHA:
        format = pgTex::FORMAT_ALPHA;
        break;

    default:
        break;
    }

    return pgLowLevelAPI::getTexturePixelSize(static_cast<pgLowLevelAPI::TextureFormat>(format));
}


PG_DEFINE_MANAGER_IS_CREATED(pgDrawMgr)


void pgDrawMgr::createAfterRes()
{
    destroyBeforeRes();

    m_instance = pgNew(pgDrawMgr);

    newScreen(INVISIBLE_SCREEN_ID);
    newScreen(DEFAULT_3D_SCREEN_ID);
    newScreen(DEFAULT_2D_SCREEN_ID);

    pgScr* scr0d = getScreen(INVISIBLE_SCREEN_ID);
    scr0d->setActive(false);

    pgScr* scr3d = getScreen(DEFAULT_3D_SCREEN_ID);
    scr3d->setClearColor(pgCol::ZERO);

    pgScr* scr2d = getScreen(DEFAULT_2D_SCREEN_ID);
    scr2d->setClearMode(false, true);
    scr2d->setPerspective(false);

    newLightSet(DEFAULT_LIGHT_SET_ID);

    pgLowLevelAPI::resetDrawState();

    newShader(DEFAULT_SHADER_ID, s_default_vert_code, s_default_frag_code, 9, 0, 1);
}


PG_DEFINE_MANAGER_DESTROY(pgDrawMgr, BeforeRes)


bool pgDrawMgr::isRender()
{
    return instance()->m_is_render;
}


void pgDrawMgr::setRender(bool is_render)
{
    instance()->m_is_render = is_render;
}


bool pgDrawMgr::hasScreen(pgID scr_id)
{
    pgDrawMgr* ins = instance();

    if (scr_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_scr_map.getN(scr_id) ? true : false;
}


pgScr* pgDrawMgr::getScreen(pgID scr_id)
{
    pgDrawMgr* ins = instance();

    if (scr_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgScr** scr = ins->m_scr_map.getN(scr_id);

    if (!scr)
    {
        pgThrow(ExceptionNotFound);
    }

    return *scr;
}


pgScr* pgDrawMgr::newScreen(pgID scr_id)
{
    pgDrawMgr* ins = instance();

    if (scr_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_scr_map.getN(scr_id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    return pgNew(pgScr)(scr_id);
}


void pgDrawMgr::deleteScreen(pgID scr_id)
{
    instance();

    if (scr_id == pgID::ZERO || scr_id == INVISIBLE_SCREEN_ID)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgDelete(getScreen(scr_id), pgScr);
}


pgScr* pgDrawMgr::getFirstScreenN()
{
    pgDrawMgr* ins = instance();
    const pgID* scr_id = ins->m_scr_map.getFirstKeyN();

    return scr_id ? *ins->m_scr_map.get(*scr_id) : NULL;
}


pgScr* pgDrawMgr::getLastScreenN()
{
    pgDrawMgr* ins = instance();
    const pgID* scr_id = ins->m_scr_map.getLastKeyN();

    return scr_id ? *ins->m_scr_map.get(*scr_id) : NULL;
}


bool pgDrawMgr::hasTexture(pgID tex_id)
{
    pgDrawMgr* ins = instance();

    if (tex_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_tex_map.getN(tex_id) ? true : false;
}


pgTex* pgDrawMgr::getTexture(pgID tex_id)
{
    pgDrawMgr* ins = instance();

    if (tex_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgTex** tex = ins->m_tex_map.getN(tex_id);

    if (!tex)
    {
        pgThrow(ExceptionNotFound);
    }

    return *tex;
}


pgTex* pgDrawMgr::newTexture(pgID tex_id, u16 width, u16 height, pgTex::TexFormat format)
{
    pgDrawMgr* ins = instance();

    if (tex_id == pgID::ZERO || width == 0 || height == 0 || width > getMaxTextureLength() || height > getMaxTextureLength() || //
        (format != pgTex::FORMAT_RGB && format != pgTex::FORMAT_RGBA && format != pgTex::FORMAT_ALPHA))
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(tex_id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    return pgNew(pgTex)(tex_id, width, height, format, pgTex::MODE_READ_WRITE, NULL, 0);
}


pgTex* pgDrawMgr::newTexture(pgID tex_id, u16 width, u16 height, pgTex::TexFormat format, const void* image, u32 image_size)
{
    pgDrawMgr* ins = instance();

    if (tex_id == pgID::ZERO || width == 0 || height == 0 || width > getMaxTextureLength() || height > getMaxTextureLength() || //
        format == pgTex::FORMAT_PNG_RGB || format == pgTex::FORMAT_PNG_RGBA || format == pgTex::FORMAT_PNG_ALPHA || //
        !image || image_size != static_cast<u32>(getTexturePixelSize(format) * width * height))
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(tex_id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    return pgNew(pgTex)(tex_id, width, height, format, pgTex::MODE_READ_ONLY, image, image_size);
}


void pgDrawMgr::deleteTexture(pgID tex_id)
{
    instance();

    if (tex_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgTex* tex = getTexture(tex_id);

    pgDelete(tex, pgTex);
}


pgTex* pgDrawMgr::getFirstTextureN()
{
    pgDrawMgr* ins = instance();
    const pgID* tex_id = ins->m_tex_map.getFirstKeyN();

    return tex_id ? *ins->m_tex_map.get(*tex_id) : NULL;
}


pgTex* pgDrawMgr::getLastTextureN()
{
    pgDrawMgr* ins = instance();
    const pgID* tex_id = ins->m_tex_map.getLastKeyN();

    return tex_id ? *ins->m_tex_map.get(*tex_id) : NULL;
}


u16 pgDrawMgr::getFontIndexNum(pgID font_id)
{
    instance();

    if (font_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(font_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return *res.getExInfo<u32>();
}


pgID pgDrawMgr::getFontID()
{
    return instance()->m_font_id;
}


u16 pgDrawMgr::getFontIndex()
{
    return instance()->m_font_index;
}


void pgDrawMgr::setFont(pgID font_id, u16 font_index)
{
    pgDrawMgr* ins = instance();

    if (font_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes res;

    pgTry
    {
        res = pgResMgr::getResource(font_id);
    }
    pgCatch(pgResMgr::ExceptionNotFound)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (font_index >= *res.getExInfo<u32>())
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ins->m_font_id = font_id;
    ins->m_font_index = font_index;
    ins->m_font_info = res.getExInfo<void>();
}


u16 pgDrawMgr::getFontSize()
{
    return instance()->m_font_size;
}


u16 pgDrawMgr::setFontSize(u16 font_size)
{
    pgDrawMgr* ins = instance();

    if (font_size == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_font_size = font_size;
}


u16 pgDrawMgr::calcFontDrawWidth(const char* str, ...)
{
    instance();

    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    char buf1[256];
    PG_VSPRINTF(buf1, 256, str);

    wchar_t buf2[256];
    pgUtil::charToWchar(buf2, 256, buf1);

    return calcFontDrawWidth(buf2);
}


u16 pgDrawMgr::calcFontDrawWidth(const wchar_t* str, ...)
{
    pgDrawMgr* ins = instance();

    if (!str)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (!ins->m_font_info)
    {
        pgThrow(ExceptionInvalidCall);
    }

    wchar_t buf[256];
    PG_VSWPRINTF(buf, 256, str);

    s32 draw_width = pgLowLevelAPI::drawFreeTypeFont(NULL, 0, 0, ins->m_font_info, ins->m_font_index, ins->m_font_size, 0, 0, buf);

    if (draw_width < 0)
    {
        pgThrow(ExceptionCalcFontDrawWidthFailed);
    }

    return draw_width;
}


bool pgDrawMgr::hasShader(pgID shd_id)
{
    pgDrawMgr* ins = instance();

    if (shd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_shd_map.getN(shd_id) ? true : false;
}


pgShd* pgDrawMgr::getShader(pgID shd_id)
{
    pgDrawMgr* ins = instance();

    if (shd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgShd** shd = ins->m_shd_map.getN(shd_id);

    if (!shd)
    {
        pgThrow(ExceptionNotFound);
    }

    return *shd;
}


pgShd* pgDrawMgr::newShader(pgID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num)
{
    pgDrawMgr* ins = instance();

    if (shd_id == pgID::ZERO || !vert_code || !frag_code || uni_num > 100 || att_num > 100 || tex_num > 3)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_shd_map.getN(shd_id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    return pgNew(pgShd)(shd_id, vert_code, frag_code, uni_num, att_num, tex_num);
}


void pgDrawMgr::deleteShader(pgID shd_id)
{
    instance();

    if (shd_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgDelete(getShader(shd_id), pgShd);
}


pgShd* pgDrawMgr::getFirstShaderN()
{
    pgDrawMgr* ins = instance();
    const pgID* shd_id = ins->m_shd_map.getFirstKeyN();

    return shd_id ? *ins->m_shd_map.get(*shd_id) : NULL;
}


pgShd* pgDrawMgr::getLastShaderN()
{
    pgDrawMgr* ins = instance();
    const pgID* shd_id = ins->m_shd_map.getLastKeyN();

    return shd_id ? *ins->m_shd_map.get(*shd_id) : NULL;
}


bool pgDrawMgr::hasLightSet(pgID lts_id)
{
    pgDrawMgr* ins = instance();

    if (lts_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_lts_map.getN(lts_id) ? true : false;
}


pgLts* pgDrawMgr::getLightSet(pgID lts_id)
{
    pgDrawMgr* ins = instance();

    if (lts_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgLts** lts = ins->m_lts_map.getN(lts_id);

    if (!lts)
    {
        pgThrow(ExceptionNotFound);
    }

    return *lts;
}


pgLts* pgDrawMgr::newLightSet(pgID lts_id)
{
    pgDrawMgr* ins = instance();

    if (lts_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_lts_map.getN(lts_id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    return pgNew(pgLts)(lts_id);
}


void pgDrawMgr::deleteLightSet(pgID lts_id)
{
    instance();

    if (lts_id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgDelete(getLightSet(lts_id), pgLts);
}


pgLts* pgDrawMgr::getFirstLightSetN()
{
    pgDrawMgr* ins = instance();
    const pgID* lts_id = ins->m_lts_map.getFirstKeyN();

    return lts_id ? *ins->m_lts_map.get(*lts_id) : NULL;
}


pgLts* pgDrawMgr::getLastLightSetN()
{
    pgDrawMgr* ins = instance();
    const pgID* lts_id = ins->m_lts_map.getLastKeyN();

    return lts_id ? *ins->m_lts_map.get(*lts_id) : NULL;
}


void pgDrawMgr::deleteAllVramObjForSystem()
{
    pgDrawMgr* ins = instance();

    for (const pgID* tex_id = ins->m_tex_map.getFirstKeyN(); tex_id; tex_id = ins->m_tex_map.getNextKeyN(*tex_id))
    {
        pgTex* tex = getTexture(*tex_id);

        if (tex->m_tex_obj != 0)
        {
            pgLowLevelAPI::unregisterTexture(tex->m_tex_obj);
            tex->m_tex_obj = 0;
        }

        tex->m_flag.setOn(pgTex::FLAG_UPLOAD);
    }

    for (const pgID* shd_id = ins->m_shd_map.getFirstKeyN(); shd_id; shd_id = ins->m_shd_map.getNextKeyN(*shd_id))
    {
        pgShd* shd = getShader(*shd_id);

        if (shd->m_shd_obj != 0)
        {
            pgLowLevelAPI::unregisterShader(shd->m_shd_obj);
            shd->m_shd_obj = 0;
        }

        shd->m_flag.setOn(pgShd::FLAG_UPLOAD);
    }
}


void pgDrawMgr::renderForSystem()
{
    pgDrawMgr* ins = instance();

    if (!ins->m_is_render)
    {
        return;
    }

    pgLowLevelAPI::resetDrawState(); // TODO

    for (const pgID* id = ins->m_scr_map.getFirstKeyN(); id; id = ins->m_scr_map.getNextKeyN(*id))
    {
        pgScr* scr = *ins->m_scr_map.get(*id);

        if (!scr->isActive())
        {
            continue;
        }

        pgLowLevelAPI::setViewport( //
            scr->m_left_in_framebuffer, scr->m_top_in_framebuffer, //
            scr->m_width_in_framebuffer, scr->m_height_in_framebuffer);

        pgLowLevelAPI::clearFramebuffer(scr->isClearColor(), scr->isClearDepth(), reinterpret_cast<const u8*>(&scr->m_clear_col));

        scr->setupProjection();

        pgDraw* sort_list = NULL;

        renderScreen(scr, &sort_list, scr->m_view);

        for (u32 i = 0; i < pgScr::GUEST_SCREEN_NUM; i++)
        {
            if (scr->m_guest_id[i] != pgID::ZERO)
            {
                renderScreen(*ins->m_scr_map.get(scr->m_guest_id[i]), &sort_list, scr->m_view);
            }
        }

        if (sort_list)
        {
            pgDraw* dummy;
            sortList(&sort_list, &dummy, sort_list);
        }

        for (pgDraw* draw = sort_list; draw; draw = draw->m_next_sort)
        {
            draw->setupDrawState();
            draw->render(scr->m_view);
        }

        scr->copyScreenTexture();
    }
}


pgDrawMgr::pgDrawMgr()
{
    m_is_render = true;
    m_font_id = pgID::ZERO;
    m_font_index = 0;
    m_font_size = 16;
    m_font_info = NULL;

    m_scr_map.init(SCREEN_HASH_SIZE);
    m_tex_map.init(TEXTURE_HASH_SIZE);
    m_shd_map.init(SHADER_HASH_SIZE);
    m_lts_map.init(LIGHTSET_HASH_SIZE);

    if (!pgLowLevelAPI::createFreeType())
    {
        pgThrow(ExceptionCreateFreeTypeFailed);
    }

    pgResMgr::addType("PNG", textureInitializer, textureFinalizer);
    pgResMgr::addType("TTF", fontInitializer, fontFinalizer);
    pgResMgr::addType("TTC", fontInitializer, fontFinalizer);
    pgResMgr::addType("OTF", fontInitializer, fontFinalizer);
}


pgDrawMgr::~pgDrawMgr()
{
    getScreen(INVISIBLE_SCREEN_ID)->moveLast();

    while (const pgID* scr_id = m_scr_map.getFirstKeyN())
    {
        pgDelete(getScreen(*scr_id), pgScr);
    }

    while (const pgID* tex_id = m_tex_map.getFirstKeyN())
    {
        deleteTexture(*tex_id);
    }

    while (const pgID* shd_id = m_shd_map.getFirstKeyN())
    {
        deleteShader(*shd_id);
    }

    while (const pgID* lts_id = m_lts_map.getFirstKeyN())
    {
        deleteLightSet(*lts_id);
    }

    for (const pgRes* res = pgResMgr::getFirstResourceN(); res; res = pgResMgr::getNextResourceN(res->getID()))
    {
        if (res->getExtension() == "TTF" || res->getExtension() == "TTC" || res->getExtension() == "OTF")
        {
            fontFinalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
    }

    if (!pgLowLevelAPI::destroyFreeType())
    {
        pgThrow(ExceptionDestroyFreeTypeFailed);
    }

    pgResMgr::removeType("PNG");
    pgResMgr::removeType("TTF");
    pgResMgr::removeType("TTC");
    pgResMgr::removeType("OTF");
}


PG_DEFINE_OPERATOR_EQUAL(pgDrawMgr)


PG_DEFINE_MANAGER_INSTANCE(pgDrawMgr)


void pgDrawMgr::renderScreen(pgScr* scr, pgDraw** sort_list, const pgMat& view)
{
    for (pgTree<pgDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        pgDraw* draw = tree->getSelf();

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

    for (pgTree<pgDraw>* tree = scr->m_root_draw.m_tree.getFirstChildN(); tree; tree = tree->getNextAllN())
    {
        pgDraw* draw = tree->getSelf();

        if (draw->isVisible())
        {
            if (draw->m_type != pgDraw::TYPE_NODE)
            {
                if (draw->m_draw_flag.isOn(pgDraw::FLAG_BOUND_CLIP) && //
                    scr->canBoundClip_noCalcProjection(draw->m_world, draw->m_bound_max, draw->m_bound_min))
                {
                    continue;
                }

                if (draw->isDrawFlag(pgDraw::FLAG_SORT))
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


void pgDrawMgr::sortList(pgDraw** sorted_start, pgDraw** sorted_end, pgDraw* target_list)
{
    pgDraw* center = target_list;
    target_list = target_list->m_next_sort;

    if (!target_list)
    {
        center->m_next_sort = NULL;

        *sorted_start = center;
        *sorted_end = center;

        return;
    }

    pgDraw* left_list = NULL;
    pgDraw* right_list = NULL;
    pgDraw* next_sort;

    for (pgDraw* draw = target_list; draw; draw = next_sort)
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
        pgDraw* start;
        pgDraw* end;

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
        pgDraw* start;
        pgDraw* end;

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


void pgDrawMgr::textureInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    pgDrawMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_tex_map.getN(id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    u16 width, height;
    pgTex::TexFormat format;

    if (!pgUtil::readPNGInfo(&width, &height, &format, data, data_size))
    {
        pgThrow(ExceptionTextureInitializerFailed);
    }

    pgNew(pgTex)(id, width, height, format, pgTex::MODE_READ_ONLY, data, data_size);
}


void pgDrawMgr::textureFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    pgDrawMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgTex** tex = ins->m_tex_map.getN(id);

    if (tex && (*tex)->getMode() != pgTex::MODE_VOLATILE)
    {
        deleteTexture(id);
    }
}


void pgDrawMgr::fontInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    *exinfo = pgLowLevelAPI::newFreeTypeFont(data, data_size);

    if (!*exinfo)
    {
        pgThrow(ExceptionFontInitializerFailed);
    }
}


void pgDrawMgr::fontFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    if (!pgLowLevelAPI::deleteFreeTypeFont(exinfo))
    {
        pgThrow(ExceptionFontFinalizerFailed);
    }
}
