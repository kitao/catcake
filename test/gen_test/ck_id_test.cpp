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


void ckIDTest()
{
    /*
        static const ckID ZERO
    */
    {
        ckAssert(ckID::ZERO.getValue() == 0);
    }

    /*
        static const u8 MAX_STRING_LENGTH
    */
    {
        ckAssert(ckID::MAX_STRING_LENGTH == 32);
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

        ckAssert(ckID::genID("0123456789012345678901234567890") != ckID::genID("01234567890123456789012345678901"));
        ckAssert(ckID::genID("01234567890123456789012345678901") == ckID::genID("012345678901234567890123456789012"));
        ckAssert(ckID::genID("01234567890123456789012345678901") == ckID::genID("0123456789012345678901234567890123"));

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
        ckAssert(ckID_("012345678901234567890123456789012") == ckID::genID("012345678901234567890123456789012"));
    }
}
