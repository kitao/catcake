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


void pgMapTest()
{
    /*
        pgMap()
        ~pgMap()
        void init(u16 hash_size)
        u16 getHashSize() const
        u16 getDataNum() const
        void add(K key, const D& data)
    */
    {
        pgMap<s8, u32> map;
        pgAssert(map.getHashSize() == 0 && map.getDataNum() == 0);

        pgAssertThrow(map.init(0), pgMap<s8, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.add(0, 123), pgMap<s8, u32>::ExceptionNotInitialized);

        map.init(5);
        pgAssert(map.getHashSize() == 5 && map.getDataNum() == 0);

        map.init(3);
        pgAssert(map.getHashSize() == 3 && map.getDataNum() == 0);

        map.add(1, 345);
        map.add(2, 456);
        map.add(-3, 789);
        pgAssert(map.getHashSize() == 3 && map.getDataNum() == 3);

        pgAssertThrow(map.add(-3, 123), pgMap<s8, u32>::ExceptionSameKeyExists);
    }

    /*
        D* get(K key)
        D* getN(K key)
        void remove(K key)
        void clear()
    */
    {
        pgMap<pgID, u32> map;

        map.clear();

        pgAssertThrow(map.get(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.getN(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.remove(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);

        pgID id[20];

        for (s32 i = 0; i < 20; i++)
        {
            id[i] = pgID::genID();
        }

        map.init(5);

        for (s32 i = 0; i < 10; i++)
        {
            map.add(id[i], 100 + i);
            pgAssert(map.getHashSize() == 5 && map.getDataNum() == i + 1);
        }

        for (u32 i = 1; i < 20; i++)
        {
            for (u32 j = 0; j <= i; j++)
            {
                if (j < 10)
                {
                    pgAssert(*map.get(id[j]) == 100 + j);
                    pgAssert(*map.getN(id[j]) == 100 + j);
                }
                else
                {
                    pgAssertThrow(map.get(id[j]), pgMap<pgID, u32>::ExceptionNotFound);
                    pgAssert(!map.getN(id[j]));
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
                pgAssertThrow(map.remove(id[i]), pgMap<pgID, u32>::ExceptionNotFound);
            }
        }

        pgAssert(map.getHashSize() == 5 && map.getDataNum() == 7);

        for (s32 i = 0; i < 20; i += 2)
        {
            if (i < 7)
            {
                map.remove(id[i]);
            }
            else
            {
                pgAssertThrow(map.remove(id[i]), pgMap<pgID, u32>::ExceptionNotFound);
            }
        }

        pgAssert(map.getHashSize() == 5 && map.getDataNum() == 3);

        for (u32 i = 0; i < 20; i++)
        {
            if (i < 7 && i % 2 != 0)
            {
                pgAssert(*map.get(id[i]) == 100 + i);
                pgAssert(*map.getN(id[i]) == 100 + i);
            }
            else
            {
                pgAssertThrow(map.get(id[i]), pgMap<pgID, u32>::ExceptionNotFound);
                pgAssert(!map.getN(id[i]));
            }
        }

        map.clear();
        pgAssert(map.getHashSize() == 5 && map.getDataNum() == 0);

        for (s32 i = 0; i < 20; i++)
        {
            pgAssertThrow(map.get(id[i]), pgMap<pgID, u32>::ExceptionNotFound);
            pgAssert(!map.getN(id[i]));
        }

        map.clear();
        pgAssert(map.getHashSize() == 5 && map.getDataNum() == 0);
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
        pgMap<pgID, u32> map;

        pgAssertThrow(map.getFirstKeyN(), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.getLastKeyN(), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.getPrevKeyN(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.getNextKeyN(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.moveFirst(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.moveLast(pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.moveBefore(pgID::ZERO, pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);
        pgAssertThrow(map.moveAfter(pgID::ZERO, pgID::ZERO), pgMap<pgID, u32>::ExceptionNotInitialized);

        map.init(2);
        pgAssert(!map.getFirstKeyN() && !map.getLastKeyN());

        pgID::setCurIDForSystem(1);

        pgID id1 = pgID::genID();
        pgID id2 = pgID::genID();
        pgID id3 = pgID::genID();

        map.add(id1, 123);
        map.add(id2, 456);
        map.add(id3, 789);
        pgAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id3);
        pgAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id2);
        pgAssert(*map.getPrevKeyN(id2) == id1 && *map.getNextKeyN(id2) == id3);
        pgAssert(*map.getPrevKeyN(id3) == id2 && !map.getNextKeyN(id3));

        map.remove(id2);
        pgAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id3);
        pgAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id3);
        pgAssert(*map.getPrevKeyN(id3) == id1 && !map.getNextKeyN(id3));

        map.remove(id1);
        pgAssert(*map.getFirstKeyN() == id3 && *map.getLastKeyN() == id3);
        pgAssert(!map.getPrevKeyN(id3) && !map.getNextKeyN(id3));

        pgAssertThrow(map.getPrevKeyN(pgID::ZERO), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.getPrevKeyN(pgID::genID()), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.getNextKeyN(pgID::ZERO), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.getNextKeyN(pgID::genID()), pgMap<pgID, u32>::ExceptionInvalidArgument);

        pgAssertThrow(map.moveFirst(id1), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.moveLast(id1), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.moveBefore(id1, id3), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.moveBefore(id3, id1), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.moveAfter(id1, id3), pgMap<pgID, u32>::ExceptionInvalidArgument);
        pgAssertThrow(map.moveAfter(id3, id1), pgMap<pgID, u32>::ExceptionInvalidArgument);

        map.add(id1, 123);
        map.add(id2, 456);
        pgAssert(*map.getFirstKeyN() == id3 && *map.getLastKeyN() == id2);
        pgAssert(!map.getPrevKeyN(id3) && *map.getNextKeyN(id3) == id1);
        pgAssert(*map.getPrevKeyN(id1) == id3 && *map.getNextKeyN(id1) == id2);
        pgAssert(*map.getPrevKeyN(id2) == id1 && !map.getNextKeyN(id2));

        map.moveFirst(id1);
        pgAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id2);
        pgAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id3);
        pgAssert(*map.getPrevKeyN(id3) == id1 && *map.getNextKeyN(id3) == id2);
        pgAssert(*map.getPrevKeyN(id2) == id3 && !map.getNextKeyN(id2));

        map.moveLast(id3);
        pgAssert(*map.getFirstKeyN() == id1 && *map.getLastKeyN() == id3);
        pgAssert(!map.getPrevKeyN(id1) && *map.getNextKeyN(id1) == id2);
        pgAssert(*map.getPrevKeyN(id2) == id1 && *map.getNextKeyN(id2) == id3);
        pgAssert(*map.getPrevKeyN(id3) == id2 && !map.getNextKeyN(id3));

        map.moveBefore(id1, id3);
        pgAssert(*map.getFirstKeyN() == id2 && *map.getLastKeyN() == id3);
        pgAssert(!map.getPrevKeyN(id2) && *map.getNextKeyN(id2) == id1);
        pgAssert(*map.getPrevKeyN(id1) == id2 && *map.getNextKeyN(id1) == id3);
        pgAssert(*map.getPrevKeyN(id3) == id1 && !map.getNextKeyN(id3));

        map.moveAfter(id3, id2);
        pgAssert(*map.getFirstKeyN() == id2 && *map.getLastKeyN() == id1);
        pgAssert(!map.getPrevKeyN(id2) && *map.getNextKeyN(id2) == id3);
        pgAssert(*map.getPrevKeyN(id3) == id2 && *map.getNextKeyN(id3) == id1);
        pgAssert(*map.getPrevKeyN(id1) == id3 && !map.getNextKeyN(id1));
    }
}
