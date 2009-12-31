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


void ckMapTest()
{
    /*
        ckMap()
        ~ckMap()
        void init(u16 hash_size)
        u16 getHashSize() const
        u16 getDataNum() const
        void add(K key, const D& data)
    */
    {
        ckMap<s8, u32> map;
        ckAssert(map.getHashSize() == 0 && map.getDataNum() == 0);

        ckAssertThrow(map.init(0), ckMap<s8, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.add(0, 123), ckMap<s8, u32>::ExceptionNotInitialized);

        map.init(5);
        ckAssert(map.getHashSize() == 5 && map.getDataNum() == 0);

        map.init(3);
        ckAssert(map.getHashSize() == 3 && map.getDataNum() == 0);

        map.add(1, 345);
        map.add(2, 456);
        map.add(-3, 789);
        ckAssert(map.getHashSize() == 3 && map.getDataNum() == 3);

        ckAssertThrow(map.add(-3, 123), ckMap<s8, u32>::ExceptionSameKeyExists);
    }

    /*
        D* get(K key)
        D* getN(K key)
        void remove(K key)
        void clear()
    */
    {
        ckMap<ckID, u32> map;

        map.clear();

        ckAssertThrow(map.get(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.getN(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.remove(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);

        ckID id[20];

        for (s32 i = 0; i < 20; i++)
        {
            id[i] = ckID::genID();
        }

        map.init(5);

        for (s32 i = 0; i < 10; i++)
        {
            map.add(id[i], 100 + i);
            ckAssert(map.getHashSize() == 5 && map.getDataNum() == i + 1);
        }

        for (u32 i = 1; i < 20; i++)
        {
            for (u32 j = 0; j <= i; j++)
            {
                if (j < 10)
                {
                    ckAssert(*map.get(id[j]) == 100 + j);
                    ckAssert(*map.getN(id[j]) == 100 + j);
                }
                else
                {
                    ckAssertThrow(map.get(id[j]), ckMap<ckID, u32>::ExceptionNotFound);
                    ckAssert(!map.getN(id[j]));
                }
            }
        }

        for (s32 i = 7; i < 20; i++)
        {
            if (i < 10)
            {
                map.remove(id[i]);
            }
            else
            {
                ckAssertThrow(map.remove(id[i]), ckMap<ckID, u32>::ExceptionNotFound);
            }
        }

        ckAssert(map.getHashSize() == 5 && map.getDataNum() == 7);

        for (s32 i = 0; i < 20; i += 2)
        {
            if (i < 7)
            {
                map.remove(id[i]);
            }
            else
            {
                ckAssertThrow(map.remove(id[i]), ckMap<ckID, u32>::ExceptionNotFound);
            }
        }

        ckAssert(map.getHashSize() == 5 && map.getDataNum() == 3);

        for (u32 i = 0; i < 20; i++)
        {
            if (i < 7 && i % 2 != 0)
            {
                ckAssert(*map.get(id[i]) == 100 + i);
                ckAssert(*map.getN(id[i]) == 100 + i);
            }
            else
            {
                ckAssertThrow(map.get(id[i]), ckMap<ckID, u32>::ExceptionNotFound);
                ckAssert(!map.getN(id[i]));
            }
        }

        map.clear();
        ckAssert(map.getHashSize() == 5 && map.getDataNum() == 0);

        for (s32 i = 0; i < 20; i++)
        {
            ckAssertThrow(map.get(id[i]), ckMap<ckID, u32>::ExceptionNotFound);
            ckAssert(!map.getN(id[i]));
        }

        map.clear();
        ckAssert(map.getHashSize() == 5 && map.getDataNum() == 0);
    }

    /*
        const K* getFirstKeyN() const
        const K* getLastKeyN() const
        const K* getPrevKeyN(K key)
        const K* getNextKeyN(K key)
        void moveFirst(K key)
        void moveLast(K key)
        void moveBefore(K key, K next_key)
        void moveAfter(K key, K prev_key)
    */
    {
        ckMap<ckID, u32> map;

        ckAssertThrow(map.getFirstKeyN(), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.getLastKeyN(), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.getPrevKeyN(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.getNextKeyN(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.moveFirst(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.moveLast(ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.moveBefore(ckID::ZERO, ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);
        ckAssertThrow(map.moveAfter(ckID::ZERO, ckID::ZERO), ckMap<ckID, u32>::ExceptionNotInitialized);

        map.init(2);
        ckAssert(!map.getFirstKeyN() && !map.getLastKeyN());

        ckID::setCurIDForSystem(1);

        ckID id1 = ckID::genID();
        ckID id2 = ckID::genID();
        ckID id3 = ckID::genID();

        map.add(id1, 123);
        map.add(id2, 456);
        map.add(id3, 789);
        ckAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id3);
        ckAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id2);
        ckAssert(*map.getPrevKeyN(id2) == id1 && *map.getNextKeyN(id2) == id3);
        ckAssert(*map.getPrevKeyN(id3) == id2 && !map.getNextKeyN(id3));

        map.remove(id2);
        ckAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id3);
        ckAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id3);
        ckAssert(*map.getPrevKeyN(id3) == id1 && !map.getNextKeyN(id3));

        map.remove(id1);
        ckAssert(*map.getFirstKeyN() == id3 && *map.getLastKeyN() == id3);
        ckAssert(!map.getPrevKeyN(id3) && !map.getNextKeyN(id3));

        ckAssertThrow(map.getPrevKeyN(ckID::ZERO), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.getPrevKeyN(ckID::genID()), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.getNextKeyN(ckID::ZERO), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.getNextKeyN(ckID::genID()), ckMap<ckID, u32>::ExceptionInvalidArgument);

        ckAssertThrow(map.moveFirst(id1), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.moveLast(id1), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.moveBefore(id1, id3), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.moveBefore(id3, id1), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.moveAfter(id1, id3), ckMap<ckID, u32>::ExceptionInvalidArgument);
        ckAssertThrow(map.moveAfter(id3, id1), ckMap<ckID, u32>::ExceptionInvalidArgument);

        map.add(id1, 123);
        map.add(id2, 456);
        ckAssert(*map.getFirstKeyN() == id3 && *map.getLastKeyN() == id2);
        ckAssert(!map.getPrevKeyN(id3) && *map.getNextKeyN(id3) == id1);
        ckAssert(*map.getPrevKeyN(id1) == id3 && *map.getNextKeyN(id1) == id2);
        ckAssert(*map.getPrevKeyN(id2) == id1 && !map.getNextKeyN(id2));

        map.moveFirst(id1);
        ckAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id2);
        ckAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id3);
        ckAssert(*map.getPrevKeyN(id3) == id1 && *map.getNextKeyN(id3) == id2);
        ckAssert(*map.getPrevKeyN(id2) == id3 && !map.getNextKeyN(id2));

        map.moveLast(id3);
        ckAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id3);
        ckAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id2);
        ckAssert(*map.getPrevKeyN(id2) == id1 && *map.getNextKeyN(id2) == id3);
        ckAssert(*map.getPrevKeyN(id3) == id2 && !map.getNextKeyN(id3));

        map.moveBefore(id1, id3);
        ckAssert(*map.getFirstKeyN() == id2 && *map.getLastKeyN() == id3);
        ckAssert(!map.getPrevKeyN(id2) && *map.getNextKeyN(id2) == id1);
        ckAssert(*map.getPrevKeyN(id1) == id2 && *map.getNextKeyN(id1) == id3);
        ckAssert(*map.getPrevKeyN(id3) == id1 && !map.getNextKeyN(id3));

        map.moveAfter(id3, id2);
        ckAssert(*map.getFirstKeyN() == id2 && *map.getLastKeyN() == id1);
        ckAssert(!map.getPrevKeyN(id2) && *map.getNextKeyN(id2) == id3);
        ckAssert(*map.getPrevKeyN(id3) == id2 && *map.getNextKeyN(id3) == id1);
        ckAssert(*map.getPrevKeyN(id1) == id3 && !map.getNextKeyN(id1));
    }
}
