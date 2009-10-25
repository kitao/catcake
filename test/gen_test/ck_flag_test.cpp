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


enum TestFlag
{
    FLAG_TEST1, FLAG_TEST2, FLAG_TEST3, //
    FLAG_TEST4, FLAG_TEST5, FLAG_TEST6, //
    FLAG_TEST7, FLAG_TEST8, FLAG_TEST9
};


void pgFlagTest()
{
    /*
        pgFlag()
        bool operator==(pgFlag<D, T> flag) const
        bool operator!=(pgFlag<D, T> flag) const
        D getValue() const
    */
    {
        pgFlag<u8, TestFlag> flag1;
        pgAssert(flag1.getValue() == 0);

        pgFlag<u8, TestFlag> flag2;

        flag2.setOn(FLAG_TEST8);
        pgAssert(flag2.getValue() != flag1.getValue());

        pgFlag<u8, TestFlag> flag3 = flag2;
        pgAssert(flag3.getValue() == flag2.getValue());

        pgAssert(flag2 == flag3);
        pgAssert(!(flag1 == flag2));

        pgAssert(flag1 != flag2);
        pgAssert(!(flag2 != flag3));
    }

    /*
        bool isOn(T flag) const
        bool isOff(T flag) const
        void setOn(T flag)
        void setOff(T flag)
        void set(T flag, bool is_on)
        void clear()
    */
    {
        pgFlag<u16, TestFlag> flag;

        flag.setOn(FLAG_TEST1);
        flag.setOn(FLAG_TEST5);
        pgAssert(flag.getValue() == 0x011);

        flag.setOff(FLAG_TEST2);
        flag.setOff(FLAG_TEST5);
        pgAssert(flag.getValue() == 0x001);

        flag.set(FLAG_TEST2, true);
        flag.set(FLAG_TEST9, true);
        pgAssert(flag.getValue() == 0x103);

        flag.set(FLAG_TEST1, false);
        flag.set(FLAG_TEST3, false);
        pgAssert(flag.getValue() == 0x102);

        pgAssert(flag.isOn(FLAG_TEST2));
        pgAssert(!flag.isOn(FLAG_TEST1));

        pgAssert(flag.isOff(FLAG_TEST3));
        pgAssert(!flag.isOff(FLAG_TEST9));

        flag.clear();
        pgAssert(flag.getValue() == 0);
    }
}
