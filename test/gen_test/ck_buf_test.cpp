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


void ckBufTest()
{
    /*
        ckBuf()
        ~ckBuf()
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

        ckBuf<BufTestClass> buf;
        ckAssert(s_ctor_count == 0 && s_dtor_count == 0);

        ckAssertThrow(buf.init(0), ckBuf<BufTestClass>::ExceptionInvalidArgument);
        ckAssertThrow(buf.newElement(), ckBuf<BufTestClass>::ExceptionNotInitialized);
        ckAssertThrow(buf.deleteElement(NULL), ckBuf<BufTestClass>::ExceptionNotInitialized);
        ckAssertThrow(buf.clear(), ckBuf<BufTestClass>::ExceptionNotInitialized);

        buf.init(5);
        ckAssert(s_ctor_count == 0 && s_dtor_count == 0);
        ckAssert(buf.getSize() == 5 && buf.getUsedSize() == 0 && buf.getFreeSize() == 5);

        buf.newElement();
        buf.newElement();
        ckAssert(s_ctor_count == 2 && s_dtor_count == 0);
        ckAssert(buf.getSize() == 5 && buf.getUsedSize() == 2 && buf.getFreeSize() == 3);

        buf.init(10);
        ckAssert(s_ctor_count == 2 && s_dtor_count == 2);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 0 && buf.getFreeSize() == 10);

        BufTestClass* test_data[10];

        for (s32 i = 0; i < 20; i++)
        {
            if (i < 10)
            {
                test_data[i] = buf.newElement();
            }
            else
            {
                ckAssertThrow(buf.newElement(), ckBuf<BufTestClass>::ExceptionOutOfBuffer);
            }
        }

        ckAssert(s_ctor_count == 12 && s_dtor_count == 2);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 10 && buf.getFreeSize() == 0);

        for (s32 i = 0; i < 10; i += 2)
        {
            buf.deleteElement(test_data[i]);
        }

        ckAssert(s_ctor_count == 12 && s_dtor_count == 7);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        ckAssertThrow(buf.deleteElement(test_data[1] - 2), ckBuf<BufTestClass>::ExceptionInvalidArgument);

        ckAssert(s_ctor_count == 12 && s_dtor_count == 7);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        ckAssertThrow(buf.deleteElement(test_data[9] + 1), ckBuf<BufTestClass>::ExceptionInvalidArgument);

        ckAssert(s_ctor_count == 12 && s_dtor_count == 7);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        ckAssertThrow(buf.deleteElement(test_data[2]), ckBuf<BufTestClass>::ExceptionInvalidArgument);

        ckAssert(s_ctor_count == 12 && s_dtor_count == 7);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 5 && buf.getFreeSize() == 5);

        for (s32 i = 0; i < 5; i++)
        {
            test_data[i] = buf.newElement();
        }

        ckAssert(s_ctor_count == 17 && s_dtor_count == 7);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 10 && buf.getFreeSize() == 0);

        buf.clear();
        ckAssert(s_ctor_count == 17 && s_dtor_count == 17);
        ckAssert(buf.getSize() == 10 && buf.getUsedSize() == 0 && buf.getFreeSize() == 10);
    }

    /*
        T* getFirstN() const
        T* getLastN() const
        T* getPrevN(const T* elem) const
        T* getNextN(const T* elem) const
    */
    {
        ckBuf<BufTestClass> buf;

        ckAssertThrow(buf.getFirstN(), ckBuf<BufTestClass>::ExceptionNotInitialized);
        ckAssertThrow(buf.getLastN(), ckBuf<BufTestClass>::ExceptionNotInitialized);
        ckAssertThrow(buf.getPrevN(NULL), ckBuf<BufTestClass>::ExceptionNotInitialized);
        ckAssertThrow(buf.getNextN(NULL), ckBuf<BufTestClass>::ExceptionNotInitialized);

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

        ckAssert(buf.getFirstN() == test_data[1]);
        ckAssert(buf.getLastN() == test_data[8]);

        ckAssert(!buf.getPrevN(test_data[1]));
        ckAssert(buf.getNextN(test_data[1]) == test_data[5]);

        ckAssert(buf.getPrevN(test_data[5]) == test_data[1]);
        ckAssert(buf.getNextN(test_data[5]) == test_data[8]);

        ckAssert(buf.getPrevN(test_data[8]) == test_data[5]);
        ckAssert(!buf.getNextN(test_data[8]));

        ckAssertThrow(buf.getPrevN(test_data[1] - 2), ckBuf<BufTestClass>::ExceptionInvalidArgument);
        ckAssertThrow(buf.getPrevN(test_data[8] + 2), ckBuf<BufTestClass>::ExceptionInvalidArgument);
        ckAssertThrow(buf.getPrevN(test_data[2]), ckBuf<BufTestClass>::ExceptionInvalidArgument);
        ckAssertThrow(buf.getNextN(test_data[1] - 2), ckBuf<BufTestClass>::ExceptionInvalidArgument);
        ckAssertThrow(buf.getNextN(test_data[8] + 2), ckBuf<BufTestClass>::ExceptionInvalidArgument);
        ckAssertThrow(buf.getNextN(test_data[9]), ckBuf<BufTestClass>::ExceptionInvalidArgument);

        buf.clear();

        ckAssert(!buf.getFirstN());
        ckAssert(!buf.getLastN());
    }
}
