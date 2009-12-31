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


void ckIDTest()
{
    /*
        static const ckID ZERO
    */
    {
        ckAssert(ckID::ZERO.getValue() == 0);
    }

    /*
        ckID()
        u32 operator%(u32 n) const
        u32 getValue() const
        ckID genID(const char* str)
    */
    {
        ckID id1;
        ckAssert(id1.getValue() == 0);
        ckAssert(id1 % 2 == 0);

        ckID id2 = ckID::genID("TestA");
        ckAssert(id2.getValue() != 0 && (id2.getValue() & (1 << 31)));
        ckAssert(id2 % 3 == id2.getValue() % 3);

        ckID id3 = ckID::genID("TestA");
        ckAssert(id3.getValue() == id2.getValue());
        ckAssert(id3 % 5 == id2 % 5);

        ckID id4 = ckID::genID("TestB");
        ckAssert(id4.getValue() != id2.getValue() && (id4.getValue() & (1 << 31)));

        ckID id5 = id2;
        ckAssert(id5.getValue() == id2.getValue());

        ckAssert(ckID::genID("0123456789012345678901234567890123456789") == ckID::genID("0123456789012345678901234567890123456789"));
        ckAssert(ckID::genID("0123456789012345678901234567890123456789") != ckID::genID("012345678901234567890123456789012345678"));

        ckAssertThrow(ckID::genID(NULL), ckID::ExceptionInvalidArgument);
    }

    /*
        bool operator==(ckID id) const
        bool operator!=(ckID id) const
    */
    {
        ckID id1 = ckID::genID("TestC");
        ckID id2 = ckID::genID("TestC");
        ckID id3 = ckID::genID("TestD");

        ckAssert(id1 == id2);
        ckAssert(!(id1 == id3));

        ckAssert(id1 != id3);
        ckAssert(!(id1 != id2));
    }

    /*
        static ckID genIDForSystem(u32 value)
    */
    {
        ckAssert(ckID::genIDForSystem(123).getValue() == 123);
        ckAssert(ckID::genIDForSystem(0).getValue() == 0);
    }

    /*
        static ckID genID()
        static void setCurIDForSystem(u32 value)
    */
    {
        ckAssert(ckID::genID().getValue() == 1);
        ckAssert(ckID::genID().getValue() == 2);
        ckAssert(ckID::genID().getValue() == 3);

        ckAssertThrow(ckID::setCurIDForSystem(0), ckID::ExceptionInvalidArgument);
        ckAssertThrow(ckID::setCurIDForSystem(0x80000000), ckID::ExceptionInvalidArgument);

        ckID::setCurIDForSystem(0x7fffffff);
        ckAssert(ckID::genID().getValue() == 0x7fffffff);

        ckAssertThrow(ckID::genID(), ckID::ExceptionOutOfID);

        ckID::setCurIDForSystem(1);

        ckAssert(ckID::genID() % 3 == 1);
        ckAssert(ckID::genID() % 3 == 2);
        ckAssert(ckID::genID() % 3 == 0);
    }

    /*
        ckID_(str)
    */
    {
        ckAssert(ckID_("catcake") == ckID::genID("catcake"));
        ckAssert(ckID_("this is test") == ckID::genID("this is test"));
        ckAssert(ckID_("") == ckID::genID(""));

        ckAssert(ckID_("0123456789012345678901234567890") == ckID::genID("0123456789012345678901234567890"));
        ckAssert(ckID_("01234567890123456789012345678901") == ckID::genID("01234567890123456789012345678901"));

        ckAssertThrow(ckID_("012345678901234567890123456789012"), ckID::ExceptionTooLongString);
    }
}
