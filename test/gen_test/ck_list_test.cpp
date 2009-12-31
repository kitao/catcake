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


template<class T> void assertAlone(ckList<T>& list)
{
    /*
        u32 getItemNum() const
        bool hasItem() const
        Item* getFirstN() const
        Item* getLastN() const
    */

    ckAssert(list.getItemNum() == 0 && !list.hasItem() && !list.getFirstN() && !list.getLastN());
}


template<class T> void assertAlone(typename ckList<T>::Item& item)
{
    /*
        Item* getPrevN() const
        Item* getNextN() const
        bool hasList() const
        ckList<T>* getListN() const
    */

    ckAssert(!item.hasList() && !item.getListN());

    ckAssertThrow(item.getPrevN(), typename ckList<T>::Item::ExceptionInvalidCall);
    ckAssertThrow(item.getNextN(), typename ckList<T>::Item::ExceptionInvalidCall);
}


template<class T> void assertItem(ckList<T>& list, typename ckList<T>::Item& item)
{
    ckAssert(list.getItemNum() == 1 && list.hasItem() && list.getFirstN() == &item && list.getLastN() == &item);
    ckAssert(!item.getPrevN() && !item.getNextN() && item.hasList() && item.getListN() == &list);
}


template<class T> void assertItem(ckList<T>& list, typename ckList<T>::Item& item1, typename ckList<T>::Item& item2)
{
    ckAssert(list.getItemNum() == 2 && list.hasItem() && list.getFirstN() == &item1 && list.getLastN() == &item2);
    ckAssert(!item1.getPrevN() && item1.getNextN() == &item2 && item1.hasList() && item1.getListN() == &list);
    ckAssert(item2.getPrevN() == &item1 && !item2.getNextN() && item2.hasList() && item2.getListN() == &list);
}


template<class T> void assertItem(ckList<T>& list, typename ckList<T>::Item& item1, typename ckList<T>::Item& item2, typename ckList<T>::Item& item3)
{
    ckAssert(list.getItemNum() == 3 && list.hasItem() && list.getFirstN() == &item1 && list.getLastN() == &item3);
    ckAssert(!item1.getPrevN() && item1.getNextN() == &item2 && item1.hasList() && item1.getListN() == &list);
    ckAssert(item2.getPrevN() == &item1 && item2.getNextN() == &item3 && item2.hasList() && item2.getListN() == &list);
    ckAssert(item3.getPrevN() == &item2 && !item3.getNextN() && item3.hasList() && item3.getListN() == &list);
}


void ckListTest()
{
    /*
        Item()
        ~Item()
        void init(T* self)
        T* getSelf() const

        ckList()
        ~ckList()
        void addFirst(Item* item)
    */
    {
        ckList<u32> list;
        assertAlone(list);

        {
            ckList<u32>::Item item;
            u32 val = 1;

            ckAssertThrow(item.init(NULL), ckList<u32>::Item::ExceptionInvalidArgument);
            ckAssertThrow(item.getSelf(), ckList<u32>::Item::ExceptionNotInitialized);

            assertAlone<u32>(item);

            item.init(&val);
            ckAssert(item.getSelf() == &val);

            list.addFirst(&item);
            assertItem(list, item);

            ckAssertThrow(list.addFirst(NULL), ckList<u32>::ExceptionInvalidArgument);
        }

        assertAlone(list);
    }

    /*
        void addLast(Item* item)
    */
    {
        ckList<u32>::Item item;

        {
            ckList<u32> list;

            list.addLast(&item);
            assertItem(list, item);

            ckAssertThrow(list.addLast(NULL), ckList<u32>::ExceptionInvalidArgument);
        }

        assertAlone<u32>(item);
    }

    /*
        void leave()

        void clear()
    */
    {
        ckList<u32> list;
        ckList<u32>::Item item1, item2, item3;

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
        ckList<u32> list;
        ckList<u32>::Item item1, item2, item3;

        list.addFirst(&item2);

        item1.joinBefore(&item2);
        item3.joinAfter(&item2);
        assertItem(list, item1, item2, item3);

        item2.joinBefore(&item1);
        item1.joinAfter(&item3);
        assertItem(list, item2, item3, item1);

        ckAssertThrow(item1.joinBefore(NULL), ckList<u32>::Item::ExceptionInvalidArgument);
        ckAssertThrow(item1.joinBefore(&item1), ckList<u32>::Item::ExceptionInvalidArgument);

        item1.leave();
        ckAssertThrow(item2.joinBefore(&item1), ckList<u32>::Item::ExceptionInvalidArgument);

        ckAssertThrow(item3.joinAfter(NULL), ckList<u32>::Item::ExceptionInvalidArgument);
        ckAssertThrow(item3.joinAfter(&item3), ckList<u32>::Item::ExceptionInvalidArgument);

        item3.leave();
        ckAssertThrow(item2.joinAfter(&item3), ckList<u32>::Item::ExceptionInvalidArgument);
    }
}
