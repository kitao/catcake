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


static u8 s_render_count1;
static u8 s_render_count2;


class RendTestClass1 : public ckRend
{
public:
    RendTestClass1()
    {
        ckAssertThrow(getPrim(), ckRend::ExceptionNotInitialized);
        ckAssertThrow(getRendBody<RendBody>(), ckRend::ExceptionNotInitialized);
        ckAssertThrow(getRendData<RendData>(0), ckRend::ExceptionNotInitialized);
    }

    void init(ckPrim* prim, r32 key)
    {
        /*
            void init(ckPrim* prim, u32 body_size, u32 data_size)
            ckPrim* getPrim() const
            template<class T> T* getRendBody()
            template<class T> T* getRendData(u16 index)
        */
        ckAssertThrow(ckRend::init(NULL, 0, 0), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(ckRend::init(prim, 0, 0, NULL), ckRend::ExceptionInvalidCall);

        ckRend::init(prim, sizeof(RendBody), sizeof(RendData));

        ckAssert(getPrim() == prim);

        RendBody* rend_body = getRendBody<RendBody>();

        rend_body->vec.set(key, key + 1.0f, key + 2.0f);

        for (s32 i = 0; i < prim->getMaxDataNum(); i++)
        {
            RendData* rend_data = getRendData<RendData>(i);
            ckAssert(isEqual(rend_data->vec, ckVec(1.0f, 2.0f, 3.0f)));
        }

        ckAssertThrow(getRendData<RendData>(prim->getMaxDataNum()), ckRend::ExceptionInvalidArgument);
    }

    void init(ckPrim* prim, void* rend_data)
    {
        /*
            void init(ckPrim* prim, u32 body_size, u32 data_size, void* rend_data)
        */
        ckAssertThrow(ckRend::init(NULL, 0, 1, rend_data), ckRend::ExceptionInvalidArgument);
        ckAssertThrow(ckRend::init(prim, 0, 0), ckRend::ExceptionInvalidCall);
        ckAssertThrow(ckRend::init(prim, 0, 1, NULL), ckRend::ExceptionInvalidArgument);

        ckRend::init(prim, sizeof(RendBody), sizeof(RendData), rend_data);
    }

    ckVec& bodyVec()
    {
        return getRendBody<RendBody>()->vec;
    }

    ckVec& dataVec(u8 index)
    {
        return getRendData<RendData>(index)->vec;
    }

private:
    struct RendBody
    {
        ckVec vec;
    };

    struct RendData
    {
        ckVec vec;
    };

    virtual ckID getClassID()
    {
        return ckID_("RendTestClass1");
    }

    virtual void initData(void* data, u16 data_num)
    {
        RendData* rend_data = static_cast<RendData*>(data);

        for (s32 i = 0; i < data_num; i++)
        {
            rend_data[i].vec.set(1.0f, 2.0f, 3.0f);
        }
    }

    virtual void render(const ckMat& view)
    {
        s_render_count1++;
    }
};


class RendTestClass2 : public ckRend
{
public:
    void init(ckPrim* prim)
    {
        ckRend::init(prim, 0, 0);
    }

private:
    virtual ckID getClassID()
    {
        return ckID_("RendTestClass2");
    }

    virtual void initData(void* data, u16 data_num) {}

    virtual void render(const ckMat& view)
    {
        s_render_count2++;
    }
};


void ckRendTest()
{
    /*
        virtual ~ckRend()
        bool isEnabled() const
        void setEnabled(bool is_enabled)

        ckRend()
    */
    {
        s_render_count1 = 0;
        s_render_count2 = 0;

        ckDrawMgr::createAfterRes();

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        {
            ckPrim prim;
            RendTestClass1 rend1;

            ckAssertThrow(rend1.init(&prim, 0.0f), ckPrim::ExceptionNotInitialized);

            ckAssertThrow(rend1.isEnabled(), ckRend::ExceptionNotInitialized);
            ckAssertThrow(rend1.setEnabled(false), ckRend::ExceptionNotInitialized);

            prim.init(ckPrim::MODE_TRIANGLE_STRIP, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 0 && s_render_count2 == 0);

            rend1.init(&prim, 10.0f);
            rend1.init(&prim, 20.0f);

            ckAssert(rend1.isEnabled());

            rend1.dataVec(2);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 1 && s_render_count2 == 0);

            ckAssertThrow(rend1.dataVec(3), ckRend::ExceptionInvalidArgument);

            rend1.setEnabled(false);
            ckAssert(!rend1.isEnabled());

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 1 && s_render_count2 == 0);

            prim.init(ckPrim::MODE_TRIANGLE_STRIP, 5, ckDrawMgr::DEFAULT_2D_SCREEN_ID);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 1 && s_render_count2 == 0);

            ckAssertThrow(rend1.isEnabled(), ckRend::ExceptionNotInitialized);
            ckAssertThrow(rend1.setEnabled(false), ckRend::ExceptionNotInitialized);

            rend1.init(&prim, 30.0f);

            ckAssert(rend1.isEnabled());

            rend1.dataVec(4);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 2 && s_render_count2 == 0);

            ckAssertThrow(rend1.dataVec(5), ckRend::ExceptionInvalidArgument);

            RendTestClass2 rend2;
            rend2.init(&prim);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 2 && s_render_count2 == 1);

            rend1.init(&prim, 40.0f);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 3 && s_render_count2 == 1);

            ckPrim::PrimData prim_data[3];
            ckVec rend_data[3];

            prim.init(ckPrim::MODE_TRIANGLE_STRIP, prim_data, 3, ckDrawMgr::DEFAULT_3D_SCREEN_ID);

            rend1.init(&prim, rend_data);
            rend1.init(&prim, rend_data);
        }

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckDrawMgr::destroyBeforeRes();
    }

    /*
        void reallocData(u16 max_data_num);
        void copyData(u16 dest_index, const ckPrim* src_prim, u16 src_index);
    */
    {
        s_render_count1 = 0;
        s_render_count2 = 0;

        ckDrawMgr::createAfterRes();

        u32 used_memory_size = ckMemMgr::getCurUsedMemorySize();

        RendTestClass1 rend1, rend2;
        RendTestClass2 rend3;

        {
            ckPrim prim1;

            prim1.init(ckPrim::MODE_TRIANGLE_STRIP, 10, ckDrawMgr::DEFAULT_3D_SCREEN_ID);
            rend1.init(&prim1, 123.0f);

            for (s32 i = 0; i < 10; i++)
            {
                rend1.dataVec(i).set(1.0f + i, 2.0f + i, 3.0f + i);
            }

            prim1.reallocData(20);

            for (s32 i = 0; i < 20; i++)
            {
                if (i < 10)
                {
                    ckAssert(isEqual(rend1.dataVec(i), ckVec(1.0f + i, 2.0f + i, 3.0f + i)));
                }
                else
                {
                    ckAssert(isEqual(rend1.dataVec(i), ckVec(1.0f, 2.0f, 3.0)));
                }
            }

            prim1.reallocData(5);

            for (s32 i = 0; i < 5; i++)
            {
                ckAssert(isEqual(rend1.dataVec(i), ckVec(1.0f + i, 2.0f + i, 3.0f + i)));
            }

            prim1.reallocData(10);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 1 && s_render_count2 == 0);

            ckPrim prim2;

            prim2.init(ckPrim::MODE_POINTS, 20, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
            rend2.init(&prim2, 234.0f);

            for (s32 i = 0; i < 5; i++)
            {
                ckAssert(isEqual(rend1.dataVec(i + 5), ckVec(1.0f, 2.0f, 3.0)));

                prim1.copyData(i + 5, &prim1, i);
                ckAssert(isEqual(rend1.dataVec(i), rend1.dataVec(i + 5)));

                prim2.copyData(i + 10, &prim1, i);
                ckAssert(isEqual(rend1.dataVec(i), rend2.dataVec(i + 10)));
            }

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 3 && s_render_count2 == 0);

            rend3.init(&prim2);

            ckDrawMgr::renderForSystem();
            ckAssert(s_render_count1 == 4 && s_render_count2 == 1);

            ckAssertThrow(prim1.copyData(0, &prim2, 0), ckRend::ExceptionInvalidArgument);
        }

        ckAssert(ckMemMgr::getCurUsedMemorySize() == used_memory_size);

        ckDrawMgr::destroyBeforeRes();
    }
}
