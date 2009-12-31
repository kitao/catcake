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


#include "ck_conf_all.h"

#include "ck_res_all.h"
#include "ck_private_macro.h"


ckConfMgr* ckConfMgr::m_instance = NULL;


CK_DEFINE_MANAGER_IS_CREATED(ckConfMgr)


CK_DEFINE_MANAGER_CREATE(ckConfMgr, AfterRes, BeforeRes)


CK_DEFINE_MANAGER_DESTROY(ckConfMgr, BeforeRes)


bool ckConfMgr::hasConfig(ckID id)
{
    ckConfMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_conf_map.getN(id) ? true : false;
}


ckConf* ckConfMgr::getConfig(ckID id)
{
    ckConfMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckConf** conf = ins->m_conf_map.getN(id);

    if (!conf)
    {
        ckThrow(ExceptionNotFound);
    }

    return *conf;
}


ckConf* ckConfMgr::newConfig(ckID id, const void* data, u32 data_size)
{
    ckConfMgr* ins = instance();

    if (id == ckID::ZERO || !data || data_size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ins->m_conf_map.getN(id))
    {
        ckThrow(ExceptionSameIDExists);
    }

    return ckNew(ckConf)(id, data, data_size);
}


void ckConfMgr::deleteConfig(ckID id)
{
    instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckDelete(getConfig(id), ckConf);
}


ckConf* ckConfMgr::getFirstConfigN()
{
    ckConfMgr* ins = instance();
    const ckID* id = ins->m_conf_map.getFirstKeyN();

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


ckConf* ckConfMgr::getLastConfigN()
{
    ckConfMgr* ins = instance();
    const ckID* id = ins->m_conf_map.getLastKeyN();

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


ckConfMgr::ckConfMgr()
{
    m_conf_map.init(CONFIG_HASH_SIZE);

    ckResMgr::addType("CKL", configInitializer, configFinalizer);
}


ckConfMgr::~ckConfMgr()
{
    while (const ckConf* conf = getFirstConfigN())
    {
        deleteConfig(conf->m_id);
    }

    ckResMgr::removeType("CKL");
}


CK_DEFINE_OPERATOR_EQUAL(ckConfMgr)


CK_DEFINE_MANAGER_INSTANCE(ckConfMgr)


void ckConfMgr::configInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    newConfig(id, data, data_size);
}


void ckConfMgr::configFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    deleteConfig(id);
}
