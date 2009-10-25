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


static u32 s_ctor_count;
static u32 s_dtor_count;


class BufTestClass
{
public:
    BufTestClass()
    {
        s_ctor_count++;
    }

    BufTestClass(u8 dummy)
    {
        s_ctor_count++;
    }

    ~BufTestClass()
    {
        s_dtor_count++;
    }

private:
    u8 m_buf[5];
};


void pgBufTest()
{
    /*
        pgBuf()
        ~pgBuf()
        void init(u16 size)
        u16 getSize() const
        u16 getUsedSize() const
        u16 getFreeSize() const
        T* newElement()
        void deleteElement(T* elem)
        void clear()
    */
    {
        s_ctor_count = 0;
        s_dtor_count = 0;

        pgBuf<BufTestClass> buf;
        pgAssert(s_ctor_count == 0 && s_dtor_count == 0);

        pgAssertThrow(buf.init(0), pgBuf<BufTestClass>::ExceptionInvalidArgument);
        pgAssertThrow(buf.newElement(), pgBuf<BufTestClass>::ExceptionNotInitialized);
        pgAssertThrow(buf.deleteElement(NULL), pgBuf<BufTestClass>::ExceptionNotInitialized);
        pgAssertThrow(buf.clear(), pgBuf<BufTestClass>::ExceptionNotInitialized);

        buf.init(5);
        pgAssert(s_ctor_count == 0 && s_dtor_count == 0);
        pgAssert(buf.getSize() == 5 && buf.getUsedSize() == 0 && buf.getFreeSize() == 5);

        buf.newElement();
        buf.newElement();
        pgAssert(s_ctor_count == 2 && s_dtor_count == 0);
        pgAssert(buf.getSize() == 5 && buf.getUsedSize() == 2 && buf.getFreeSize() == 3);

        buf.init(10);
        pgAssert(s_ctor_count == 2 && s_dtor_count == 2);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 0 && buf.getFreeSize() == 10);

        BufTestClass* test_data[10];

        for (s32 i = 0; i < 20; i++)
        {
            if (i < 10)
            {
                test_data[i] = buf.newElement();
            }
            else
            {
                pgAssertThrow(buf.newElement(), pgBuf<BufTestClass>::ExceptionOutOfBuffer);
            }
        }

        pgAssert(s_ctor_count == 12 && s_dtor_count == 2);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 10 && buf.getFreeSize() == 0);

        for (s32 i = 0; i < 10; i += 2)
        {
            buf.deleteElement(test_data[i]);
        }

        pgAssert(s_ctor_count == 12 && s_dtor_count == 7);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        pgAssertThrow(buf.deleteElement(test_data[1] - 2), pgBuf<BufTestClass>::ExceptionInvalidArgument);

        pgAssert(s_ctor_count == 12 && s_dtor_count == 7);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        pgAssertThrow(buf.deleteElement(test_data[9] + 1), pgBuf<BufTestClass>::ExceptionInvalidArgument);

        pgAssert(s_ctor_count == 12 && s_dtor_count == 7);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        pgAssertThrow(buf.deleteElement(test_data[2]), pgBuf<BufTestClass>::ExceptionInvalidArgument);

        pgAssert(s_ctor_count == 12 && s_dtor_count == 7);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        for (s32 i = 0; i < 5; i++)
        {
            test_data[i] = buf.newElement();
        }

        pgAssert(s_ctor_count == 17 && s_dtor_count == 7);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 10 && buf.getFreeSize() == 0);

        buf.clear();
        pgAssert(s_ctor_count == 17 && s_dtor_count == 17);
        pgAssert(buf.getSize() == 10 && buf.getUsedSize() == 0 && buf.getFreeSize() == 10);
    }

    /*
        T* getFirstN() const
        T* getLastN() const
        T* getPrevN(const T* elem) const
        T* getNextN(const T* elem) const
    */
    {
        pgBuf<BufTestClass> buf;

        pgAssertThrow(buf.getFirstN(), pgBuf<BufTestClass>::ExceptionNotInitialized);
        pgAssertThrow(buf.getLastN(), pgBuf<BufTestClass>::ExceptionNotInitialized);
        pgAssertThrow(buf.getPrevN(NULL), pgBuf<BufTestClass>::ExceptionNotInitialized);
        pgAssertThrow(buf.getNextN(NULL), pgBuf<BufTestClass>::ExceptionNotInitialized);

        buf.init(10);

        BufTestClass* test_data[10];

        for (s32 i = 0; i < 10; i++)
        {
            test_data[i] = buf.newElement();
        }

        buf.deleteElement(test_data[0]);
        buf.deleteElement(test_data[2]);
        buf.deleteElement(test_data[3]);
        buf.deleteElement(test_data[4]);
        buf.deleteElement(test_data[6]);
        buf.deleteElement(test_data[7]);
        buf.deleteElement(test_data[9]);

        pgAssert(buf.getFirstN() == test_data[1]);
        pgAssert(buf.getLastN() == test_data[8]);

        pgAssert(!buf.getPrevN(test_data[1]));
        pgAssert(buf.getNextN(test_data[1]) == test_data[5]);

        pgAssert(buf.getPrevN(test_data[5]) == test_data[1]);
        pgAssert(buf.getNextN(test_data[5]) == test_data[8]);

        pgAssert(buf.getPrevN(test_data[8]) == test_data[5]);
        pgAssert(!buf.getNextN(test_data[8]));

        pgAssertThrow(buf.getPrevN(test_data[1] - 2), pgBuf<BufTestClass>::ExceptionInvalidArgument);
        pgAssertThrow(buf.getPrevN(test_data[8] + 2), pgBuf<BufTestClass>::ExceptionInvalidArgument);
        pgAssertThrow(buf.getPrevN(test_data[2]), pgBuf<BufTestClass>::ExceptionInvalidArgument);
        pgAssertThrow(buf.getNextN(test_data[1] - 2), pgBuf<BufTestClass>::ExceptionInvalidArgument);
        pgAssertThrow(buf.getNextN(test_data[8] + 2), pgBuf<BufTestClass>::ExceptionInvalidArgument);
        pgAssertThrow(buf.getNextN(test_data[9]), pgBuf<BufTestClass>::ExceptionInvalidArgument);

        buf.clear();

        pgAssert(!buf.getFirstN());
        pgAssert(!buf.getLastN());
    }
}
