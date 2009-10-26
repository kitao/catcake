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
    @ingroup ckGen
    A tree container which can have a parent and children.
    @tparam T The type which is stored in the ckTree.
*/
template<class T> class ckTree
{
public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);

    /*!
        Constructs a ckTree.
    */
    ckTree()
    {
        m_self = NULL;
        m_parent = NULL;
        m_last_child = NULL;
        m_prev = m_next = NULL;
    }

    /*!
        Destructs this ckTree.
    */
    ~ckTree()
    {
        leave();
        clear();
    }

    /*!
        Initializes this ckTree to have the specified value.
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
        Returns the value of this ckTree.
        @return The value of this ckTree.
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
        Returns whether this ckTree has a parent ckTree.
        @return Whether this ckTree has a parent ckTree.
    */
    bool hasParent() const
    {
        return m_parent ? true : false;
    }

    /*!
        Returns the parent ckTree of this ckTree.
        If this ckTree has no parent ckTree, returns NULL.
        @return The parent ckTree of this ckTree.
    */
    ckTree<T>* getParentN() const
    {
        return m_parent;
    }

    /*!
        Returns the previous ckTree of this ckTree.
        If this ckTree has no previous ckTree, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous ckTree of this ckTree.
    */
    ckTree<T>* getPrevAllN() const
    {
        return m_prev;
    }

    /*!
        Returns the next ckTree of this ckTree.
        If this ckTree has no next ckTree, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The next ckTree of this ckTree.
    */
    ckTree<T>* getNextAllN() const
    {
        return m_next;
    }

    /*!
        Returns the previous sibling ckTree of this ckTree.
        If this ckTree has no previous sibling ckTree, returns NULL.
        @return The previous sibling ckTree of this ckTree.
    */
    ckTree<T>* getPrevSiblingN() const
    {
        if (!hasParent())
        {
            ckThrow(ExceptionInvalidCall);
        }

        if (m_prev != m_parent)
        {
            ckTree<T>* prev = m_prev;

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
        Returns the next sibling ckTree of this ckTree.
        If this ckTree has no next sibling ckTree, returns NULL.
        @return The next sibling ckTree of this ckTree.
    */
    ckTree<T>* getNextSiblingN() const
    {
        if (!hasParent())
        {
            ckThrow(ExceptionInvalidCall);
        }

        ckTree<T>* desc = getLastDescendant();
        ckTree<T>* next = desc->m_next;

        return (next && next->m_parent == m_parent) ? next : NULL;
    }

    /*!
        Insertes this ckTree before the specified ckTree.
        @param[in] tree A ckTree.
    */
    void joinBefore(ckTree<T>* tree)
    {
        if (!tree || tree == this || !tree->hasParent())
        {
            ckThrow(ExceptionInvalidArgument);
        }

        for (ckTree<T>* parent = tree->m_parent; parent; parent = parent->m_parent)
        {
            if (parent == this)
            {
                ckThrow(ExceptionInvalidArgument);
            }
        }

        leave();

        ckTree<T>* desc = getLastDescendant();

        m_parent = tree->m_parent;
        m_prev = tree->m_prev;
        desc->m_next = tree;

        m_prev->m_next = this;
        desc->m_next->m_prev = desc;
    }

    /*!
        Insertes this ckTree after the specified ckTree.
        @param[in] tree A ckTree.
    */
    void joinAfter(ckTree<T>* tree)
    {
        if (!tree || tree == this || !tree->hasParent())
        {
            ckThrow(ExceptionInvalidArgument);
        }

        for (ckTree<T>* parent = tree->m_parent; parent; parent = parent->m_parent)
        {
            if (parent == this)
            {
                ckThrow(ExceptionInvalidArgument);
            }
        }

        leave();

        ckTree<T>* tree_desc = tree->getLastDescendant();
        ckTree<T>* this_desc = getLastDescendant();

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
        Removes this ckTree from the parent ckTree.
    */
    void leave()
    {
        if (hasParent())
        {
            ckTree<T>* desc = getLastDescendant();

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
        Returns Whether this ckTree has any child ckTree.
        @return Whether this ckTree has any child ckTree.
    */
    bool hasChild() const
    {
        return m_last_child ? true : false;
    }

    /*!
        Returns the first child ckTree of this ckTree.
        If this ckTree has no child ckTree, returns NULL.
        @return The first child ckTree of this ckTree.
    */
    ckTree<T>* getFirstChildN() const
    {
        return hasChild() ? m_next : NULL;
    }

    /*!
        Returns the last child ckTree of this ckTree.
        If this ckTree has no child ckTree, returns NULL.
        @return The last child ckTree of this ckTree.
    */
    ckTree<T>* getLastChildN() const
    {
        return m_last_child;
    }

    /*!
        Returns the last descendant of this ckTree.
        If this ckTree has no child, returns itself.
        @return The last descendant of this ckTree.
    */
    ckTree<T>* getLastDescendant() const
    {
        ckTree<T>* desc = const_cast<ckTree<T>*>(this);

        while (desc->m_last_child)
        {
            desc = desc->m_last_child;
        }

        return desc;
    }

    /*!
        Adds the specified ckTree to this ckTree as the first child ckTree.
        @param[in] child A ckTree.
    */
    void addFirst(ckTree<T>* child)
    {
        if (!child || child == this)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        for (ckTree<T>* parent = m_parent; parent; parent = parent->m_parent)
        {
            if (parent == child)
            {
                ckThrow(ExceptionInvalidArgument);
            }
        }

        child->leave();

        ckTree<T>* child_desc = child->getLastDescendant();

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
        Adds the specified ckTree to this ckTree as the last child ckTree.
        @param[in] child A ckTree.
    */
    void addLast(ckTree<T>* child)
    {
        if (!child || child == this)
        {
            ckThrow(ExceptionInvalidArgument);
        }

        for (ckTree<T>* parent = m_parent; parent; parent = parent->m_parent)
        {
            if (parent == child)
            {
                ckThrow(ExceptionInvalidArgument);
            }
        }

        child->leave();

        ckTree<T>* this_desc = getLastDescendant();
        ckTree<T>* child_desc = child->getLastDescendant();

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
        Removes all child ckTrees from this ckTree.
    */
    void clear()
    {
        while (hasChild())
        {
            getFirstChildN()->leave();
        }
    }

private:
    ckTree(const ckTree<T>&) {}
    void operator=(const ckTree<T>&) {}

    T* m_self;
    ckTree<T>* m_parent;
    ckTree<T>* m_last_child;
    ckTree<T>* m_prev;
    ckTree<T>* m_next;
};
