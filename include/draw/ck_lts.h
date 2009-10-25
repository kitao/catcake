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
class PG_API pgLts
{
    friend class pgDrawMgr;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotFound);
    pgDefineException(ExceptionSameIDExists);

    static const u32 MAX_PARA_LIGHT_NUM = 3;
    static const u32 MAX_NEAR_LIGHT_NUM = 3;

    pgLts* getPrevN() const;
    pgLts* getNextN() const;

    pgID getID() const;

    pgCol getAmbientColor() const;
    void setAmbientColor(pgCol col);

    bool isParaLightActive(u8 index) const;
    void setParaLightActive(u8 index, bool is_active);

    const pgVec& getParaLightDir(u8 index) const;
    void setParaLightDir(u8 index, const pgVec& dir);

    pgCol getParaLightColor(u8 index) const;
    void setParaLightColor(u8 index, pgCol col);

    u32 getPointLightNum() const;
    pgLit* getPointLight(pgID lit_id);
    pgLit* newPointLight(pgID lit_id);
    void deletePointLight(pgID lit_id);
    pgLit* getFirstPointLightN() const;
    pgLit* getLastPointLightN() const;
    void clearPointLight();

    void findNearLight(const pgVec& pos);
    u8 getNearLightNum() const;
    const pgVec& getNearLightDir(u8 index) const;
    pgCol getNearLightColor(u8 index) const;

private:
    struct ParaLight
    {
        pgType<u8, bool> is_active;
        pgVec dir;
        pgCol col;
        u16 col_int;
    };

    struct NearLight
    {
        pgVec dir;
        pgCol col;
        u16 col_int;
    };

    pgLts(pgID lts_id);
    ~pgLts();
    void operator=(const pgLts&);

    pgLit* getPointLightN(pgID lit_id);

    void addLightToNearLight(const pgVec& dir, pgCol col, u16 col_int);

    pgID m_id;
    pgCol m_amb_col;
    ParaLight m_para_lit[MAX_PARA_LIGHT_NUM];
    u8 m_near_lit_num;
    u8 m_near_lit_index_tbl[MAX_NEAR_LIGHT_NUM];
    NearLight m_near_lit[MAX_NEAR_LIGHT_NUM];
    pgList<pgLit> m_lit_list;
};
