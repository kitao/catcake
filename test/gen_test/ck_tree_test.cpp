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


template<class T> void assertAlone(ckTree<T>& tree)
{
    /*
        bool hasParent() const
        ckTree<T>* getParentN() const
        ckTree<T>* getPrevAllN() const
        ckTree<T>* getNextAllN() const
        ckTree<T>* getPrevSiblingN() const
        ckTree<T>* getNextSiblingN() const
        bool hasChild() const
        ckTree<T>* getFirstChildN() const
        ckTree<T>* getLastChildN() const
        ckTree<T>* getLastDescendant() const
    */

    ckAssert(!tree.hasParent() && !tree.getParentN());
    ckAssert(!tree.getPrevAllN() && !tree.getNextAllN());
    ckAssert(!tree.hasChild() && !tree.getFirstChildN() && !tree.getLastChildN());
    ckAssert(tree.getLastDescendant() == &tree);

    ckAssertThrow(tree.getPrevSiblingN(), typename ckTree<T>::ExceptionInvalidCall);
    ckAssertThrow(tree.getNextSiblingN(), typename ckTree<T>::ExceptionInvalidCall);
}


template<class T> void assertChild(ckTree<T>& parent)
{
    ckAssert(!parent.hasChild() && !parent.getFirstChildN() && !parent.getLastChildN());
    ckAssert(parent.getLastDescendant() == &parent);
}


template<class T> void assertChild(ckTree<T>& parent, ckTree<T>& child)
{
    ckAssert(parent.hasChild() && parent.getFirstChildN() == &child && parent.getLastChildN() == &child);
    ckAssert(child.hasParent() && child.getParentN() == &parent && !child.getPrevSiblingN() && !child.getNextSiblingN());
}


template<class T> void assertChild(ckTree<T>& parent, ckTree<T>& child1, ckTree<T>& child2)
{
    ckAssert(parent.hasChild() && parent.getFirstChildN() == &child1 && parent.getLastChildN() == &child2);
    ckAssert(child1.hasParent() && child1.getParentN() == &parent && !child1.getPrevSiblingN() && child1.getNextSiblingN() == &child2);
    ckAssert(child2.hasParent() && child2.getParentN() == &parent && child2.getPrevSiblingN() == &child1 && !child2.getNextSiblingN());
}


template<class T> void assertChild(ckTree<T>& parent, ckTree<T>& child1, ckTree<T>& child2, ckTree<T>& child3)
{
    ckAssert(parent.hasChild() && parent.getFirstChildN() == &child1 && parent.getLastChildN() == &child3);
    ckAssert(child1.hasParent() && child1.getParentN() == &parent && !child1.getPrevSiblingN() && child1.getNextSiblingN() == &child2);
    ckAssert(child2.hasParent() && child2.getParentN() == &parent && child2.getPrevSiblingN() == &child1 && child2.getNextSiblingN() == &child3);
    ckAssert(child3.hasParent() && child3.getParentN() == &parent && child3.getPrevSiblingN() == &child2 && !child3.getNextSiblingN());
}


template<class T> void assertOrder(ckTree<T>& tree1, ckTree<T>& tree2)
{
    ckAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    ckAssert(tree2.getPrevAllN() == &tree1 && !tree2.getNextAllN());
}


template<class T> void assertOrder(ckTree<T>& tree1, ckTree<T>& tree2, ckTree<T>& tree3)
{
    ckAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    ckAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    ckAssert(tree3.getPrevAllN() == &tree2 && !tree3.getNextAllN());
}


template<class T> void assertOrder(ckTree<T>& tree1, ckTree<T>& tree2, ckTree<T>& tree3, ckTree<T>& tree4)
{
    ckAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    ckAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    ckAssert(tree3.getPrevAllN() == &tree2 && tree3.getNextAllN() == &tree4);
    ckAssert(tree4.getPrevAllN() == &tree3 && !tree4.getNextAllN());
}


template<class T> void assertOrder(ckTree<T>& tree1, ckTree<T>& tree2, ckTree<T>& tree3, ckTree<T>& tree4, ckTree<T>& tree5)
{
    ckAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    ckAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    ckAssert(tree3.getPrevAllN() == &tree2 && tree3.getNextAllN() == &tree4);
    ckAssert(tree4.getPrevAllN() == &tree3 && tree4.getNextAllN() == &tree5);
    ckAssert(tree5.getPrevAllN() == &tree4 && !tree5.getNextAllN());
}


template<class T> void assertOrder(ckTree<T>& tree1, ckTree<T>& tree2, ckTree<T>& tree3, ckTree<T>& tree4, ckTree<T>& tree5, ckTree<T>& tree6)
{
    ckAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    ckAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    ckAssert(tree3.getPrevAllN() == &tree2 && tree3.getNextAllN() == &tree4);
    ckAssert(tree4.getPrevAllN() == &tree3 && tree4.getNextAllN() == &tree5);
    ckAssert(tree5.getPrevAllN() == &tree4 && tree5.getNextAllN() == &tree6);
    ckAssert(tree6.getPrevAllN() == &tree5 && !tree6.getNextAllN());
}


template<class T> void assertLastDescendant(ckTree<T>& tree1, ckTree<T>& tree2)
{
    ckAssert(tree1.getLastDescendant() == &tree2);
}


void ckTreeTest()
{
    /*
        ckTree()
        ~ckTree()
        void init(T* self)
        T* getSelf() const
        void addFirst(ckTree<T>* child)
    */
    {
        ckTree<u32> tree1;
        assertAlone(tree1);

        ckAssertThrow(tree1.init(NULL), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.getSelf(), ckTree<u32>::ExceptionNotInitialized);

        u32 v1 = 1;

        tree1.init(&v1);
        ckAssert(tree1.getSelf() == &v1);

        {
            ckTree<u32> tree2;
            u32 v2 = 2;

            tree2.init(&v2);
            ckAssert(tree2.getSelf() == &v2);

            tree1.addFirst(&tree2);
            assertChild(tree1, tree2);
            assertChild(tree2);
            assertOrder(tree1, tree2);
            assertLastDescendant(tree1, tree2);
            assertLastDescendant(tree2, tree2);

            ckAssertThrow(tree2.addFirst(NULL), ckTree<u32>::ExceptionInvalidArgument);
            ckAssertThrow(tree2.addFirst(&tree2), ckTree<u32>::ExceptionInvalidArgument);
            ckAssertThrow(tree2.addFirst(&tree1), ckTree<u32>::ExceptionInvalidArgument);
        }

        assertAlone(tree1);
        assertLastDescendant(tree1, tree1);
    }

    /*
        void addLast(ckTree<T>* child)
    */
    {
        ckTree<u32> tree2;

        {
            ckTree<u32> tree1;

            tree1.addLast(&tree2);
            assertChild(tree1, tree2);
            assertChild(tree2);
            assertOrder(tree1, tree2);
            assertLastDescendant(tree1, tree2);
            assertLastDescendant(tree2, tree2);

            ckAssertThrow(tree2.addLast(NULL), ckTree<u32>::ExceptionInvalidArgument);
            ckAssertThrow(tree2.addLast(&tree2), ckTree<u32>::ExceptionInvalidArgument);
            ckAssertThrow(tree2.addLast(&tree1), ckTree<u32>::ExceptionInvalidArgument);
        }

        assertAlone(tree2);
        assertLastDescendant(tree2, tree2);
    }

    /*
        void leave()
        void clear()
    */
    {
        ckTree<u32> tree1, tree2, tree3, tree4, tree5, tree6;

        tree1.addFirst(&tree2);
        assertChild(tree1, tree2);
        assertChild(tree2);
        assertOrder(tree1, tree2);
        assertLastDescendant(tree1, tree2);
        assertLastDescendant(tree2, tree2);

        tree2.addLast(&tree3);
        assertChild(tree1, tree2);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertOrder(tree1, tree2, tree3);
        assertLastDescendant(tree1, tree3);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);

        tree4.addFirst(&tree5);
        tree4.addLast(&tree6);
        tree1.addLast(&tree4);
        assertChild(tree1, tree2, tree4);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertChild(tree4, tree5, tree6);
        assertChild(tree5);
        assertChild(tree6);
        assertOrder(tree1, tree2, tree3, tree4, tree5, tree6);
        assertLastDescendant(tree1, tree6);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree6);
        assertLastDescendant(tree5, tree5);
        assertLastDescendant(tree6, tree6);

        tree4.leave();
        assertChild(tree1, tree2);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertOrder(tree1, tree2, tree3);
        assertLastDescendant(tree1, tree3);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);

        tree4.leave();
        assertChild(tree4, tree5, tree6);
        assertChild(tree5);
        assertChild(tree6);
        assertOrder(tree4, tree5, tree6);
        assertLastDescendant(tree4, tree6);
        assertLastDescendant(tree5, tree5);
        assertLastDescendant(tree6, tree6);

        tree1.addFirst(&tree4);
        assertChild(tree1, tree4, tree2);
        assertChild(tree4, tree5, tree6);
        assertChild(tree5);
        assertChild(tree6);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertOrder(tree1, tree4, tree5, tree6, tree2, tree3);
        assertLastDescendant(tree1, tree3);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree6);
        assertLastDescendant(tree5, tree5);
        assertLastDescendant(tree6, tree6);

        tree1.addLast(&tree4);
        assertChild(tree1, tree2, tree4);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertChild(tree4, tree5, tree6);
        assertChild(tree5);
        assertChild(tree6);
        assertOrder(tree1, tree2, tree3, tree4, tree5, tree6);
        assertLastDescendant(tree1, tree6);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree6);
        assertLastDescendant(tree5, tree5);
        assertLastDescendant(tree6, tree6);

        tree1.clear();
        assertAlone(tree1);

        tree1.clear();
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertOrder(tree2, tree3);
        assertChild(tree4, tree5, tree6);
        assertChild(tree5);
        assertChild(tree6);
        assertOrder(tree4, tree5, tree6);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree6);
        assertLastDescendant(tree5, tree5);
        assertLastDescendant(tree6, tree6);
    }

    /*
        void joinBefore(ckTree<T>* tree)
        void joinAfter(ckTree<T>* tree)
    */
    {
        ckTree<u32> tree1, tree2, tree3, tree4, tree5, tree6;

        tree1.addFirst(&tree4);
        tree2.joinBefore(&tree4);
        tree5.joinAfter(&tree4);
        tree2.addFirst(&tree3);
        assertChild(tree1, tree2, tree4, tree5);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertChild(tree4);
        assertChild(tree5);
        assertOrder(tree1, tree2, tree3, tree4, tree5);
        assertLastDescendant(tree1, tree5);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree4);
        assertLastDescendant(tree5, tree5);

        tree5.addLast(&tree6);
        assertChild(tree1, tree2, tree4, tree5);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertChild(tree4);
        assertChild(tree5, tree6);
        assertChild(tree6);
        assertOrder(tree1, tree2, tree3, tree4, tree5, tree6);
        assertLastDescendant(tree1, tree6);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree4);
        assertLastDescendant(tree5, tree6);
        assertLastDescendant(tree6, tree6);

        tree5.joinBefore(&tree4);
        assertChild(tree1, tree2, tree5, tree4);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertChild(tree5, tree6);
        assertChild(tree6);
        assertChild(tree4);
        assertOrder(tree1, tree2, tree3, tree5, tree6, tree4);
        assertLastDescendant(tree1, tree4);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree4);
        assertLastDescendant(tree5, tree6);
        assertLastDescendant(tree6, tree6);

        tree2.joinAfter(&tree4);
        assertChild(tree1, tree5, tree4, tree2);
        assertChild(tree5, tree6);
        assertChild(tree6);
        assertChild(tree4);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertOrder(tree1, tree5, tree6, tree4, tree2, tree3);
        assertLastDescendant(tree1, tree3);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree4);
        assertLastDescendant(tree5, tree6);
        assertLastDescendant(tree6, tree6);

        tree5.leave();
        assertChild(tree1, tree4, tree2);
        assertChild(tree4);
        assertChild(tree2, tree3);
        assertChild(tree3);
        assertOrder(tree1, tree4, tree2, tree3);
        assertChild(tree5, tree6);
        assertChild(tree6);
        assertOrder(tree5, tree6);
        assertLastDescendant(tree1, tree3);
        assertLastDescendant(tree2, tree3);
        assertLastDescendant(tree3, tree3);
        assertLastDescendant(tree4, tree4);
        assertLastDescendant(tree5, tree6);
        assertLastDescendant(tree6, tree6);

        ckAssertThrow(tree1.joinBefore(NULL), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.joinBefore(&tree1), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.joinBefore(&tree5), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.joinBefore(&tree3), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.joinAfter(NULL), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.joinAfter(&tree1), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree2.joinAfter(&tree5), ckTree<u32>::ExceptionInvalidArgument);
        ckAssertThrow(tree1.joinAfter(&tree3), ckTree<u32>::ExceptionInvalidArgument);
    }
}
