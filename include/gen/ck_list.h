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


/*!
    @ingroup pgGen
    A Sequence of elements stored in a linked list.
    @tparam T The type which is stored in the pgList.
*/
template<class T> class pgList
{
public:
    pgDefineException(ExceptionInvalidArgument);

    /*!
        An element of pgList.
    */
    class Item
    {
        friend class pgList<T>;

    public:
        pgDefineException(ExceptionInvalidArgument);
        pgDefineException(ExceptionInvalidCall);
        pgDefineException(ExceptionNotInitialized);

        /*!
            Constructs a pgList::Item.
        */
        Item()
        {
            m_self = NULL;
            m_list = NULL;
            m_prev = m_next = NULL;
        }

        /*!
            Destructs this pgList::Item.
        */
        ~Item()
        {
            leave();
        }

        /*!
            Initializes this pgList::Item to have the specified value.
            This method can be called more than once.
            @param[in] self The pointer to a value.
        */
        void init(T* self)
        {
            if (!self)
            {
                pgThrow(ExceptionInvalidArgument);
            }

            m_self = self;
        }

        /*!
            Returns the value of this pgList::Item.
            @return The value of this pgList::Item.
        */
        T* getSelf() const
        {
            if (!m_self)
            {
                pgThrow(ExceptionNotInitialized);
            }

            return m_self;
        }

        /*!
            Returns the previous pgList::Item of this pgList::Item.
            If the pgList::Item has no previous pgList::Item, returns NULL.
            @return The previous pgList::Item of this pgList::Item.
        */
        Item* getPrevN() const
        {
            if (!hasList())
            {
                pgThrow(ExceptionInvalidCall);
            }

            return (m_prev != &m_list->m_start) ? m_prev : NULL;
        }

        /*!
            Returns the next pgList::Item of this pgList::Item.
            If the pgList::Item has no next pgList::Item, returns NULL.
            @return The next pgList::Item of this pgList::Item.
        */
        Item* getNextN() const
        {
            if (!hasList())
            {
                pgThrow(ExceptionInvalidCall);
            }

            return (m_next != &m_list->m_end) ? m_next : NULL;
        }

        /*!
            Returns whether this pgList::Item belongs to a pgList.
            @return Wheter this pgList::Item belongs to a pgList.
        */
        bool hasList() const
        {
            return m_list ? true : false;
        }

        /*!
            Returns the pgList which this pgList::Item belongs to.
            If ths pgList::Item does not belong to any pgList, returns NULL.
            @return The pgList which this pgList::Item belongs to.
        */
        pgList<T>* getListN() const
        {
            return m_list;
        }

        /*!
            Insertes this pgList::Item before the specified pgList::Item.
            @param[in] item A pgList::Item.
        */
        void joinBefore(Item* item)
        {
            if (!item || item == this || !item->hasList())
            {
                pgThrow(ExceptionInvalidArgument);
            }

            leave();

            m_list = item->m_list;

            m_prev = item->m_prev;
            m_next = item;

            m_prev->m_next = m_next->m_prev = this;

            m_list->m_item_num++;
        }

        /*!
            Insertes this pgList::Item after the specified pgList::Item.
            @param[in] item A pgList::Item.
        */
        void joinAfter(Item* item)
        {
            if (!item || item == this || !item->hasList())
            {
                pgThrow(ExceptionInvalidArgument);
            }

            leave();

            m_list = item->m_list;

            m_prev = item;
            m_next = item->m_next;

            m_prev->m_next = m_next->m_prev = this;

            m_list->m_item_num++;
        }

        /*!
            Removes this pgList::Item from the pgList which the pgList::Item belongs to.
        */
        void leave()
        {
            if (m_prev && m_next)
            {
                m_list->m_item_num--;

                m_prev->m_next = m_next;
                m_next->m_prev = m_prev;

                m_list = NULL;
                m_prev = m_next = NULL;
            }
        }

    private:
        Item(const Item&) {}
        void operator=(const Item&) {}

        T* m_self;
        pgList<T>* m_list;
        Item* m_prev;
        Item* m_next;
    };

    /*!
        Constructs a pgList with no item.
    */
    pgList()
    {
        m_item_num = 0;

        m_start.m_list = m_end.m_list = this;
        m_start.m_next = &m_end;
        m_end.m_prev = &m_start;
    }

    /*!
        Destructs this pgList.
    */
    ~pgList()
    {
        clear();
    }

    /*!
        Returns the number of pgList::Items in this pgList.
        @return The number of pgList::Items in this pgList.
    */
    u32 getItemNum() const
    {
        return m_item_num;
    }

    /*!
        Returns whether this pgList has any pgList::Item.
        @return Whether this pgList has any pgList::Item.
    */
    bool hasItem() const
    {
        return getFirstN() ? true : false;
    }

    /*!
        Returns the first pgList::Item in this pgList.
        If this pgList has no pgList::Item, returns NULL.
        @return The first pgList::Item in this pgList.
    */
    Item* getFirstN() const
    {
        return (m_start.m_next != &m_end) ? m_start.m_next : NULL;
    }

    /*!
        Returns the last pgList::Item in this pgList.
        If this list has no pgList::Item, returns NULL.
        @return The last pgList::Item in this pgList.
    */
    Item* getLastN() const
    {
        return (m_end.m_prev != &m_start) ? m_end.m_prev : NULL;
    }

    /*!
        Adds the specified pgList::Item to this pgList as the first pgList::Item.
        @param[in] item A pgList::Item.
    */
    void addFirst(Item* item)
    {
        if (!item)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        item->joinAfter(&m_start);
    }

    /*!
        Adds the specified pgList::Item to this pgList as the last pgList::Item.
        @param[in] item A pgList::Item.
    */
    void addLast(Item* item)
    {
        if (!item)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        item->joinBefore(&m_end);
    }

    /*!
        Removes all pgList::Items from this pgList.
    */
    void clear()
    {
        while (hasItem())
        {
            getFirstN()->leave();
        }
    }

private:
    pgList(const pgList<T>&) {}
    void operator=(const pgList<T>&) {}

    Item m_start;
    Item m_end;
    u32 m_item_num;
};
