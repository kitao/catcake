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


void pgStrTest()
{
    /*
        pgStr()
        pgStr(T ch)
        pgStr(const T* str)
        pgStr(const pgStr<T, N>& str)
        template<u8 N2> pgStr(const pgStr<T, N2>& str)
        const T* getString() const
        u8 getLength() const
    */
    {
        pgStr<wchar_t, 4> str1;
        pgAssert(isEqual(str1.getString(), L"") && str1.getLength() == 0);

        pgStr<wchar_t, 4> str2 = L't';
        pgAssert(isEqual(str2.getString(), L"t") && str2.getLength() == 1);

        pgStr<wchar_t, 4> str3 = L"test";
        pgAssert(isEqual(str3.getString(), L"test") && str3.getLength() == 4);

        pgStr<wchar_t, 4> str4 = str3;
        pgAssert(isEqual(str4.getString(), L"test") && str4.getLength() == 4);

        pgStr<wchar_t, 6> str5 = str4;
        pgAssert(isEqual(str5.getString(), L"test") && str4.getLength() == 4);

        pgStr<wchar_t, 2> str6 = L"test";
        pgAssert(isEqual(str6.getString(), L"te") && str6.getLength() == 2);

        pgStr<wchar_t, 2> str7 = str3;
        pgAssert(isEqual(str7.getString(), L"te") && str7.getLength() == 2);

        pgStr<char, 255> str8 = ( //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" //
            "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF");
        pgAssert(str8.getLength() == 255);
    }

    /*
        pgStr<T, N>& operator=(T ch)
        pgStr<T, N>& operator=(const T* str)
        pgStr<T, N>& operator=(const pgStr<T, N>& str)
        template<u8 N2> pgStr<T, N>& operator=(const pgStr<T, N2>& str)
    */
    {
        pgStr<wchar_t, 4> str1;

        str1 = L't';
        pgAssert(isEqual(str1.getString(), L"t") && str1.getLength() == 1);

        str1 = L"test";
        pgAssert(isEqual(str1.getString(), L"test") && str1.getLength() == 4);

        pgStr<wchar_t, 4> str2 = L"abc";

        str1 = str2;
        pgAssert(isEqual(str1.getString(), L"abc") && str1.getLength() == 3);

        pgStr<wchar_t, 6> str3 = L"123456";

        str1 = str3;
        pgAssert(isEqual(str1.getString(), L"1234") && str1.getLength() == 4);
    }

    /*
        bool operator==(T ch) const
        bool operator==(const T* str) const
        template<u8 N2> bool operator==(const pgStr<T, N2>& str) const
        bool operator!=(T ch) const
        bool operator!=(const T* str) const
        template<u8 N2> bool operator!=(const pgStr<T, N2>& str) const
    */
    {
        pgStr<wchar_t, 2> str1 = L't';
        pgStr<wchar_t, 4> str2 = L"test";
        pgStr<wchar_t, 6> str3 = str2;
        pgStr<wchar_t, 8> str4 = L"abcd";

        pgAssert(str1 == L't');
        pgAssert(!(str1 == L's'));

        pgAssert(str2 == L"test");
        pgAssert(!(str2 == L"tester"));

        pgAssert(str2 == str3);
        pgAssert(!(str2 == str4));

        pgAssert(str1 != L's');
        pgAssert(!(str1 != L't'));

        pgAssert(str2 != L"tester");
        pgAssert(!(str2 != L"test"));

        pgAssert(str2 != str4);
        pgAssert(!(str2 != str3));
    }

    /*
        T& operator[](u8 index)
    */
    {
        pgStr<wchar_t, 8> str = L"test";
        pgAssert(str[0] == 't' && str[1] == 'e' && str[2] == 's' && str[3] == 't');

        str[1] = 't';
        str[2] = 't';
        pgAssert(str == L"tttt");

        for (s32 i = 0; i < 10; i++)
        {
            if (i < 4)
            {
                str[i];
            }
            else
            {
                pgAssertThrow(str[i], pgStr<wchar_t, 8>::ExceptionOutOfRange);
            }
        }
    }

    /*
        pgStr<T, N> operator+(T ch) const
        pgStr<T, N> operator+(const T* str) const
        template<u8 N2> pgStr<T, N> operator+(const pgStr<T, N2>& str) const
        void operator+=(T ch)
        void operator+=(const T* str)
        template<u8 N2> void operator+=(const pgStr<T, N2>& str)
    */
    {
        pgStr<wchar_t, 8> str1 = L"123";

        pgStr<wchar_t, 6> str2 = str1 + L'4';
        pgAssert(str2 == L"1234" && str2.getLength() == 4);

        pgStr<wchar_t, 6> str3 = str1 + L"456";
        pgAssert(str3 == L"123456" && str3.getLength() == 6);

        pgStr<wchar_t, 12> str4 = str1 + str3;
        pgAssert(str4 == L"12312345" && str4.getLength() == 8);

        str1 += L'4';
        pgAssert(str1 == L"1234" && str1.getLength() == 4);

        str1 += L"56";
        pgAssert(str1 == L"123456" && str1.getLength() == 6);

        str4 += str1;
        pgAssert(str4 == L"123123451234" && str4.getLength() == 12);
    }

    /*
        u32 operator%(u32 n) const
    */
    {
        pgStr<char, 3> str1 = "123";
        pgStr<char, 3> str2 = "45";

        pgAssert(str1 % 7 == ('1' + '2' + '3') % 7);
        pgAssert(str2 % 11 == ('4' + '5') % 11);
    }

    /*
        pgStr<T, N> getSubStr(u8 index, u8 length = 0) const
    */
    {
        pgStr<wchar_t, 6> str = L"123456";

        pgAssert(str.getSubStr(0, 3) == L"123");
        pgAssert(str.getSubStr(0, 10) == L"123456");
        pgAssert(str.getSubStr(3, 2) == L"45");
        pgAssert(str.getSubStr(3, 10) == L"456");
        pgAssert(str.getSubStr(2) == L"3456");
        pgAssert(str.getSubStr(6, 3) == L"");
    }
}
