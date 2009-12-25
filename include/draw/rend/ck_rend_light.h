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
