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


#include "test.h"


void ckShdTest()
{
    /*
        ckShd* getPrevN() const
        ckShd* getNextN() const
        ckID getID() const
    */
    {
        ckDrawMgr::createAfterRes();

        ckShd* def_shd1 = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID);
        ckShd* def_shd2 = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(def_shd1->getID() == ckDrawMgr::DEFAULT_NO_TEXTURE_SHADER_ID && def_shd2->getID() == ckDrawMgr::DEFAULT_ALPHA_TEXTURE_SHADER_ID);
        ckAssert(!def_shd1->getPrevN() && !def_shd2->getNextN());

        ckID id = ckID::genID();
        ckShd* shd = ckDrawMgr::newShader(id, "dummy", "dummy", 0, 0, 0);
        ckAssert(shd->getID() == id);
        ckAssert(!def_shd1->getPrevN() && def_shd2->getNextN() == shd);
        ckAssert(shd->getPrevN() == def_shd2 && !shd->getNextN());

        ckDrawMgr::deleteShader(id);
        ckAssert(!def_shd1->getPrevN() && !def_shd2->getNextN());

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        u8 getUniformNum() const
        u8 getAttribNum() const
        u8 getTextureNum() const
        bool isValid() const
    */
    {
        static const char s_vert_code[] = "void main() { gl_Position = vec4(0.0, 0.0, 0.0, 0.0); }";
        static const char s_frag_code[] = "void main() { gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); }";

        ckDrawMgr::createAfterRes();

        ckShd* shd = ckDrawMgr::newShader(ckID::genID(), s_vert_code, s_frag_code, 4, 3, 2);

        ckAssert(shd->getUniformNum() == 4);
        ckAssert(shd->getAttribNum() == 3);
        ckAssert(shd->getTextureNum() == 2);

        if (ckDrawMgr::isShaderAvailable())
        {
            ckAssert(shd->isValid());
        }
        else
        {
            ckAssert(!shd->isValid());
        }

        ckDrawMgr::destroyBeforeRes();
    }
}
