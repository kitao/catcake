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


template<class T> void assertAlone(pgList<T>& list)
{
    /*
        u32 getItemNum() const
        bool hasItem() const
        Item* getFirstN() const
        Item* getLastN() const
    */

    pgAssert(list.getItemNum() == 0 && !list.hasItem() && !list.getFirstN() && !list.getLastN());
}


template<class T> void assertAlone(typename pgList<T>::Item& item)
{
    /*
        Item* getPrevN() const
        Item* getNextN() const
        bool hasList() const
        pgList<T>* getListN() const
    */

    pgAssert(!item.hasList() && !item.getListN());

    pgAssertThrow(item.getPrevN(), typename pgList<T>::Item::ExceptionInvalidCall);
    pgAssertThrow(item.getNextN(), typename pgList<T>::Item::ExceptionInvalidCall);
}


template<class T> void assertItem(pgList<T>& list, typename pgList<T>::Item& item)
{
    pgAssert(list.getItemNum() == 1 && list.hasItem() && list.getFirstN() == &item && list.getLastN() == &item);
    pgAssert(!item.getPrevN() && !item.getNextN() && item.hasList() && item.getListN() == &list);
}


template<class T> void assertItem(pgList<T>& list, typename pgList<T>::Item& item1, typename pgList<T>::Item& item2)
{
    pgAssert(list.getItemNum() == 2 && list.hasItem() && list.getFirstN() == &item1 && list.getLastN() == &item2);
    pgAssert(!item1.getPrevN() && item1.getNextN() == &item2 && item1.hasList() && item1.getListN() == &list);
    pgAssert(item2.getPrevN() == &item1 && !item2.getNextN() && item2.hasList() && item2.getListN() == &list);
}


template<class T> void assertItem(pgList<T>& list, typename pgList<T>::Item& item1, typename pgList<T>::Item& item2, typename pgList<T>::Item& item3)
{
    pgAssert(list.getItemNum() == 3 && list.hasItem() && list.getFirstN() == &item1 && list.getLastN() == &item3);
    pgAssert(!item1.getPrevN() && item1.getNextN() == &item2 && item1.hasList() && item1.getListN() == &list);
    pgAssert(item2.getPrevN() == &item1 && item2.getNextN() == &item3 && item2.hasList() && item2.getListN() == &list);
    pgAssert(item3.getPrevN() == &item2 && !item3.getNextN() && item3.hasList() && item3.getListN() == &list);
}


void pgListTest()
{
    /*
        Item()
        ~Item()
        void init(T* self)
        T* getSelf() const

        pgList()
        ~pgList()
        void addFirst(Item* item)
    */
    {
        pgList<u32> list;
        assertAlone(list);

        {
            pgList<u32>::Item item;
            u32 val = 1;

            pgAssertThrow(item.init(NULL), pgList<u32>::Item::ExceptionInvalidArgument);
            pgAssertThrow(item.getSelf(), pgList<u32>::Item::ExceptionNotInitialized);

            assertAlone<u32>(item);

            item.init(&val);
            pgAssert(item.getSelf() == &val);

            list.addFirst(&item);
            assertItem(list, item);

            pgAssertThrow(list.addFirst(NULL), pgList<u32>::ExceptionInvalidArgument);
        }

        assertAlone(list);
    }

    /*
        void addLast(Item* item)
    */
    {
        pgList<u32>::Item item;

        {
            pgList<u32> list;

            list.addLast(&item);
            assertItem(list, item);

            pgAssertThrow(list.addLast(NULL), pgList<u32>::ExceptionInvalidArgument);
        }

        assertAlone<u32>(item);
    }

    /*
        void leave()

        void clear()
    */
    {
        pgList<u32> list;
        pgList<u32>::Item item1, item2, item3;

        list.addFirst(&item1);
        list.addLast(&item2);
        list.addLast(&item3);
        assertItem(list, item1, item2, item3);

        item3.leave();
        item3.leave();
        assertItem(list, item1, item2);
        assertAlone<u32>(item3);

        list.addFirst(&item3);
        assertItem(list, item3, item1, item2);

        list.addLast(&item1);
        assertItem(list, item3, item2, item1);

        list.clear();
        assertAlone(list);
        assertAlone<u32>(item1);
        assertAlone<u32>(item2);
        assertAlone<u32>(item3);

        list.clear();
    }

    /*
        void joinBefore(Item* item)
        void joinAfter(Item* item)
    */
    {
        pgList<u32> list;
        pgList<u32>::Item item1, item2, item3;

        list.addFirst(&item2);

        item1.joinBefore(&item2);
        item3.joinAfter(&item2);
        assertItem(list, item1, item2, item3);

        item2.joinBefore(&item1);
        item1.joinAfter(&item3);
        assertItem(list, item2, item3, item1);

        pgAssertThrow(item1.joinBefore(NULL), pgList<u32>::Item::ExceptionInvalidArgument);
        pgAssertThrow(item1.joinBefore(&item1), pgList<u32>::Item::ExceptionInvalidArgument);

        item1.leave();
        pgAssertThrow(item2.joinBefore(&item1), pgList<u32>::Item::ExceptionInvalidArgument);

        pgAssertThrow(item3.joinAfter(NULL), pgList<u32>::Item::ExceptionInvalidArgument);
        pgAssertThrow(item3.joinAfter(&item3), pgList<u32>::Item::ExceptionInvalidArgument);

        item3.leave();
        pgAssertThrow(item2.joinAfter(&item3), pgList<u32>::Item::ExceptionInvalidArgument);
    }
}
