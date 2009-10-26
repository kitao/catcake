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

    ckResMgr::addType("CKC", configInitializer, configFinalizer);
}


ckConfMgr::~ckConfMgr()
{
    while (const ckConf* conf = getFirstConfigN())
    {
        deleteConfig(conf->m_id);
    }

    ckResMgr::removeType("CKC");
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
