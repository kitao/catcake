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


void ckEntTest()
{
    /*
        static const u32 MAX_NAME_LENGTH
    */
    {
        ckAssert(ckEnt::MAX_NAME_LENGTH == 20);
    }

    /*
        ckEnt* getPrevN() const
        ckEnt* getNextN() const
        const ckStr<char, MAX_NAME_LENGTH>& getName() const
        u16 getValueNum() const
        ValueType getValueType(u16 index) const
        s32 getValue_s32(u16 index) const
        r32 getValue_r32(u16 index) const
        const char* getValue_string(u16 index) const
    */
    {
        ckConfMgr::createAfterRes();

        ckResMgr::loadResource(TEST_DATA_DIR "test_config1.ckl", true);

        ckConf* conf = ckConfMgr::getConfig(ckID_("test_config1.ckl"));

        ckEnt* ent1 = conf->getEntryFromFirstN("test_s32");
        ckAssert(ent1->getName() == "test_s32");
        ckAssert(ent1->getValueNum() == 1);
        ckAssert(ent1->getValueType(0) == ckEnt::TYPE_S32);
        ckAssert(ent1->getValue_s32(0) == 123 && isEqual(ent1->getValue_r32(0), 123.0f) && (ckStr<char, 8>(ent1->getValue_string(0))) == "");

        ckAssert(ent1->getValueType(1) == ckEnt::TYPE_S32);
        ckAssert(ent1->getValue_s32(1) == 0 && isEqual(ent1->getValue_r32(1), 0.0f) && (ckStr<char, 8>(ent1->getValue_string(1))) == "");

        ckEnt* ent2 = conf->getEntryFromFirstN("test_r32");
        ckAssert(ent2->getName() == "test_r32");
        ckAssert(ent2->getValueNum() == 1);
        ckAssert(ent2->getValueType(0) == ckEnt::TYPE_R32);
        ckAssert(ent2->getValue_s32(0) == 4 && isEqual(ent2->getValue_r32(0), 4.56f) && (ckStr<char, 8>(ent2->getValue_string(0))) == "");

        ckEnt* ent3 = conf->getEntryFromFirstN("test_string");
        ckAssert(ent3->getName() == "test_string");
        ckAssert(ent3->getValueNum() == 1);
        ckAssert(ent3->getValueType(0) == ckEnt::TYPE_STRING);
        ckAssert(ent3->getValue_s32(0) == 0 && isEqual(ent3->getValue_r32(0), 0.0f) && (ckStr<char, 8>(ent3->getValue_string(0))) == "789");

        ckEnt* ent4 = conf->getEntryFromFirstN("test_tuple");
        ckAssert(ent4->getName() == "test_tuple");
        ckAssert(ent4->getValueNum() == 3);
        ckAssert(ent4->getValueType(0) == ckEnt::TYPE_S32 && ent4->getValueType(1) == ckEnt::TYPE_R32 && ent4->getValueType(2) == ckEnt::TYPE_STRING);
        ckAssert(ent4->getValue_s32(0) == -111 && isEqual(ent4->getValue_r32(1), -2.22f) && (ckStr<char, 8>(ent4->getValue_string(2))) == "333");

        ckEnt* ent5 = conf->getEntryFromFirstN("D");
        ckAssert(ent5->getName() == "D");
        ckAssert(ent5->getValueNum() == 1);
        ckAssert(ent5->getValueType(0) == ckEnt::TYPE_S32);
        ckAssert(ent5->getValue_s32(0) == 4 && isEqual(ent5->getValue_r32(0), 4.0f) && (ckStr<char, 8>(ent5->getValue_string(0))) == "");

        ckEnt* ent6 = conf->getEntryFromFirstN("_");
        ckAssert(ent6->getValue_string(0)[0] == '\n' && ent6->getValue_string(0)[1] == '\t' && //
            ent6->getValue_string(0)[2] == '\\' && ent6->getValue_string(0)[3] == '"' && ent6->getValue_string(0)[4] == 'x');

        ckEnt* ent7 = conf->getEntryFromFirstN("section");
        ckEnt* ent8 = conf->getEntryFromLastN("A");
        ckAssert(!ent7->getPrevN() && ent7->getNextN() == ent1 && ent1->getPrevN() == ent7);
        ckAssert(ent6->getNextN() == ent8 && ent8->getPrevN() == ent6 && !ent8->getNextN());

        ckResMgr::removeResource(ckID_("test_config1.ckl"));

        ckConfMgr::destroyBeforeRes();
    }
}
