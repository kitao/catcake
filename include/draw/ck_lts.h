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


/*!
    @ingroup ckDraw
    A light set, which manages point lights, parallel lights, and ambient lights.
*/
class CK_API ckLts
{
    friend class ckDrawMgr;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionSameIDExists);
    //! @endcond

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
