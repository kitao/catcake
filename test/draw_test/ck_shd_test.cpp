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

        ckShd* def_shd = ckDrawMgr::getShader(ckDrawMgr::DEFAULT_SHADER_ID);
        ckAssert(def_shd->getID() == ckDrawMgr::DEFAULT_SHADER_ID);
        ckAssert(!def_shd->getPrevN() && !def_shd->getNextN());

        ckID id = ckID::genID();
        ckShd* shd = ckDrawMgr::newShader(id, "dummy", "dummy", 0, 0, 0);
        ckAssert(shd->getID() == id);
        ckAssert(!def_shd->getPrevN() && def_shd->getNextN() == shd);
        ckAssert(shd->getPrevN() == def_shd && !shd->getNextN());

        ckDrawMgr::deleteShader(id);
        ckAssert(!def_shd->getPrevN() && !def_shd->getNextN());

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
