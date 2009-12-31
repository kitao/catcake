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


/*!
    @ingroup ckGen
    A sequence of elements stored in a linked list.
    @tparam T The type which is stored in the list.
*/
template<class T> class ckList
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    //! @endcond

    /*!
        An element of ckList.
    */
    class Item
    {
        friend class ckList<T>;

    public:
        //! @cond
        ckDefineException(ExceptionInvalidArgument);
        ckDefineException(ExceptionInvalidCall);
        ckDefineException(ExceptionNotInitialized);
        //! @endcond

        /*!
            Constructs a ckList::Item.
        */
        Item()
        {
            m_self = NULL;
            m_list = NULL;
            m_prev = m_next = NULL;
        }

        /*!
            Destructs this ckList::Item.
        */
        ~Item()
        {
            leave();
        }

        /*!
            Initializes this ckList::Item to have the specified value.
            This method can be called more than once.
            @param[in] self The pointer to a value.
        */
        void init(T* self)
        {
            if (!self)
            {
                ckThrow(ExceptionInvalidArgument);
            }

            m_self = self;
        }

        /*!
            Returns the value of this ckList::Item.
            @return The value of this ckList::Item.
        */
        T* getSelf() const
        {
            if (!m_self)
            {
                ckThrow(ExceptionNotInitialized);
            }

            return m_self;
        }

        /*!
            Returns the previous ckList::Item of this ckList::Item.
            If the ckList::Item has no previous ckList::Item, returns NULL.
            @return The previous ckList::Item of this ckList::Item.
        */
        Item* getPrevN() const
        {
            if (!hasList())
            {
                ckThrow(ExceptionInvalidCall);
            }

            return (m_prev != &m_list->m_start) ? m_prev : NULL;
        }

        /*!
            Returns the next ckList::Item of this ckList::Item.
            If the ckList::Item has no next ckList::Item, returns NULL.
            @return The next ckList::Item of this ckList::Item.
        */
        Item* getNextN() const
        {
            if (!hasList())
            {
                ckThrow(ExceptionInvalidCall);
            }

            return (m_next != &m_list->m_end) ? m_next : NULL;
        }

        /*!
            Returns whether this ckList::Item belongs to a ckList.
            @return Wheter this ckList::Item belongs to a ckList.
        */
        bool hasList() const
        {
            return m_list ? true : false;
        }

        /*!
            Returns the ckList which this ckList::Item belongs to.
            If ths ckList::Item does not belong to any ckList, returns NULL.
            @return The ckList which this ckList::Item belongs to.
        */
        ckList<T>* getListN() const
        {
            return m_list;
        }

        /*!
            Insertes this ckList::Item before the specified ckList::Item.
            @param[in] item A ckList::Item.
        */
        void joinBefore(Item* item)
        {
            if (!item || item == this || !item->hasList())
            {
                ckThrow(ExceptionInvalidArgument);
            }

            leave();

            m_list = item->m_list;

            m_prev = item->m_prev;
            m_next = item;

            m_prev->m_next = m_next->m_prev = this;

            m_list->m_item_num++;
        }

        /*!
            Insertes this ckList::Item after the specified ckList::Item.
            @param[in] item A ckList::Item.
        */
        void joinAfter(Item* item)
        {
            if (!item || item == this || !item->hasList())
            {
                ckThrow(ExceptionInvalidArgument);
            }

            leave();

            m_list = item->m_list;

            m_prev = item;
            m_next = item->m_next;

            m_prev->m_next = m_next->m_prev = this;

            m_list->m_item_num++;
        }

        /*!
            Removes this ckList::Item from the ckList which the ckList::Item belongs to.
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
        ckList<T>* m_list;
        Item* m_prev;
        Item* m_next;
    };

    /*!
        Constructs a ckList with no item.
    */
    ckList()
    {
        m_item_num = 0;

        m_start.m_list = m_end.m_list = this;
        m_start.m_next = &m_end;
        m_end.m_prev = &m_start;
    }

    /*!
        Destructs this ckList.
    */
    ~ckList()
    {
        clear();
    }

    /*!
        Returns the number of ckList::Items in this ckList.
        @return The number of ckList::Items in this ckList.
    */
    u32 getItemNum() const
    {
        return m_item_num;
    }

    /*!
        Returns whether this ckList has any ckList::Item.
        @return Whether this ckList has any ckList::Item.
    */
    bool hasItem() const
    {
        return getFirstN() ? true : false;
    }

    /*!
        Returns the first ckList::Item in this ckList.
        If this ckList has no ckList::Item, returns NULL.
        @return The first ckList::Item in this ckList.
    */
    Item* getFirstN() const
    {
        return (m_start.m_next != &m_end) ? m_start.m_next : NULL;
    }

    /*!
        Returns the last ckList::Item in this ckList.
        If this list has no ckList::Item, returns NULL.
        @return The last ckList::Item in this ckList.
    */
    Item* getLastN() const
    {
        return (m_end.m_prev != &m_start) ? m_end.m_prev : NULL;
    }

    /*!
        Adds the specified ckList::Item to this ckList as the first ckList::Item.
        @param[in] item A ckList::Item.
    */
    void addFirst(Item* item)
    {
        if (!item)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        item->joinAfter(&m_start);
    }

    /*!
        Adds the specified ckList::Item to this ckList as the last ckList::Item.
        @param[in] item A ckList::Item.
    */
    void addLast(Item* item)
    {
        if (!item)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        item->joinBefore(&m_end);
    }

    /*!
        Removes all ckList::Items from this ckList.
    */
    void clear()
    {
        while (hasItem())
        {
            getFirstN()->leave();
        }
    }

private:
    ckList(const ckList<T>&) {}
    void operator=(const ckList<T>&) {}

    Item m_start;
    Item m_end;
    u32 m_item_num;
};
