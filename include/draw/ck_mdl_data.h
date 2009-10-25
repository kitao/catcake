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


/*!
    @ingroup pgDraw
    TODO
*/
class PG_API pgMdlData
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionInvalidData);
    pgDefineException(ExceptionNotInitialized);

    static const u32 MODEL_DATA_VERSION = 50; // 0.5.0

    enum ModelDataMode
    {
        MODE_READ, //
        MODE_WRITE
    };

    pgMdlData();
    ~pgMdlData();

    void initAsReader(const void* data, u32 data_size);
    void initAsWriter(u16 node_num, u16 vert_num, pgID tex_id, bool has_normal);

    ModelDataMode getMode() const;
    u16 getNodeNum() const;
    u16 getVertNum() const;
    bool hasNormal() const;
    pgID getTextureID() const;

    u16 getNodeParentIndex(u16 node_index) const;
    void setNodeParentIndex(u16 node_index, u16 parent_index);
    void setNodeParentIndex_noParent(u16 node_index);

    const pgMat& getNodeLocal(u16 node_index) const;
    void setNodeLocal(u16 node_index, const pgMat& local);

    pgPrim::PrimMode getNodePrimMode(u16 node_index) const;
    void setNodePrimMode(u16 node_index, pgPrim::PrimMode prim_mode);

    u16 getNodeVertIndex(u16 node_index) const;
    u16 getNodeVertNum(u16 node_index) const;
    void setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num);

    const pgPrim::PrimData* getNodePrimData(u16 node_index) const;
    const pgVec* getNodeNormalData(u16 node_index) const;

    pgPrim::BlendMode getNodeBlendMode(u16 node_index) const;
    void setNodeBlendMode(u16 node_index, pgPrim::BlendMode blend_mode);

    const pgVec& getNodeClipBoundMinForReader(u16 node_index) const;
    const pgVec& getNodeClipBoundMaxForReader(u16 node_index) const;

    const pgVec& getVertPos(u16 vert_index) const;
    void setVertPos(u16 vert_index, const pgVec& pos);

    pgCol getVertCol(u16 vert_index) const;
    void setVertCol(u16 vert_index, pgCol col);

    r32 getVertU(u16 vert_index) const;
    r32 getVertV(u16 vert_index) const;
    void setVertUV(u16 vert_index, r32 u, r32 v);

    const pgVec& getVertN(u16 vert_index) const;
    void setVertN(u16 vert_index, const pgVec& n);

    void calcNormalAsTriangles(bool is_smoothing);

    void registerAsResource(pgID res_id);

private:
    enum ModelDataFlag
    {
        FLAG_HAS_NORMAL
    };

    struct ModelDataHeader
    {
        pgID format_id;
        u16 format_version;
        u16 node_num;
        u16 vert_num;
        pgFlag<u16, ModelDataFlag> flag;
        pgID tex_id;
        u32 prim_data_offset;
        u32 normal_data_offset;
    };

    struct NodeInfo
    {
        pgMat local;
        pgVec bound_min;
        pgVec bound_max;
        u16 parent_index;
        pgType<u8, pgPrim::PrimMode> prim_mode;
        pgType<u8, pgPrim::BlendMode> blend_mode;
        u16 vert_index;
        u16 vert_num;
    };

    pgMdlData(const pgMdlData&);
    void operator=(const pgMdlData&);

    pgType<u8, ModelDataMode> m_mode;
    u8* m_data;
    u32 m_data_size;
    ModelDataHeader* m_mdl_data_header;
    NodeInfo* m_node_info;
    pgPrim::PrimData* m_prim_data;
    pgVec* m_normal_data;
};
