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


void pgShdTest()
{
    /*
        pgShd* getPrevN() const
        pgShd* getNextN() const
        pgID getID() const
    */
    {
        pgDrawMgr::createAfterRes();

        pgShd* def_shd = pgDrawMgr::getShader(pgDrawMgr::DEFAULT_SHADER_ID);
        pgAssert(def_shd->getID() == pgDrawMgr::DEFAULT_SHADER_ID);
        pgAssert(!def_shd->getPrevN() && !def_shd->getNextN());

        pgID id = pgID::genID();
        pgShd* shd = pgDrawMgr::newShader(id, "dummy", "dummy", 0, 0, 0);
        pgAssert(shd->getID() == id);
        pgAssert(!def_shd->getPrevN() && def_shd->getNextN() == shd);
        pgAssert(shd->getPrevN() == def_shd && !shd->getNextN());

        pgDrawMgr::deleteShader(id);
        pgAssert(!def_shd->getPrevN() && !def_shd->getNextN());

        pgDrawMgr::destroyBeforeRes();
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

        pgDrawMgr::createAfterRes();

        pgShd* shd = pgDrawMgr::newShader(pgID::genID(), s_vert_code, s_frag_code, 4, 3, 2);

        pgAssert(shd->getUniformNum() == 4);
        pgAssert(shd->getAttribNum() == 3);
        pgAssert(shd->getTextureNum() == 2);

        if (pgDrawMgr::isShaderAvailable())
        {
            pgAssert(shd->isValid());
        }
        else
        {
            pgAssert(!shd->isValid());
        }

        pgDrawMgr::destroyBeforeRes();
    }
}
