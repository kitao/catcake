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
