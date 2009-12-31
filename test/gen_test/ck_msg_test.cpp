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


void ckMsgTest()
{
    /*
        ckMsg()
        template<class T> T getParam(u8 index)
        template<class T> void setParam(u8 index, T param)
    */
    {
        ckMsg<2> msg1;
        ckAssertThrow(msg1.getParam<u8>(0), ckMsg<2>::ExceptionInvalidCall);
        ckAssertThrow(msg1.getParam<s16>(1), ckMsg<2>::ExceptionInvalidCall);
        ckAssertThrow(msg1.getParam<r32>(2), ckMsg<2>::ExceptionInvalidArgument);

        msg1.setParam<s8>(0, -123);
        ckAssert(msg1.getParam<s8>(0) == -123);
        ckAssertThrow(msg1.getParam<s16>(0), ckMsg<2>::ExceptionInvalidType);

        msg1.setParam<r32>(1, 1.234f);
        ckAssert(isEqual(msg1.getParam<r32>(1), 1.234f));
        ckAssertThrow(msg1.getParam<r64>(0), ckMsg<2>::ExceptionInvalidType);

        ckMsg<2> msg2 = msg1;
        ckAssert(msg2.getParam<s8>(0) == -123);
        ckAssert(isEqual(msg2.getParam<r32>(1), 1.234f));
        ckAssertThrow(msg2.getParam<u64>(2), ckMsg<2>::ExceptionInvalidArgument);

        msg2.setParam<s16>(0, -12345);
        ckAssert(msg2.getParam<s16>(0) == -12345);
        ckAssertThrow(msg2.getParam<u8>(0), ckMsg<2>::ExceptionInvalidType);

        msg2.setParam<u32>(1, 54321);
        ckAssert(msg2.getParam<u32>(1) == 54321);
        ckAssertThrow(msg2.getParam<s16>(1), ckMsg<2>::ExceptionInvalidType);

        msg1 = msg2;
        ckAssert(msg2.getParam<s16>(0) == -12345);
        ckAssert(msg2.getParam<u32>(1) == 54321);
    }
}
