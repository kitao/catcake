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


#include "ck_util_all.h"

#include "ck_sys_all.h"
#include "ck_res_all.h"


void ckUtil::import3DSModel(const char* filename, ckID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
{
    return import3DSModelAs(ckID::genID(ckUtil::getBasename(filename)), filename, tex_id, has_normal, is_smoothing_normal, scale);
}


static void setNodeInfo(ckMdlData* mdl_data, u16 node_index, const ckMat& local, ckDraw::BlendMode blend_mode, //
    u16 vert_index, u16 vert_num, const u8* vert_array, const u8* uv_array, const u8* face_array, r32 scale)
{
    ckMat scaled_local = local;
    scaled_local.trans *= scale;

    mdl_data->setNodeLocal(node_index, scaled_local);
    mdl_data->setNodePrimMode(node_index, ckPrim::MODE_TRIANGLES);
    mdl_data->setNodeBlendMode(node_index, blend_mode);
    mdl_data->setNodeVertInfo(node_index, vert_index, vert_num);

    if (!face_array)
    {
        return;
    }

    if (vert_array)
    {
        if (scale != 1.0f)
        {
            for (s32 i = 0; i < vert_num; i++)
            {
                u16 array_index = ckReadLittleEndian(face_array + (i + i / 3) * sizeof(u16), u16) * 3;
                ckVec vert_pos( //
                    ckReadLittleEndian(vert_array + sizeof(r32) * (array_index + 0), r32), //
                    ckReadLittleEndian(vert_array + sizeof(r32) * (array_index + 1), r32), //
                    ckReadLittleEndian(vert_array + sizeof(r32) * (array_index + 2), r32));

                mdl_data->setVertPos(vert_index + i, vert_pos * scale);
            }
        }
        else
        {
            for (s32 i = 0; i < vert_num; i++)
            {
                u16 array_index = ckReadLittleEndian(face_array + (i + i / 3) * sizeof(u16), u16) * 3;
                ckVec vert_pos( //
                    ckReadLittleEndian(vert_array + sizeof(r32) * (array_index + 0), r32), //
                    ckReadLittleEndian(vert_array + sizeof(r32) * (array_index + 1), r32), //
                    ckReadLittleEndian(vert_array + sizeof(r32) * (array_index + 2), r32));

                mdl_data->setVertPos(vert_index + i, vert_pos);
            }
        }
    }

    if (uv_array)
    {
        for (s32 i = 0; i < vert_num; i++)
        {
            u16 array_index = ckReadLittleEndian(face_array + (i + i / 3) * sizeof(u16), u16) * 2;
            r32 u = ckReadLittleEndian(uv_array + sizeof(r32) * (array_index + 0), r32);
            r32 v = -ckReadLittleEndian(uv_array + sizeof(r32) * (array_index + 1), r32);

            if (u > 1.0f)
            {
                u -= static_cast<s32>(u);
            }
            else if (u < 0.0f)
            {
                u += static_cast<s32>(u) + 1.0f;
            }

            if (v > 1.0f)
            {
                v -= static_cast<s32>(v);
            }
            else if (v < 0.0f)
            {
                v += static_cast<s32>(v) + 1.0f;
            }

            mdl_data->setVertUV(vert_index + i, u, v);
        }
    }
}


void ckUtil::import3DSModelAs(ckID res_id, const char* filename, ckID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
{
    if ((!has_normal && is_smoothing_normal) || scale <= 0.0f)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckID temp_res_id = ckID::genID();

    ckResMgr::loadResourceAs(temp_res_id, filename, false);

    ckRes temp_res = ckResMgr::getResource(temp_res_id);

    /*
        count up the number of objects and vertices in 3ds file
    */
    const u8* data = temp_res.getData<u8>();
    const u8* data_end = data + temp_res.getDataSize();

    u32 node_num = 0;
    u32 vert_num = 0;

    while (data < data_end)
    {
        u16 chunk_id = ckReadLittleEndian(data, u16);
        data += 2;

        u32 chunk_size = ckReadLittleEndian(data, u32);
        data += 4;

        switch (chunk_id)
        {
        case 0x4d4d: // MAIN CHUNK
            break;

        case 0x3d3d: // 3D EDITOR CHUNK
            break;

        case 0x4000: // OBJECT BLOCK
            node_num++;
            data += ckUtil::strlen(reinterpret_cast<const char*>(data)) + 1;
            break;

        case 0x4100: // TRIANGULAR MESH
            break;

        case 0x4120: // FACES DESCRIPTION
            vert_num += ckReadLittleEndian(data, u16) * 3;
            data += chunk_size - 6;
            break;

        default:
            data += chunk_size - 6;
            break;
        }
    }

    if (node_num == 0 || node_num > 0xffff || vert_num == 0 || vert_num > 0xffff)
    {
        ckResMgr::removeResource(temp_res_id);
        ckThrow(ExceptionInvalidData);
    }

    /*
        make ckMdlData from 3ds file
    */
    ckMdlData mdl_data;
    mdl_data.initAsWriter(node_num, vert_num, tex_id, has_normal);

    bool is_first_node = true;
    u16 node_vert_num = 0;
    ckMat node_local = ckMat::UNIT;
    ckDraw::BlendMode node_blend_mode = ckDraw::BLEND_OFF;
    u16 node_index = 0;
    u16 vert_index = 0;
    u16 tree_node_index = 0;
    const u8* vert_array = NULL;
    const u8* uv_array = NULL;
    const u8* face_array = NULL;

    data = temp_res.getData<u8>();

    while (data < data_end)
    {
        u16 chunk_id = ckReadLittleEndian(data, u16);
        data += 2;

        u32 chunk_size = ckReadLittleEndian(data, u32);
        data += 4;

        switch (chunk_id)
        {
        case 0x4d4d: // MAIN CHUNK
            break;

        case 0x3d3d: // 3D EDITOR CHUNK
            break;

        case 0x4000: // OBJECT BLOCK
            if (is_first_node)
            {
                is_first_node = false;
            }
            else
            {
                setNodeInfo(&mdl_data, node_index, node_local, node_blend_mode, //
                    vert_index, node_vert_num, vert_array, uv_array, face_array, scale);

                node_index++;
                vert_index += node_vert_num;

                node_local = ckMat::UNIT;
                node_vert_num = 0;
                vert_array = NULL;
                uv_array = NULL;
                face_array = NULL;
            }

            switch (*reinterpret_cast<const char*>(data))
            {
            case '*':
                node_blend_mode = ckDraw::BLEND_HALF;
                break;

            case '+':
                node_blend_mode = ckDraw::BLEND_ADD;
                break;

            default:
                node_blend_mode = ckDraw::BLEND_OFF;
                break;
            }

            data += ckUtil::strlen(reinterpret_cast<const char*>(data)) + 1;
            break;

        case 0x4100: // TRIANGULAR MESH
            break;

        case 0x4110: // VERTICES LIST
            vert_array = data + 2;
            data += chunk_size - 6;
            break;

        case 0x4120: // FACES DESCRIPTION
            node_vert_num = ckReadLittleEndian(data, u16) * 3;
            face_array = data + 2;
            data += chunk_size - 6;
            break;

        case 0x4140: // MAPPING COORDINATES LIST
            uv_array = data + 2;
            data += chunk_size - 6;
            break;

        case 0x4160: // LOCAL COORDINATES SYSTEM
            node_local = *reinterpret_cast<const ckMat*>(data);
            data += chunk_size - 6;
            break;

        case 0xb000: // KEYFRAMER CHUNK
            break;

        case 0xb002: // MESH INFORMATION BLOCK
            break;

        case 0xb010: // OBJECT NAME
            {
                const u8* parent_index_data = data + ckUtil::strlen(reinterpret_cast<const char*>(data)) + 5;
                s16 parent_index = ckReadLittleEndian(parent_index_data, s16);

                if (parent_index >= 0 && parent_index < static_cast<u16>(node_num))
                {
                    mdl_data.setNodeParentIndex(tree_node_index, parent_index);
                }
                else
                {
                    mdl_data.setNodeParentIndex_noParent(tree_node_index);
                }

                tree_node_index++;
            }

            data += chunk_size - 6;
            break;

        default:
            data += chunk_size - 6;
            break;
        }
    }

    if (!is_first_node)
    {
        setNodeInfo(&mdl_data, node_index, node_local, node_blend_mode, vert_index, //
            node_vert_num, vert_array, uv_array, face_array, scale);
    }

    if (has_normal)
    {
        mdl_data.calcNormalAsTriangles(is_smoothing_normal);
    }

    mdl_data.registerAsResource(res_id);

    ckResMgr::removeResource(temp_res_id);
}
