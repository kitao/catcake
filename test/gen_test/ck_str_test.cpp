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


void ckStrTest()
{
    /*
        ckStr()
        ckStr(T ch)
        ckStr(const T* str)
        ckStr(const ckStr<T, N>& str)
        template<u8 N2> ckStr(const ckStr<T, N2>& str)
        const T* getString() const
        u8 getLength() const
    */
    {
        ckStr<wchar_t, 4> str1;
        ckAssert(isEqual(str1.getString(), L"") && str1.getLength() == 0);

        ckStr<wchar_t, 4> str2 = L't';
        ckAssert(isEqual(str2.getString(), L"t") && str2.getLength() == 1);

        ckStr<wchar_t, 4> str3 = L"test";
        ckAssert(isEqual(str3.getString(), L"test") && str3.getLength() == 4);

        ckStr<wchar_t, 4> str4 = str3;
        ckAssert(isEqual(str4.getString(), L"test") && str4.getLength() == 4);

        ckStr<wchar_t, 6> str5 = str4;
        ckAssert(isEqual(str5.getString(), L"test") && str4.getLength() == 4);

        ckStr<wchar_t, 2> str6 = L"test";
        ckAssert(isEqual(str6.getString(), L"te") && str6.getLength() == 2);

        ckStr<wchar_t, 2> str7 = str3;
        ckAssert(isEqual(str7.getString(), L"te") && str7.getLength() == 2);

        ckStr<char, 255> str8 = ( //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
        ckAssert(str8.getLength() == 255);
    }

    /*
        ckStr<T, N>& operator=(T ch)
        ckStr<T, N>& operator=(const T* str)
        ckStr<T, N>& operator=(const ckStr<T, N>& str)
        template<u8 N2> ckStr<T, N>& operator=(const ckStr<T, N2>& str)
    */
    {
        ckStr<wchar_t, 4> str1;

        str1 = L't';
        ckAssert(isEqual(str1.getString(), L"t") && str1.getLength() == 1);

        str1 = L"test";
        ckAssert(isEqual(str1.getString(), L"test") && str1.getLength() == 4);

        ckStr<wchar_t, 4> str2 = L"abc";

        str1 = str2;
        ckAssert(isEqual(str1.getString(), L"abc") && str1.getLength() == 3);

        ckStr<wchar_t, 6> str3 = L"123456";

        str1 = str3;
        ckAssert(isEqual(str1.getString(), L"1234") && str1.getLength() == 4);
    }

    /*
        bool operator==(T ch) const
        bool operator==(const T* str) const
        template<u8 N2> bool operator==(const ckStr<T, N2>& str) const
        bool operator!=(T ch) const
        bool operator!=(const T* str) const
        template<u8 N2> bool operator!=(const ckStr<T, N2>& str) const
    */
    {
        ckStr<wchar_t, 2> str1 = L't';
        ckStr<wchar_t, 4> str2 = L"test";
        ckStr<wchar_t, 6> str3 = str2;
        ckStr<wchar_t, 8> str4 = L"abcd";

        ckAssert(str1 == L't');
        ckAssert(!(str1 == L's'));

        ckAssert(str2 == L"test");
        ckAssert(!(str2 == L"tester"));

        ckAssert(str2 == str3);
        ckAssert(!(str2 == str4));

        ckAssert(str1 != L's');
        ckAssert(!(str1 != L't'));

        ckAssert(str2 != L"tester");
        ckAssert(!(str2 != L"test"));

        ckAssert(str2 != str4);
        ckAssert(!(str2 != str3));
    }

    /*
        T& operator[](u8 index)
    */
    {
        ckStr<wchar_t, 8> str = L"test";
        ckAssert(str[0] == 't' && str[1] == 'e' && str[2] == 's' && str[3] == 't');

        str[1] = 't';
        str[2] = 't';
        ckAssert(str == L"tttt");

        for (s32 i = 0; i < 10; i++)
        {
            if (i < 4)
            {
                str[i];
            }
            else
            {
                ckAssertThrow(str[i], ckStr<wchar_t, 8>::ExceptionOutOfRange);
            }
        }
    }

    /*
        ckStr<T, N> operator+(T ch) const
        ckStr<T, N> operator+(const T* str) const
        template<u8 N2> ckStr<T, N> operator+(const ckStr<T, N2>& str) const
        void operator+=(T ch)
        void operator+=(const T* str)
        template<u8 N2> void operator+=(const ckStr<T, N2>& str)
    */
    {
        ckStr<wchar_t, 8> str1 = L"123";

        ckStr<wchar_t, 6> str2 = str1 + L'4';
        ckAssert(str2 == L"1234" && str2.getLength() == 4);

        ckStr<wchar_t, 6> str3 = str1 + L"456";
        ckAssert(str3 == L"123456" && str3.getLength() == 6);

        ckStr<wchar_t, 12> str4 = str1 + str3;
        ckAssert(str4 == L"12312345" && str4.getLength() == 8);

        str1 += L'4';
        ckAssert(str1 == L"1234" && str1.getLength() == 4);

        str1 += L"56";
        ckAssert(str1 == L"123456" && str1.getLength() == 6);

        str4 += str1;
        ckAssert(str4 == L"123123451234" && str4.getLength() == 12);
    }

    /*
        u32 operator%(u32 n) const
    */
    {
        ckStr<char, 3> str1 = "123";
        ckStr<char, 3> str2 = "45";

        ckAssert(str1 % 7 == ('1' + '2' + '3') % 7);
        ckAssert(str2 % 11 == ('4' + '5') % 11);
    }

    /*
        ckStr<T, N> getSubStr(u8 index, u8 length = 0) const
    */
    {
        ckStr<wchar_t, 6> str = L"123456";

        ckAssert(str.getSubStr(0, 3) == L"123");
        ckAssert(str.getSubStr(0, 10) == L"123456");
        ckAssert(str.getSubStr(3, 2) == L"45");
        ckAssert(str.getSubStr(3, 10) == L"456");
        ckAssert(str.getSubStr(2) == L"3456");
        ckAssert(str.getSubStr(6, 3) == L"");
    }
}
