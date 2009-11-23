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
    A light set.
*/
class CK_API ckLts
{
    friend class ckDrawMgr;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionSameIDExists);

    /*!
        The maximum number of parallel lights.
    */
    static const u32 MAX_PARA_LIGHT_NUM = 3;

    /*!
        The muximum number of valid lights.
    */
    static const u32 MAX_NEAR_LIGHT_NUM = 3;

    /*!
        Returns the previous light set.
        @return The previous light set.
    */
    ckLts* getPrevN() const;

    /*!
        Returns the next light set.
        @return The next light set.
    */
    ckLts* getNextN() const;

    /*!
        Returns the ID of this light set.
        @return The ID of this light set.
    */
    ckID getID() const;

    /*!
        Returns the ambient color.
        @return The ambient color.
    */
    ckCol getAmbientColor() const;

    /*!
        Sets the ambient color.
        @param[in] col A color.
    */
    void setAmbientColor(ckCol col);

    /*!
        Returns whether the specified parallel light is active.
        @param[in] index The index of a parallel light.
        @return Whether the specified parallel light is active.
    */
    bool isParaLightActive(u8 index) const;

    /*!
        Determines whether the specified parallel light is active.
        @param[in] index The index of a parallel light.
        @param[in] is_active Whether a parallel light is active.
    */
    void setParaLightActive(u8 index, bool is_active);

    /*!
        Returns the direction of the specified parallel light.
        @param[in] index The index of a parallel light.
        @return The direction of the specified parallel light.
    */
    const ckVec& getParaLightDir(u8 index) const;

    /*!
        Sets the direction of the specified parallel light.
        @param[in] index The index of a parallel light.
        @param[in] dir The direction of a parallel light.
    */
    void setParaLightDir(u8 index, const ckVec& dir);

    /*!
        Returns the color of the specified parallel light.
        @param[in] index The index of a parallel light.
        @return The color of the specified parallel light.
    */
    ckCol getParaLightColor(u8 index) const;

    /*!
        Sets the color of the specified parallel light.
        @param[in] index The index of a parallel light.
        @param[in] col The color of a parallel light.
    */
    void setParaLightColor(u8 index, ckCol col);

    /*!
        Returns the number of point lights.
        @return The number of point lights.
    */
    u32 getPointLightNum() const;

    /*!
        Returns the specified point light.
        @param[in] lit_id The ID of a point light.
        @return The specified point light.
    */
    ckLit* getPointLight(ckID lit_id);

    /*!
        Creates a point light.
        @param[in] lit_id The ID of a point light.
        @return A point light.
    */
    ckLit* newPointLight(ckID lit_id);

    /*!
        Deletes the specified point light.
        @param[in] lit_id The ID of a point light.
    */
    void deletePointLight(ckID lit_id);

    /*!
        Returns the first point light. If the first point light doesn't exist, returns NULL.
        @return The first point light.
    */
    ckLit* getFirstPointLightN() const;

    /*!
        Returns the last point light. If the last point light doesn't exist, returns NULL.
        @return The last point light.
    */
    ckLit* getLastPointLightN() const;

    /*!
        Removes the all point lights.
    */
    void clearPointLight();

    /*!
        Finds the nearest lights of the specified position.
        @param[in] pos A position.
    */
    void findNearLight(const ckVec& pos);

    /*!
        Returns the number of the nearest lights.
    */
    u8 getNearLightNum() const;

    /*!
        Returns the direction of the specified nearest light.
        @param[in] index The index of a nearest light.
        @return The direction of the specified nearest light.
    */
    const ckVec& getNearLightDir(u8 index) const;

    /*!
        Returns the color of the specified nearest light.
        @param[in] index The index of a nearest light.
        @return The color of the specified nearest light.
    */
    ckCol getNearLightColor(u8 index) const;

private:
    struct ParaLight
    {
        ckType<u8, bool> is_active;
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
