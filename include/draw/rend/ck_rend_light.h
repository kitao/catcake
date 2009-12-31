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
    A custom renderer for lighting.
*/
class CK_API ckRend_Light : public ckRend
{
public:
    /*!
        A renderer data
    */
    struct RendData
    {
        ckVec normal; //!< The normal.
    };

    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] lts_id A light-set ID.
    */
    void init(ckPrim* prim, ckID lts_id);

    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] rend_data Shared renderer data.
        @param[in] lts_id A light-set ID.
    */
    void init(ckPrim* prim, RendData* rend_data, ckID lts_id);

    /*!
        Returns the light-set ID.
        @return The light-set ID.
    */
    ckID getLightSetID();

    /*!
        Sets the light-set ID.
        @param[in] lts_id A light-set ID.
    */
    void setLightSetID(ckID lts_id);

    /*!
        Returns the reference to the specified renderer data.
        @param[in] index The index of a renderer data.
        @return The reference to the specified renderer data.
    */
    ckVec& dataN(u16 index);

    /*!
        Calculates the normals as triangles.
        @param[in] is_smoothing If true, the normals are smoothed.
    */
    void calcNormalAsTriangles(bool is_smoothing);

private:
    struct RendBody
    {
        ckLts* lts;
    };

    virtual ckID getClassID();
    virtual void initData(void* data, u16 data_num);
    virtual void render(const ckMat& view);
    void render_soft(const ckMat& view);
    void render_shader(const ckMat& view);
};
