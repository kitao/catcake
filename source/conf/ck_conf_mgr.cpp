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


#include "pg_conf_all.h"

#include "pg_res_all.h"
#include "pg_private_macro.h"


pgConfMgr* pgConfMgr::m_instance = NULL;


PG_DEFINE_MANAGER_IS_CREATED(pgConfMgr)


PG_DEFINE_MANAGER_CREATE(pgConfMgr, AfterRes, BeforeRes)


PG_DEFINE_MANAGER_DESTROY(pgConfMgr, BeforeRes)


bool pgConfMgr::hasConfig(pgID id)
{
    pgConfMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_conf_map.getN(id) ? true : false;
}


pgConf* pgConfMgr::getConfig(pgID id)
{
    pgConfMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgConf** conf = ins->m_conf_map.getN(id);

    if (!conf)
    {
        pgThrow(ExceptionNotFound);
    }

    return *conf;
}


pgConf* pgConfMgr::newConfig(pgID id, const void* data, u32 data_size)
{
    pgConfMgr* ins = instance();

    if (id == pgID::ZERO || !data || data_size == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (ins->m_conf_map.getN(id))
    {
        pgThrow(ExceptionSameIDExists);
    }

    return pgNew(pgConf)(id, data, data_size);
}


void pgConfMgr::deleteConfig(pgID id)
{
    instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgDelete(getConfig(id), pgConf);
}


pgConf* pgConfMgr::getFirstConfigN()
{
    pgConfMgr* ins = instance();
    const pgID* id = ins->m_conf_map.getFirstKeyN();

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


pgConf* pgConfMgr::getLastConfigN()
{
    pgConfMgr* ins = instance();
    const pgID* id = ins->m_conf_map.getLastKeyN();

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


pgConfMgr::pgConfMgr()
{
    m_conf_map.init(CONFIG_HASH_SIZE);

    pgResMgr::addType("PGC", configInitializer, configFinalizer);
}


pgConfMgr::~pgConfMgr()
{
    while (const pgConf* conf = getFirstConfigN())
    {
        deleteConfig(conf->m_id);
    }

    pgResMgr::removeType("PGC");
}


PG_DEFINE_OPERATOR_EQUAL(pgConfMgr)


PG_DEFINE_MANAGER_INSTANCE(pgConfMgr)


void pgConfMgr::configInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    newConfig(id, data, data_size);
}


void pgConfMgr::configFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    deleteConfig(id);
}
