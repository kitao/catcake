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


class ckRend_Light;


/*!
    @ingroup ckDraw
    A 3D model, which consists of drawing element nodes.
*/
class CK_API ckMdl
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        Constructs a model.
    */
    ckMdl();

    /*!
        Destructs this model.
    */
    ~ckMdl();

    /*!
        Initializes this model.
        @param[in] mdl_data_id The ID of a model data.
        @param[in] scr_id The ID of a screen.
    */
    void init(ckID mdl_data_id, ckID scr_id);

    /*!
        Initializes this model.
        @param[in] mdl_data_id The ID of a model data.
        @param[in] parent The ID of a drawing object.
    */
    void init(ckID mdl_data_id, ckDraw* parent);

    /*!
        Returns the model data.
        @return The model data.
    */
    const ckMdlData* getModelData() const;

    /*!
        Returns the texture ID.
        @return The texture ID.
    */
    ckID getTextureID() const;

    /*!
        Sets the texture ID.
        @param[in] tex_id A texture ID.
    */
    void setTextureID(ckID tex_id);

    /*!
        Returns the ID of the light set.
        @return The ID of the light set.
    */
    ckID getLightSetID() const;

    /*!
        Sets the ID of the light set.
        @param[in] lts_id The ID of the light set.
    */
    void setLightSetID(ckID lts_id);

    /*!
        Returns the root drawing object.
        @return The root drawing object.
    */
    ckDraw* getRootDraw();

    /*!
        Returns the drawing object of the specified node.
        @param[in] node_index The index of a node.
        @return The drawing object of the specified node.
    */
    ckDraw* getNodeDraw(u16 node_index);

private:
    ckMdl(const ckMdl&);
    void operator=(const ckMdl&);

    void init2(ckID mdl_data_id, ckID scr_id, ckDraw* parent);
    void uninit();

    ckMdlData m_mdl_data;
    ckDraw** m_draw_ptr;
    ckRend_Light** m_rend_ptr;
    ckNode m_root_node;
    u16 m_node_num;
    ckID m_lts_id;
};
