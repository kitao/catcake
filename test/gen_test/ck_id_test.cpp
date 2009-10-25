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


void pgIDTest()
{
    /*
        static const pgID ZERO
    */
    {
        pgAssert(pgID::ZERO.getValue() == 0);
    }

    /*
        pgID()
        u32 operator%(u32 n) const
        u32 getValue() const
        pgID genID(const char* str)
    */
    {
        pgID id1;
        pgAssert(id1.getValue() == 0);
        pgAssert(id1 % 2 == 0);

        pgID id2 = pgID::genID("TestA");
        pgAssert(id2.getValue() != 0 && (id2.getValue() & (1 << 31)));
        pgAssert(id2 % 3 == id2.getValue() % 3);

        pgID id3 = pgID::genID("TestA");
        pgAssert(id3.getValue() == id2.getValue());
        pgAssert(id3 % 5 == id2 % 5);

        pgID id4 = pgID::genID("TestB");
        pgAssert(id4.getValue() != id2.getValue() && (id4.getValue() & (1 << 31)));

        pgID id5 = id2;
        pgAssert(id5.getValue() == id2.getValue());

        pgAssertThrow(pgID::genID(NULL), pgID::ExceptionInvalidArgument);
    }

    /*
        bool operator==(pgID id) const
        bool operator!=(pgID id) const
    */
    {
        pgID id1 = pgID::genID("TestC");
        pgID id2 = pgID::genID("TestC");
        pgID id3 = pgID::genID("TestD");

        pgAssert(id1 == id2);
        pgAssert(!(id1 == id3));

        pgAssert(id1 != id3);
        pgAssert(!(id1 != id2));
    }

    /*
        static pgID genIDForSystem(u32 value)
    */
    {
        pgAssert(pgID::genIDForSystem(123).getValue() == 123);
        pgAssert(pgID::genIDForSystem(0).getValue() == 0);
    }

    /*
        static pgID genID()
        static void setCurIDForSystem(u32 value)
    */
    {
        pgAssert(pgID::genID().getValue() == 1);
        pgAssert(pgID::genID().getValue() == 2);
        pgAssert(pgID::genID().getValue() == 3);

        pgAssertThrow(pgID::setCurIDForSystem(0), pgID::ExceptionInvalidArgument);
        pgAssertThrow(pgID::setCurIDForSystem(0x80000000), pgID::ExceptionInvalidArgument);

        pgID::setCurIDForSystem(0x7fffffff);
        pgAssert(pgID::genID().getValue() == 0x7fffffff);

        pgAssertThrow(pgID::genID(), pgID::ExceptionOutOfID);

        pgID::setCurIDForSystem(1);

        pgAssert(pgID::genID() % 3 == 1);
        pgAssert(pgID::genID() % 3 == 2);
        pgAssert(pgID::genID() % 3 == 0);
    }

    /*
        pgID_(str)
    */
    {
        pgAssert(pgID_("pogolyn") == pgID::genID("pogolyn"));
        pgAssert(pgID_("this is test") == pgID::genID("this is test"));
        pgAssert(pgID_("") == pgID::genID(""));
        pgAssert(pgID_("01234567890123456789012345678901") == pgID::genID("01234567890123456789012345678901"));
        pgAssert(pgID_("012345678901234567890123456789010") != pgID::genID("012345678901234567890123456789010"));
    }
}
