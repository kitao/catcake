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
    A tree container which can have a parent and children.
    @tparam T The type which is stored in the tree.
*/
template<class T> class ckTree
{
public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    ckDefineException(ExceptionNotInitialized);
    //! @endcond

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
