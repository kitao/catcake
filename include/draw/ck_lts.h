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
    @ingroup ckDraw
    TODO
*/
class CK_API ckLts
{
    friend class ckDrawMgr;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionSameIDExists);

    static const u32 MAX_PARA_LIGHT_NUM = 3;
    static const u32 MAX_NEAR_LIGHT_NUM = 3;

    ckLts* getPrevN() const;
    ckLts* getNextN() const;

    ckID getID() const;

    ckCol getAmbientColor() const;
    void setAmbientColor(ckCol col);

    bool isParaLightEnabled(u8 index) const;
    void setParaLightEnabled(u8 index, bool is_enabled);

    const ckVec& getParaLightDir(u8 index) const;
    void setParaLightDir(u8 index, const ckVec& dir);

    ckCol getParaLightColor(u8 index) const;
    void setParaLightColor(u8 index, ckCol col);

    u32 getPointLightNum() const;
    ckLit* getPointLight(ckID lit_id);
    ckLit* newPointLight(ckID lit_id);
    void deletePointLight(ckID lit_id);
    ckLit* getFirstPointLightN() const;
    ckLit* getLastPointLightN() const;
    void clearPointLight();

    void findNearLight(const ckVec& pos);
    u8 getNearLightNum() const;
    const ckVec& getNearLightDir(u8 index) const;
    ckCol getNearLightColor(u8 index) const;

private:
    struct ParaLight
    {
        ckType<u8, bool> is_enabled;
        ckVec dir;
        ckCol col;
        u16 col_int;
    };

    struct NearLight
    {
        ckVec dir;
        ckCol col;
        u16 col_int;
    };

    ckLts(ckID lts_id);
    ~ckLts();
    void operator=(const ckLts&);

    ckLit* getPointLightN(ckID lit_id);

    void addLightToNearLight(const ckVec& dir, ckCol col, u16 col_int);

    ckID m_id;
    ckCol m_amb_col;
    ParaLight m_para_lit[MAX_PARA_LIGHT_NUM];
    u8 m_near_lit_num;
    u8 m_near_lit_index_tbl[MAX_NEAR_LIGHT_NUM];
    NearLight m_near_lit[MAX_NEAR_LIGHT_NUM];
    ckList<ckLit> m_lit_list;
};
