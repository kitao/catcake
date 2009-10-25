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


template<class T> void assertAlone(pgTree<T>& tree)
{
    /*
        bool hasParent() const
        pgTree<T>* getParentN() const
        pgTree<T>* getPrevAllN() const
        pgTree<T>* getNextAllN() const
        pgTree<T>* getPrevSiblingN() const
        pgTree<T>* getNextSiblingN() const
        bool hasChild() const
        pgTree<T>* getFirstChildN() const
        pgTree<T>* getLastChildN() const
        pgTree<T>* getLastDescendant() const
    */

    pgAssert(!tree.hasParent() && !tree.getParentN());
    pgAssert(!tree.getPrevAllN() && !tree.getNextAllN());
    pgAssert(!tree.hasChild() && !tree.getFirstChildN() && !tree.getLastChildN());
    pgAssert(tree.getLastDescendant() == &tree);

    pgAssertThrow(tree.getPrevSiblingN(), typename pgTree<T>::ExceptionInvalidCall);
    pgAssertThrow(tree.getNextSiblingN(), typename pgTree<T>::ExceptionInvalidCall);
}


template<class T> void assertChild(pgTree<T>& parent)
{
    pgAssert(!parent.hasChild() && !parent.getFirstChildN() && !parent.getLastChildN());
    pgAssert(parent.getLastDescendant() == &parent);
}


template<class T> void assertChild(pgTree<T>& parent, pgTree<T>& child)
{
    pgAssert(parent.hasChild() && parent.getFirstChildN() == &child && parent.getLastChildN() == &child);
    pgAssert(child.hasParent() && child.getParentN() == &parent && !child.getPrevSiblingN() && !child.getNextSiblingN());
}


template<class T> void assertChild(pgTree<T>& parent, pgTree<T>& child1, pgTree<T>& child2)
{
    pgAssert(parent.hasChild() && parent.getFirstChildN() == &child1 && parent.getLastChildN() == &child2);
    pgAssert(child1.hasParent() && child1.getParentN() == &parent && !child1.getPrevSiblingN() && child1.getNextSiblingN() == &child2);
    pgAssert(child2.hasParent() && child2.getParentN() == &parent && child2.getPrevSiblingN() == &child1 && !child2.getNextSiblingN());
}


template<class T> void assertChild(pgTree<T>& parent, pgTree<T>& child1, pgTree<T>& child2, pgTree<T>& child3)
{
    pgAssert(parent.hasChild() && parent.getFirstChildN() == &child1 && parent.getLastChildN() == &child3);
    pgAssert(child1.hasParent() && child1.getParentN() == &parent && !child1.getPrevSiblingN() && child1.getNextSiblingN() == &child2);
    pgAssert(child2.hasParent() && child2.getParentN() == &parent && child2.getPrevSiblingN() == &child1 && child2.getNextSiblingN() == &child3);
    pgAssert(child3.hasParent() && child3.getParentN() == &parent && child3.getPrevSiblingN() == &child2 && !child3.getNextSiblingN());
}


template<class T> void assertOrder(pgTree<T>& tree1, pgTree<T>& tree2)
{
    pgAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    pgAssert(tree2.getPrevAllN() == &tree1 && !tree2.getNextAllN());
}


template<class T> void assertOrder(pgTree<T>& tree1, pgTree<T>& tree2, pgTree<T>& tree3)
{
    pgAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    pgAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    pgAssert(tree3.getPrevAllN() == &tree2 && !tree3.getNextAllN());
}


template<class T> void assertOrder(pgTree<T>& tree1, pgTree<T>& tree2, pgTree<T>& tree3, pgTree<T>& tree4)
{
    pgAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    pgAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    pgAssert(tree3.getPrevAllN() == &tree2 && tree3.getNextAllN() == &tree4);
    pgAssert(tree4.getPrevAllN() == &tree3 && !tree4.getNextAllN());
}


template<class T> void assertOrder(pgTree<T>& tree1, pgTree<T>& tree2, pgTree<T>& tree3, pgTree<T>& tree4, pgTree<T>& tree5)
{
    pgAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    pgAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    pgAssert(tree3.getPrevAllN() == &tree2 && tree3.getNextAllN() == &tree4);
    pgAssert(tree4.getPrevAllN() == &tree3 && tree4.getNextAllN() == &tree5);
    pgAssert(tree5.getPrevAllN() == &tree4 && !tree5.getNextAllN());
}


template<class T> void assertOrder(pgTree<T>& tree1, pgTree<T>& tree2, pgTree<T>& tree3, pgTree<T>& tree4, pgTree<T>& tree5, pgTree<T>& tree6)
{
    pgAssert(!tree1.getPrevAllN() && tree1.getNextAllN() == &tree2);
    pgAssert(tree2.getPrevAllN() == &tree1 && tree2.getNextAllN() == &tree3);
    pgAssert(tree3.getPrevAllN() == &tree2 && tree3.getNextAllN() == &tree4);
    pgAssert(tree4.getPrevAllN() == &tree3 && tree4.getNextAllN() == &tree5);
    pgAssert(tree5.getPrevAllN() == &tree4 && tree5.getNextAllN() == &tree6);
    pgAssert(tree6.getPrevAllN() == &tree5 && !tree6.getNextAllN());
}


template<class T> void assertLastDescendant(pgTree<T>& tree1, pgTree<T>& tree2)
{
    pgAssert(tree1.getLastDescendant() == &tree2);
}


void pgTreeTest()
{
    /*
        pgTree()
        ~pgTree()
        void init(T* self)
        T* getSelf() const
        void addFirst(pgTree<T>* child)
    */
    {
        pgTree<u32> tree1;
        assertAlone(tree1);

        pgAssertThrow(tree1.init(NULL), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.getSelf(), pgTree<u32>::ExceptionNotInitialized);

        u32 v1 = 1;

        tree1.init(&v1);
        pgAssert(tree1.getSelf() == &v1);

        {
            pgTree<u32> tree2;
            u32 v2 = 2;

            tree2.init(&v2);
            pgAssert(tree2.getSelf() == &v2);

            tree1.addFirst(&tree2);
            assertChild(tree1, tree2);
            assertChild(tree2);
            assertOrder(tree1, tree2);
            assertLastDescendant(tree1, tree2);
            assertLastDescendant(tree2, tree2);

            pgAssertThrow(tree2.addFirst(NULL), pgTree<u32>::ExceptionInvalidArgument);
            pgAssertThrow(tree2.addFirst(&tree2), pgTree<u32>::ExceptionInvalidArgument);
            pgAssertThrow(tree2.addFirst(&tree1), pgTree<u32>::ExceptionInvalidArgument);
        }

        assertAlone(tree1);
        assertLastDescendant(tree1, tree1);
    }

    /*
        void addLast(pgTree<T>* child)
    */
    {
        pgTree<u32> tree2;

        {
            pgTree<u32> tree1;

            tree1.addLast(&tree2);
            assertChild(tree1, tree2);
            assertChild(tree2);
            assertOrder(tree1, tree2);
            assertLastDescendant(tree1, tree2);
            assertLastDescendant(tree2, tree2);

            pgAssertThrow(tree2.addLast(NULL), pgTree<u32>::ExceptionInvalidArgument);
            pgAssertThrow(tree2.addLast(&tree2), pgTree<u32>::ExceptionInvalidArgument);
            pgAssertThrow(tree2.addLast(&tree1), pgTree<u32>::ExceptionInvalidArgument);
        }

        assertAlone(tree2);
        assertLastDescendant(tree2, tree2);
    }

    /*
        void leave()
        void clear()
    */
    {
        pgTree<u32> tree1, tree2, tree3, tree4, tree5, tree6;

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
        void joinBefore(pgTree<T>* tree)
        void joinAfter(pgTree<T>* tree)
    */
    {
        pgTree<u32> tree1, tree2, tree3, tree4, tree5, tree6;

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

        pgAssertThrow(tree1.joinBefore(NULL), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.joinBefore(&tree1), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.joinBefore(&tree5), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.joinBefore(&tree3), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.joinAfter(NULL), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.joinAfter(&tree1), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree2.joinAfter(&tree5), pgTree<u32>::ExceptionInvalidArgument);
        pgAssertThrow(tree1.joinAfter(&tree3), pgTree<u32>::ExceptionInvalidArgument);
    }
}
