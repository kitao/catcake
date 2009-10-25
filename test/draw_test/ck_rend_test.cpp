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


class RendTestClass1 : public pgRend
{
public:
    RendTestClass1()
    {
        pgAssertThrow(getPrim(), pgRend::ExceptionNotInitialized);
        pgAssertThrow(getRendBody<RendBody>(), pgRend::ExceptionNotInitialized);
        pgAssertThrow(getRendData<RendData>(0), pgRend::ExceptionNotInitialized);
    }

    void init(pgPrim* prim, r32 key)
    {
        /*
            void init(pgPrim* prim, u32 body_size, u32 data_size)
            pgPrim* getPrim() const
            template<class T> T* getRendBody()
            template<class T> T* getRendData(u16 index)
        */
        pgAssertThrow(pgRend::init(NULL, 0, 0), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(pgRend::init(prim, 0, 0, NULL), pgRend::ExceptionInvalidCall);

        pgRend::init(prim, sizeof(RendBody), sizeof(RendData));

        pgAssert(getPrim() == prim);

        RendBody* rend_body = getRendBody<RendBody>();

        rend_body->vec.set(key, key + 1.0f, key + 2.0f);

        for (s32 i = 0; i < prim->getMaxDataNum(); i++)
        {
            RendData* rend_data = getRendData<RendData>(i);
            pgAssert(isEqual(rend_data->vec, pgVec(1.0f, 2.0f, 3.0f)));
        }

        pgAssertThrow(getRendData<RendData>(prim->getMaxDataNum()), pgRend::ExceptionInvalidArgument);
    }

    void init(pgPrim* prim, void* rend_data)
    {
        /*
            void init(pgPrim* prim, u32 body_size, u32 data_size, void* rend_data)
        */
        pgAssertThrow(pgRend::init(NULL, 0, 1, rend_data), pgRend::ExceptionInvalidArgument);
        pgAssertThrow(pgRend::init(prim, 0, 0), pgRend::ExceptionInvalidCall);
        pgAssertThrow(pgRend::init(prim, 0, 1, NULL), pgRend::ExceptionInvalidArgument);

        pgRend::init(prim, sizeof(RendBody), sizeof(RendData), rend_data);
    }

    pgVec& bodyVec()
    {
        return getRendBody<RendBody>()->vec;
    }

    pgVec& dataVec(u8 index)
    {
        return getRendData<RendData>(index)->vec;
    }

private:
    struct RendBody
    {
        pgVec vec;
    };

    struct RendData
    {
        pgVec vec;
    };

    virtual pgID getClassID()
    {
        return pgID_("RendTestClass1");
    }

    virtual void initData(void* data, u16 data_num)
    {
        RendData* rend_data = static_cast<RendData*>(data);

        for (s32 i = 0; i < data_num; i++)
        {
            rend_data[i].vec.set(1.0f, 2.0f, 3.0f);
        }
    }

    virtual void render(const pgMat& view)
    {
        s_render_count1++;
    }
};


class RendTestClass2 : public pgRend
{
public:
    void init(pgPrim* prim)
    {
        pgRend::init(prim, 0, 0);
    }

private:
    virtual pgID getClassID()
    {
        return pgID_("RendTestClass2");
    }

    virtual void initData(void* data, u16 data_num) {}

    virtual void render(const pgMat& view)
    {
        s_render_count2++;
    }
};


void pgRendTest()
{
    /*
        virtual ~pgRend()
        bool isActive() const
        void setActive(bool is_active)

        pgRend()
    */
    {
        s_render_count1 = 0;
        s_render_count2 = 0;

        pgDrawMgr::createAfterRes();

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        {
            pgPrim prim;
            RendTestClass1 rend1;

            pgAssertThrow(rend1.init(&prim, 0.0f), pgPrim::ExceptionNotInitialized);

            pgAssertThrow(rend1.isActive(), pgRend::ExceptionNotInitialized);
            pgAssertThrow(rend1.setActive(false), pgRend::ExceptionNotInitialized);

            prim.init(pgPrim::MODE_TRIANGLE_STRIP, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 0 && s_render_count2 == 0);

            rend1.init(&prim, 10.0f);
            rend1.init(&prim, 20.0f);

            pgAssert(rend1.isActive());

            rend1.dataVec(2);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 1 && s_render_count2 == 0);

            pgAssertThrow(rend1.dataVec(3), pgRend::ExceptionInvalidArgument);

            rend1.setActive(false);
            pgAssert(!rend1.isActive());

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 1 && s_render_count2 == 0);

            prim.init(pgPrim::MODE_TRIANGLE_STRIP, 5, pgDrawMgr::DEFAULT_2D_SCREEN_ID);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 1 && s_render_count2 == 0);

            pgAssertThrow(rend1.isActive(), pgRend::ExceptionNotInitialized);
            pgAssertThrow(rend1.setActive(false), pgRend::ExceptionNotInitialized);

            rend1.init(&prim, 30.0f);

            pgAssert(rend1.isActive());

            rend1.dataVec(4);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 2 && s_render_count2 == 0);

            pgAssertThrow(rend1.dataVec(5), pgRend::ExceptionInvalidArgument);

            RendTestClass2 rend2;
            rend2.init(&prim);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 2 && s_render_count2 == 1);

            rend1.init(&prim, 40.0f);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 3 && s_render_count2 == 1);

            pgPrim::PrimData prim_data[3];
            pgVec rend_data[3];

            prim.init(pgPrim::MODE_TRIANGLE_STRIP, prim_data, 3, pgDrawMgr::DEFAULT_3D_SCREEN_ID);

            rend1.init(&prim, rend_data);
            rend1.init(&prim, rend_data);
        }

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgDrawMgr::destroyBeforeRes();
    }

    /*
        void reallocData(u16 max_data_num);
        void copyData(u16 dest_index, const pgPrim* src_prim, u16 src_index);
    */
    {
        s_render_count1 = 0;
        s_render_count2 = 0;

        pgDrawMgr::createAfterRes();

        u32 used_memory_size = pgMemMgr::getCurUsedMemorySize();

        RendTestClass1 rend1, rend2;
        RendTestClass2 rend3;

        {
            pgPrim prim1;

            prim1.init(pgPrim::MODE_TRIANGLE_STRIP, 10, pgDrawMgr::DEFAULT_3D_SCREEN_ID);
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
                    pgAssert(isEqual(rend1.dataVec(i), pgVec(1.0f + i, 2.0f + i, 3.0f + i)));
                }
                else
                {
                    pgAssert(isEqual(rend1.dataVec(i), pgVec(1.0f, 2.0f, 3.0)));
                }
            }

            prim1.reallocData(5);

            for (s32 i = 0; i < 5; i++)
            {
                pgAssert(isEqual(rend1.dataVec(i), pgVec(1.0f + i, 2.0f + i, 3.0f + i)));
            }

            prim1.reallocData(10);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 1 && s_render_count2 == 0);

            pgPrim prim2;

            prim2.init(pgPrim::MODE_POINTS, 20, pgDrawMgr::DEFAULT_2D_SCREEN_ID);
            rend2.init(&prim2, 234.0f);

            for (s32 i = 0; i < 5; i++)
            {
                pgAssert(isEqual(rend1.dataVec(i + 5), pgVec(1.0f, 2.0f, 3.0)));

                prim1.copyData(i + 5, &prim1, i);
                pgAssert(isEqual(rend1.dataVec(i), rend1.dataVec(i + 5)));

                prim2.copyData(i + 10, &prim1, i);
                pgAssert(isEqual(rend1.dataVec(i), rend2.dataVec(i + 10)));
            }

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 3 && s_render_count2 == 0);

            rend3.init(&prim2);

            pgDrawMgr::renderForSystem();
            pgAssert(s_render_count1 == 4 && s_render_count2 == 1);

            pgAssertThrow(prim1.copyData(0, &prim2, 0), pgRend::ExceptionInvalidArgument);
        }

        pgAssert(pgMemMgr::getCurUsedMemorySize() == used_memory_size);

        pgDrawMgr::destroyBeforeRes();
    }
}
