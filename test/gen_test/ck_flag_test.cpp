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

        flag2.setOn(FLAG_TEST8);
        ckAssert(flag2.getValue() != flag1.getValue());

        ckFlag<u8, TestFlag> flag3 = flag2;
        ckAssert(flag3.getValue() == flag2.getValue());

        ckAssert(flag2 == flag3);
        ckAssert(!(flag1 == flag2));

        ckAssert(flag1 != flag2);
        ckAssert(!(flag2 != flag3));
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
        ckFlag<u16, TestFlag> flag;

        flag.setOn(FLAG_TEST1);
        flag.setOn(FLAG_TEST5);
        ckAssert(flag.getValue() == 0x011);

        flag.setOff(FLAG_TEST2);
        flag.setOff(FLAG_TEST5);
        ckAssert(flag.getValue() == 0x001);

        flag.set(FLAG_TEST2, true);
        flag.set(FLAG_TEST9, true);
        ckAssert(flag.getValue() == 0x103);

        flag.set(FLAG_TEST1, false);
        flag.set(FLAG_TEST3, false);
        ckAssert(flag.getValue() == 0x102);

        ckAssert(flag.isOn(FLAG_TEST2));
        ckAssert(!flag.isOn(FLAG_TEST1));

        ckAssert(flag.isOff(FLAG_TEST3));
        ckAssert(!flag.isOff(FLAG_TEST9));

        flag.clear();
        ckAssert(flag.getValue() == 0);
    }
}
