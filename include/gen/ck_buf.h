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
    A memory pool for the specified type.
    @tparam T The type which is managed by the memory pool.
*/
template<class T> class pgBuf
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotInitialized);
    pgDefineException(ExceptionOutOfBuffer);

    /*!
        Constructs and initialize a pgBuf whose size is zero.
    */
    pgBuf()
    {
        m_elem = NULL;
        m_state = NULL;
        m_index_tbl = NULL;
        m_size = m_used_size = 0;
    }

    /*!
        Destructs this pgBuf.
    */
    ~pgBuf()
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

            pgFree(m_elem);
            pgDeleteArray(m_state, u8);
            pgDeleteArray(m_index_tbl, u16);
        }
    }

    /*!
        Initializes this pgBuf with the specified size.
        @param[in] size The size of this pgBuf.
    */
    void init(u16 size)
    {
        if (size == 0)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        this->~pgBuf();

        m_size = size;
        m_used_size = 0;

        m_elem = static_cast<T*>(pgMalloc(sizeof(T) * m_size));
        pgNewArray(m_state, u8, m_size);
        pgNewArray(m_index_tbl, u16, m_size);

        for (s32 i = 0; i < m_size; i++)
        {
            m_state[i] = STATE_FREE;
            m_index_tbl[i] = i;
        }
    }

    /*!
        Returns the size of this pgBuf.
        @return The size of this pgBuf.
    */
    u16 getSize() const
    {
        return m_size;
    }

    /*!
        Returns the used size of this pgBuf.
        @return The used size of this pgBuf.
    */
    u16 getUsedSize() const
    {
        return m_used_size;
    }

    /*!
        Returns the available size of this pgBuf.
        @return The available size of this pgBuf.
    */
    u16 getFreeSize() const
    {
        return m_size - m_used_size;
    }

    /*!
        Returns the first value in this pgBuf.
        If this pgBuf has no value, returns NULL.
        @return The first value in this pgBuf.
    */
    T* getFirstN() const
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
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
        Returns the last value in this pgBuf.
        If this pgBuf has no value, returns NULL.
        @return The last value in this pgBuf.
    */
    T* getLastN() const
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
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
        @param[in] elem A value in this pgBuf.
        @return The previous value of the specified value.
    */
    T* getPrevN(const T* elem) const
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            pgThrow(ExceptionInvalidArgument);
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
        @param[in] elem A value in this pgBuf.
        @return The next value of the specified value.
    */
    T* getNextN(const T* elem) const
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            pgThrow(ExceptionInvalidArgument);
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
        Allocates a value from this pgBuf.
        @return An allocated value.
    */
    T* newElement()
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
        }

        if (m_used_size >= m_size)
        {
            pgThrow(ExceptionOutOfBuffer);
        }

        u16 index = m_index_tbl[m_used_size];

        m_state[index] = STATE_USED;

        m_used_size++;

        T* elem = &m_elem[index];
        new(elem, NULL) T;

        return elem;
    }

    /*!
        Deletes the specified value in this pgBuf.
        @param[in] elem A value in this pgBuf.
    */
    void deleteElement(T* elem)
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        elem->~T();

        m_used_size--;

        m_index_tbl[m_used_size] = index;
        m_state[index] = STATE_FREE;
    }

    /*!
        Deletes all values in this pgBuf.
    */
    void clear()
    {
        if (!m_elem)
        {
            pgThrow(ExceptionNotInitialized);
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

    pgBuf(const pgBuf<T>&) {}
    void operator=(const pgBuf<T>&) {}

    T* m_elem;
    u8* m_state;
    u16* m_index_tbl;
    u16 m_size;
    u16 m_used_size;
};
