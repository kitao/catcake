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
    A tree container which can have a parent and children.
    @tparam T The type which is stored in the pgTree.
*/
template<class T> class pgTree
{
public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);
    pgDefineException(ExceptionNotInitialized);

    /*!
        Constructs a pgTree.
    */
    pgTree()
    {
        m_self = NULL;
        m_parent = NULL;
        m_last_child = NULL;
        m_prev = m_next = NULL;
    }

    /*!
        Destructs this pgTree.
    */
    ~pgTree()
    {
        leave();
        clear();
    }

    /*!
        Initializes this pgTree to have the specified value.
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
        Returns the value of this pgTree.
        @return The value of this pgTree.
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
        Returns whether this pgTree has a parent pgTree.
        @return Whether this pgTree has a parent pgTree.
    */
    bool hasParent() const
    {
        return m_parent ? true : false;
    }

    /*!
        Returns the parent pgTree of this pgTree.
        If this pgTree has no parent pgTree, returns NULL.
        @return The parent pgTree of this pgTree.
    */
    pgTree<T>* getParentN() const
    {
        return m_parent;
    }

    /*!
        Returns the previous pgTree of this pgTree.
        If this pgTree has no previous pgTree, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous pgTree of this pgTree.
    */
    pgTree<T>* getPrevAllN() const
    {
        return m_prev;
    }

    /*!
        Returns the next pgTree of this pgTree.
        If this pgTree has no next pgTree, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The next pgTree of this pgTree.
    */
    pgTree<T>* getNextAllN() const
    {
        return m_next;
    }

    /*!
        Returns the previous sibling pgTree of this pgTree.
        If this pgTree has no previous sibling pgTree, returns NULL.
        @return The previous sibling pgTree of this pgTree.
    */
    pgTree<T>* getPrevSiblingN() const
    {
        if (!hasParent())
        {
            pgThrow(ExceptionInvalidCall);
        }

        if (m_prev != m_parent)
        {
            pgTree<T>* prev = m_prev;

            while (prev->m_parent != m_parent)
            {
                prev = prev->m_parent;
            }

            return prev;
        }
        else
        {
            return NULL;
        }
    }

    /*!
        Returns the next sibling pgTree of this pgTree.
        If this pgTree has no next sibling pgTree, returns NULL.
        @return The next sibling pgTree of this pgTree.
    */
    pgTree<T>* getNextSiblingN() const
    {
        if (!hasParent())
        {
            pgThrow(ExceptionInvalidCall);
        }

        pgTree<T>* desc = getLastDescendant();
        pgTree<T>* next = desc->m_next;

        return (next && next->m_parent == m_parent) ? next : NULL;
    }

    /*!
        Insertes this pgTree before the specified pgTree.
        @param[in] tree A pgTree.
    */
    void joinBefore(pgTree<T>* tree)
    {
        if (!tree || tree == this || !tree->hasParent())
        {
            pgThrow(ExceptionInvalidArgument);
        }

        for (pgTree<T>* parent = tree->m_parent; parent; parent = parent->m_parent)
        {
            if (parent == this)
            {
                pgThrow(ExceptionInvalidArgument);
            }
        }

        leave();

        pgTree<T>* desc = getLastDescendant();

        m_parent = tree->m_parent;
        m_prev = tree->m_prev;
        desc->m_next = tree;

        m_prev->m_next = this;
        desc->m_next->m_prev = desc;
    }

    /*!
        Insertes this pgTree after the specified pgTree.
        @param[in] tree A pgTree.
    */
    void joinAfter(pgTree<T>* tree)
    {
        if (!tree || tree == this || !tree->hasParent())
        {
            pgThrow(ExceptionInvalidArgument);
        }

        for (pgTree<T>* parent = tree->m_parent; parent; parent = parent->m_parent)
        {
            if (parent == this)
            {
                pgThrow(ExceptionInvalidArgument);
            }
        }

        leave();

        pgTree<T>* tree_desc = tree->getLastDescendant();
        pgTree<T>* this_desc = getLastDescendant();

        m_parent = tree->m_parent;
        m_prev = tree_desc;
        this_desc->m_next = tree_desc->m_next;

        m_prev->m_next = this;

        if (this_desc->m_next)
        {
            this_desc->m_next->m_prev = this_desc;
        }

        if (m_parent->m_last_child == tree)
        {
            m_parent->m_last_child = this;
        }
    }

    /*!
        Removes this pgTree from the parent pgTree.
    */
    void leave()
    {
        if (hasParent())
        {
            pgTree<T>* desc = getLastDescendant();

            if (m_parent->m_last_child == this)
            {
                m_parent->m_last_child = getPrevSiblingN();
            }

            m_prev->m_next = desc->m_next;

            if (desc->m_next)
            {
                desc->m_next->m_prev = m_prev;
            }

            m_parent = NULL;
            m_prev = desc->m_next = NULL;
        }
    }

    /*!
        Returns Whether this pgTree has any child pgTree.
        @return Whether this pgTree has any child pgTree.
    */
    bool hasChild() const
    {
        return m_last_child ? true : false;
    }

    /*!
        Returns the first child pgTree of this pgTree.
        If this pgTree has no child pgTree, returns NULL.
        @return The first child pgTree of this pgTree.
    */
    pgTree<T>* getFirstChildN() const
    {
        return hasChild() ? m_next : NULL;
    }

    /*!
        Returns the last child pgTree of this pgTree.
        If this pgTree has no child pgTree, returns NULL.
        @return The last child pgTree of this pgTree.
    */
    pgTree<T>* getLastChildN() const
    {
        return m_last_child;
    }

    /*!
        Returns the last descendant of this pgTree.
        If this pgTree has no child, returns itself.
        @return The last descendant of this pgTree.
    */
    pgTree<T>* getLastDescendant() const
    {
        pgTree<T>* desc = const_cast<pgTree<T>*>(this);

        while (desc->m_last_child)
        {
            desc = desc->m_last_child;
        }

        return desc;
    }

    /*!
        Adds the specified pgTree to this pgTree as the first child pgTree.
        @param[in] child A pgTree.
    */
    void addFirst(pgTree<T>* child)
    {
        if (!child || child == this)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        for (pgTree<T>* parent = m_parent; parent; parent = parent->m_parent)
        {
            if (parent == child)
            {
                pgThrow(ExceptionInvalidArgument);
            }
        }

        child->leave();

        pgTree<T>* child_desc = child->getLastDescendant();

        child->m_parent = this;
        child->m_prev = this;
        child_desc->m_next = m_next;

        child->m_prev->m_next = child;

        if (child_desc->m_next)
        {
            child_desc->m_next->m_prev = child_desc;
        }

        if (!m_last_child)
        {
            m_last_child = child;
        }
    }

    /*!
        Adds the specified pgTree to this pgTree as the last child pgTree.
        @param[in] child A pgTree.
    */
    void addLast(pgTree<T>* child)
    {
        if (!child || child == this)
        {
            pgThrow(ExceptionInvalidArgument);
        }

        for (pgTree<T>* parent = m_parent; parent; parent = parent->m_parent)
        {
            if (parent == child)
            {
                pgThrow(ExceptionInvalidArgument);
            }
        }

        child->leave();

        pgTree<T>* this_desc = getLastDescendant();
        pgTree<T>* child_desc = child->getLastDescendant();

        child->m_parent = this;
        child->m_prev = this_desc;
        child_desc->m_next = this_desc->m_next;

        child->m_prev->m_next = child;

        if (child_desc->m_next)
        {
            child_desc->m_next->m_prev = child_desc;
        }

        m_last_child = child;
    }

    /*!
        Removes all child pgTrees from this pgTree.
    */
    void clear()
    {
        while (hasChild())
        {
            getFirstChildN()->leave();
        }
    }

private:
    pgTree(const pgTree<T>&) {}
    void operator=(const pgTree<T>&) {}

    T* m_self;
    pgTree<T>* m_parent;
    pgTree<T>* m_last_child;
    pgTree<T>* m_prev;
    pgTree<T>* m_next;
};
