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
    A sprite, a rectangle which always faces the camera.
*/
class CK_API ckSprt : public ckDraw
{
public:
    /*!
        Sprite data.
    */
    struct CK_API SprtData
    {
        ckVec pos; //!< Position.
        ckCol col; //!< Color.
        r32 w; //!< Width.
        r32 h; //!< Height.
        r32 u1; //!< The u value of the left-top vertex.
        r32 v1; //!< The v value of the left-top vertex.
        r32 u2; //!< The u value of the right-bottom vertex.
        r32 v2; //!< The v value of the right-bottom vertex.
        s32 ang; //!< Z-rotation.

        SprtData(); //!< Constructs a sprite data.
    };

    /*!
        Constructs a sprite.
    */
    ckSprt();

    /*!
        Destructs this sprite.
    */
    virtual ~ckSprt();

    /*!
        Initializes this sprite.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] scr_id A screen ID.
    */
    void init(u16 max_data_num, ckID scr_id);

    /*!
        Initializes this sprite.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] parent A parent drawing element.
    */
    void init(u16 max_data_num, ckDraw* parent);

    /*!
        Initializes this sprite.
        @param[in] sprt_data A shared sprite data.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] scr_id A screen ID.
    */
    void init(SprtData* sprt_data, u16 max_data_num, ckID scr_id);

    /*!
        Initializes this sprite.
        @param[in] sprt_data A shared sprite data.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] parent A parent drawing element.
    */
    void init(SprtData* sprt_data, u16 max_data_num, ckDraw* parent);

    /*!
        Returns whether shering sprite data.
        @return Whether shering sprite data.
    */
    bool isShareData() const;

    /*!
        Returns the number of used sprite data.
        @return The number of used sprite data.
    */
    u16 getCurDataNum() const;

    /*!
        Sets the number of used sprite data.
        @param[in] cur_data_num The number of used sprite data.
    */
    void setCurDataNum(u16 cur_data_num);

    /*!
        Returns the maximum number of the sprite data.
    */
    u16 getMaxDataNum() const;

    /*!
        Changes the maximum number of the sprite data.
    */
    void reallocData(u16 max_data_num);

    /*!
        Copies the specified sprite data.
        @param[in] dest_index The index of destination sprite data.
        @param[in] src_sprt A source primitive.
        @param[in] src_index The index of souce primitive data. 
    */
    void copyData(u16 dest_index, const ckSprt* src_sprt, u16 src_index);

    /*!
        Returns the reference to the position of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the position of the specified sprite data.
    */
    ckVec& dataPos(u16 index);

    /*!
        Returns the reference to the color of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the color of the specified sprite data.
    */
    ckCol& dataCol(u16 index);

    /*!
        Returns the reference to the width of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the width of the specified sprite data.
    */
    r32& dataW(u16 index);

    /*!
        Returns the reference to the height of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the height of the specified sprite data.
    */
    r32& dataH(u16 index);

    /*!
        Returns the reference to the u value of the left-top vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the u value of the left-top vertex of the specified sprite data.
    */
    r32& dataU1(u16 index);

    /*!
        Returns the reference to the v value of the left-top vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the v value of the left-top vertex of the specified sprite data.
    */
    r32& dataV1(u16 index);

    /*!
        Returns the reference to the u value of the right-bottom vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the u value of the right-bottom vertex of the specified sprite data.
    */
    r32& dataU2(u16 index);

    /*!
        Returns the reference to the v value of the right-bottom vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the v value of the right-bottom vertex of the specified sprite data.
    */
    r32& dataV2(u16 index);

    /*!
        Returns the reference to the angle of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the angle of the specified sprite data.
    */
    s32& dataAng(u16 index);

    /*!
        Sets the size of the specified sprite data.
        @param[in] index The index of a sprite data.
        @param[in] width The width of a aprite data.
        @param[in] height The height of a aprite data.
    */
    void setDataSize(u16 index, r32 width, r32 height);

    /*!
        Sets the uv-value of the specified sprite data.
        @param[in] index The index of a sprite data.
        @param[in] u1 The u value of the left-top vertex of a sprite data.
        @param[in] v1 The v value of the left-top vertex of a sprite data.
        @param[in] u2 The u value of the right-bottom vertex of a sprite data.
        @param[in] v2 The v value of the right-bottom vertex of a sprite data.
    */
    void setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2);

private:
    void init2(bool is_share_data, SprtData* sprt_data, u16 max_data_num, ckID scr_id, ckDraw* parent);

    virtual void render(const ckMat& view);
    void render_soft(const ckMat& view);
    void render_shader(const ckMat& view);

    ckType<u8, bool> m_is_share_data;
    SprtData* m_sprt_data;
    u16 m_cur_data_num;
    u16 m_max_data_num;
};
