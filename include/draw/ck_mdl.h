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


class pgRend_Light;


/*!
    @ingroup pgDraw
    TODO
*/
class PG_API pgMdl
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotInitialized);

    pgMdl();
    ~pgMdl();

    void init(pgID mdl_data_id, pgID scr_id);
    void init(pgID mdl_data_id, pgDraw* parent);

    const pgMdlData* getModelData() const;

    pgID getTextureID() const;
    void setTextureID(pgID tex_id);

    pgID getLightSetID() const;
    void setLightSetID(pgID lts_id);

    pgDraw* getRootDraw();
    pgDraw* getNodeDraw(u16 node_index);

private:
    pgMdl(const pgMdl&);
    void operator=(const pgMdl&);

    void init2(pgID mdl_data_id, pgID scr_id, pgDraw* parent);
    void uninit();

    pgMdlData m_mdl_data;
    pgDraw** m_draw_ptr;
    pgRend_Light** m_rend_ptr;
    pgNode m_root_node;
    u16 m_node_num;
    pgID m_lts_id;
};
