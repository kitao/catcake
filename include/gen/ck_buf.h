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
    A memory pool for the specified type.
    @tparam T The type which is managed by the memory pool.
*/
template<class T> class ckBuf
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotInitialized);
    ckDefineException(ExceptionOutOfBuffer);
    //! @endcond

    /*!
        Constructs and initialize a ckBuf whose size is zero.
    */
    ckBuf()
    {
        m_elem = NULL;
        m_state = NULL;
        m_index_tbl = NULL;
        m_size = m_used_size = 0;
    }

    /*!
        Destructs this ckBuf.
    */
    ~ckBuf()
    {
        if (m_elem)
        {
            for (s32 i = 0; i < m_size; i++)
            {
                if (m_state[i] == STATE_USED)
                {
                    m_elem[i].~T();
                }
            }

            ckFree(m_elem);
            ckDeleteArray(m_state, u8);
            ckDeleteArray(m_index_tbl, u16);
        }
    }

    /*!
        Initializes this ckBuf with the specified size.
        @param[in] size The size of this ckBuf.
    */
    void init(u16 size)
    {
        if (size == 0)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        this->~ckBuf();

        m_size = size;
        m_used_size = 0;

        m_elem = static_cast<T*>(ckMalloc(sizeof(T) * m_size));
        ckNewArray(m_state, u8, m_size);
        ckNewArray(m_index_tbl, u16, m_size);

        for (s32 i = 0; i < m_size; i++)
        {
            m_state[i] = STATE_FREE;
            m_index_tbl[i] = i;
        }
    }

    /*!
        Returns the size of this ckBuf.
        @return The size of this ckBuf.
    */
    u16 getSize() const
    {
        return m_size;
    }

    /*!
        Returns the used size of this ckBuf.
        @return The used size of this ckBuf.
    */
    u16 getUsedSize() const
    {
        return m_used_size;
    }

    /*!
        Returns the available size of this ckBuf.
        @return The available size of this ckBuf.
    */
    u16 getFreeSize() const
    {
        return m_size - m_used_size;
    }

    /*!
        Returns the first value in this ckBuf.
        If this ckBuf has no value, returns NULL.
        @return The first value in this ckBuf.
    */
    T* getFirstN() const
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        for (s32 i = 0; i < m_size; i++)
        {
            if (m_state[i] == STATE_USED)
            {
                return &m_elem[i];
            }
        }

        return NULL;
    }

    /*!
        Returns the last value in this ckBuf.
        If this ckBuf has no value, returns NULL.
        @return The last value in this ckBuf.
    */
    T* getLastN() const
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        for (s32 i = m_size - 1; i >= 0; i--)
        {
            if (m_state[i] == STATE_USED)
            {
                return &m_elem[i];
            }
        }

        return NULL;
    }

    /*!
        Returns the previous value of the specified value.
        If the specified value has no previous value, returns NULL.
        @param[in] elem A value in this ckBuf.
        @return The previous value of the specified value.
    */
    T* getPrevN(const T* elem) const
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        if (index > 0)
        {
            for (s32 i = index - 1; i >= 0; i--)
            {
                if (m_state[i] == STATE_USED)
                {
                    return &m_elem[i];
                }
            }
        }

        return NULL;
    }

    /*!
        Returns the next value of the specified value.
        If the specified value has no next value, returns NULL.
        @param[in] elem A value in this ckBuf.
        @return The next value of the specified value.
    */
    T* getNextN(const T* elem) const
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        if (index + 1 < m_size)
        {
            for (s32 i = index + 1; i < m_size; i++)
            {
                if (m_state[i] == STATE_USED)
                {
                    return &m_elem[i];
                }
            }
        }

        return NULL;
    }

    /*!
        Allocates a value from this ckBuf.
        @return An allocated value.
    */
    T* newElement()
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        if (m_used_size >= m_size)
        {
            ckThrow(ExceptionOutOfBuffer);
        }

        u16 index = m_index_tbl[m_used_size];

        m_state[index] = STATE_USED;

        m_used_size++;

        T* elem = &m_elem[index];
        new(elem, NULL) T;

        return elem;
    }

    /*!
        Deletes the specified value in this ckBuf.
        @param[in] elem A value in this ckBuf.
    */
    void deleteElement(T* elem)
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        elem->~T();

        m_used_size--;

        m_index_tbl[m_used_size] = index;
        m_state[index] = STATE_FREE;
    }

    /*!
        Deletes all values in this ckBuf.
    */
    void clear()
    {
        if (!m_elem)
        {
            ckThrow(ExceptionNotInitialized);
        }

        m_used_size = 0;

        for (s32 i = 0; i < m_size; i++)
        {
            if (m_state[i] == STATE_USED)
            {
                m_elem[i].~T();
                m_state[i] = STATE_FREE;
            }

            m_index_tbl[i] = i;
        }
    }

private:
    enum ElemState
    {
        STATE_FREE, //
        STATE_USED
    };

    ckBuf(const ckBuf<T>&) {}
    void operator=(const ckBuf<T>&) {}

    T* m_elem;
    u8* m_state;
    u16* m_index_tbl;
    u16 m_size;
    u16 m_used_size;
};
