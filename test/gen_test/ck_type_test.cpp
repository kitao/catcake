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


enum TestType
{
    TYPE_TEST1, TYPE_TEST2, TYPE_TEST3, TYPE_TEST4
};


void ckTypeTest()
{
    /*
        ckType()
        ckType(T type)
        ckType<D, T>& operator=(T type)
        T getType() const
        D getValue() const
    */
    {
        ckType<u8, TestType> type1;
        ckAssert(type1.getType() == TYPE_TEST1);
        ckAssert(type1.getValue() == 0);

        ckType<u8, TestType> type2;

        type2 = TYPE_TEST2;
        ckAssert(type2.getType() == TYPE_TEST2);
        ckAssert(type2.getValue() == 1);

        ckType<u8, TestType> type3 = type2;
        ckAssert(type3.getType() == type2.getType());
        ckAssert(type3.getValue() == type2.getValue());

        ckType<u8, TestType> type4 = TYPE_TEST3;
        ckAssert(type4.getType() == TYPE_TEST3);
        ckAssert(type4.getValue() == 2);

        type1 = type4;
        ckAssert(type1.getType() == type4.getType());
        ckAssert(type1.getValue() == type4.getValue());
    }

    /*
        bool operator==(ckType<D, T> type) const
        bool operator==(T type) const
        friend bool operator==(T type1, ckType<D, T> type2)
        bool operator!=(ckType<D, T> type) const
        bool operator!=(T type) const
        friend bool operator!=(T type1, ckType<D, T> type2)
    */
    {
        ckType<u16, TestType> type1 = TYPE_TEST1;
        ckType<u16, TestType> type2 = TYPE_TEST1;
        ckType<u16, TestType> type3 = TYPE_TEST2;

        ckAssert(type1 == type2);
        ckAssert(!(type1 == type3));

        ckAssert(type3 == TYPE_TEST2);
        ckAssert(!(type3 == TYPE_TEST3));

        ckAssert(TYPE_TEST2 == type3);
        ckAssert(!(TYPE_TEST2 == type1));

        ckAssert(type1 != type3);
        ckAssert(!(type1 != type2));

        ckAssert(type3 != TYPE_TEST3);
        ckAssert(!(type3 != TYPE_TEST2));

        ckAssert(TYPE_TEST2 != type1);
        ckAssert(!(TYPE_TEST2 != type3));
    }

    /*
        Test codes for ckType<D, bool>

        ckType()
        ckType(T type)
        ckType<D, T>& operator=(T type)
        T getType() const
        D getValue() const
    */
    {
        ckType<u8, bool> type1;
        ckAssert(type1.getType() == false);
        ckAssert(type1.getValue() == 0);

        ckType<u8, bool> type2;

        type2 = true;
        ckAssert(type2.getType() == true);
        ckAssert(type2.getValue() == 1);

        ckType<u8, bool> type3 = type2;
        ckAssert(type3.getType() == type2.getType());
        ckAssert(type3.getValue() == type2.getValue());

        ckType<u8, bool> type4 = true;
        ckAssert(type4.getType() == true);
        ckAssert(type4.getValue() == 1);

        type1 = type4;
        ckAssert(type1.getType() == type4.getType());
        ckAssert(type1.getValue() == type4.getValue());
    }

    /*
        Test codes for ckType<D, bool>

        bool operator==(ckType<D, T> type) const
        bool operator==(T type) const
        friend bool operator==(T type1, ckType<D, T> type2)
        bool operator!=(ckType<D, T> type) const
        bool operator!=(T type) const
        friend bool operator!=(T type1, ckType<D, T> type2)
    */
    {
        ckType<u16, bool> type1 = false;
        ckType<u16, bool> type2 = false;
        ckType<u16, bool> type3 = true;

        ckAssert(type1 == type2);
        ckAssert(!(type1 == type3));

        ckAssert(type3 == true);
        ckAssert(!(type3 == false));

        ckAssert(true == type3);
        ckAssert(!(true == type1));

        ckAssert(type1 != type3);
        ckAssert(!(type1 != type2));

        ckAssert(type3 != false);
        ckAssert(!(type3 != true));

        ckAssert(true != type1);
        ckAssert(!(true != type3));
    }
}
