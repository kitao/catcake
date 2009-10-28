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


void ckFlagTest()
{
    /*
        ckFlag()
        bool operator==(ckFlag<D, T> flag) const
        bool operator!=(ckFlag<D, T> flag) const
        D getValue() const
    */
    {
        ckFlag<u8, TestFlag> flag1;
        ckAssert(flag1.getValue() == 0);

        ckFlag<u8, TestFlag> flag2;

        flag2.setEnabled(FLAG_TEST8);
        ckAssert(flag2.getValue() != flag1.getValue());

        ckFlag<u8, TestFlag> flag3 = flag2;
        ckAssert(flag3.getValue() == flag2.getValue());

        ckAssert(flag2 == flag3);
        ckAssert(!(flag1 == flag2));

        ckAssert(flag1 != flag2);
        ckAssert(!(flag2 != flag3));
    }

    /*
        bool isEnabled(T flag) const
        bool isDisabled(T flag) const
        void setEnabled(T flag)
        void setDisabled(T flag)
        void set(T flag, bool is_enabled)
        void clear()
    */
    {
        ckFlag<u16, TestFlag> flag;

        flag.setEnabled(FLAG_TEST1);
        flag.setEnabled(FLAG_TEST5);
        ckAssert(flag.getValue() == 0x011);

        flag.setDisabled(FLAG_TEST2);
        flag.setDisabled(FLAG_TEST5);
        ckAssert(flag.getValue() == 0x001);

        flag.set(FLAG_TEST2, true);
        flag.set(FLAG_TEST9, true);
        ckAssert(flag.getValue() == 0x103);

        flag.set(FLAG_TEST1, false);
        flag.set(FLAG_TEST3, false);
        ckAssert(flag.getValue() == 0x102);

        ckAssert(flag.isEnabled(FLAG_TEST2));
        ckAssert(!flag.isEnabled(FLAG_TEST1));

        ckAssert(flag.isDisabled(FLAG_TEST3));
        ckAssert(!flag.isDisabled(FLAG_TEST9));

        flag.clear();
        ckAssert(flag.getValue() == 0);
    }
}
