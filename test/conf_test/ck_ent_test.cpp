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


void pgEntTest()
{
    /*
        static const u32 MAX_NAME_LENGTH
    */
    {
        pgAssert(pgEnt::MAX_NAME_LENGTH == 20);
    }

    /*
        pgEnt* getPrevN() const
        pgEnt* getNextN() const
        const pgStr<char, MAX_NAME_LENGTH>& getName() const
        u16 getValueNum() const
        ValueType getValueType(u16 index) const
        s32 getValue_s32(u16 index) const
        r32 getValue_r32(u16 index) const
        const char* getValue_string(u16 index) const
    */
    {
        pgConfMgr::createAfterRes();

        pgResMgr::loadResource(TEST_DATA_DIR "test_config1.pgc", true);

        pgConf* conf = pgConfMgr::getConfig(pgID_("test_config1.pgc"));

        pgEnt* ent1 = conf->getEntryFromFirstN("test_s32");
        pgAssert(ent1->getName() == "test_s32");
        pgAssert(ent1->getValueNum() == 1);
        pgAssert(ent1->getValueType(0) == pgEnt::TYPE_S32);
        pgAssert(ent1->getValue_s32(0) == 123 && isEqual(ent1->getValue_r32(0), 123.0f) && (pgStr<char, 8>(ent1->getValue_string(0))) == "");

        pgAssert(ent1->getValueType(1) == pgEnt::TYPE_S32);
        pgAssert(ent1->getValue_s32(1) == 0 && isEqual(ent1->getValue_r32(1), 0.0f) && (pgStr<char, 8>(ent1->getValue_string(1))) == "");

        pgEnt* ent2 = conf->getEntryFromFirstN("test_r32");
        pgAssert(ent2->getName() == "test_r32");
        pgAssert(ent2->getValueNum() == 1);
        pgAssert(ent2->getValueType(0) == pgEnt::TYPE_R32);
        pgAssert(ent2->getValue_s32(0) == 4 && isEqual(ent2->getValue_r32(0), 4.56f) && (pgStr<char, 8>(ent2->getValue_string(0))) == "");

        pgEnt* ent3 = conf->getEntryFromFirstN("test_string");
        pgAssert(ent3->getName() == "test_string");
        pgAssert(ent3->getValueNum() == 1);
        pgAssert(ent3->getValueType(0) == pgEnt::TYPE_STRING);
        pgAssert(ent3->getValue_s32(0) == 0 && isEqual(ent3->getValue_r32(0), 0.0f) && (pgStr<char, 8>(ent3->getValue_string(0))) == "789");

        pgEnt* ent4 = conf->getEntryFromFirstN("test_tuple");
        pgAssert(ent4->getName() == "test_tuple");
        pgAssert(ent4->getValueNum() == 3);
        pgAssert(ent4->getValueType(0) == pgEnt::TYPE_S32 && ent4->getValueType(1) == pgEnt::TYPE_R32 && ent4->getValueType(2) == pgEnt::TYPE_STRING);
        pgAssert(ent4->getValue_s32(0) == -111 && isEqual(ent4->getValue_r32(1), -2.22f) && (pgStr<char, 8>(ent4->getValue_string(2))) == "333");

        pgEnt* ent5 = conf->getEntryFromFirstN("D");
        pgAssert(ent5->getName() == "D");
        pgAssert(ent5->getValueNum() == 1);
        pgAssert(ent5->getValueType(0) == pgEnt::TYPE_S32);
        pgAssert(ent5->getValue_s32(0) == 4 && isEqual(ent5->getValue_r32(0), 4.0f) && (pgStr<char, 8>(ent5->getValue_string(0))) == "");

        pgEnt* ent6 = conf->getEntryFromFirstN("_");
        pgAssert(ent6->getValue_string(0)[0] == '\n' && ent6->getValue_string(0)[1] == '\t' && //
            ent6->getValue_string(0)[2] == '\\' && ent6->getValue_string(0)[3] == '"' && ent6->getValue_string(0)[4] == 'x');

        pgEnt* ent7 = conf->getEntryFromFirstN("section");
        pgEnt* ent8 = conf->getEntryFromLastN("A");
        pgAssert(!ent7->getPrevN() && ent7->getNextN() == ent1 && ent1->getPrevN() == ent7);
        pgAssert(ent6->getNextN() == ent8 && ent8->getPrevN() == ent6 && !ent8->getNextN());

        pgResMgr::removeResource(pgID_("test_config1.pgc"));

        pgConfMgr::destroyBeforeRes();
    }
}
