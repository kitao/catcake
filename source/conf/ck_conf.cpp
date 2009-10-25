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

#include "pg_sys_all.h"
#include "pg_util_all.h"
#include "pg_low_level_api.h"
#include "pg_private_macro.h"


pgConf* pgConf::getPrevN() const
{
    pgConfMgr* ins = pgConfMgr::instance();
    const pgID* id = ins->m_conf_map.getPrevKeyN(m_id);

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


pgConf* pgConf::getNextN() const
{
    pgConfMgr* ins = pgConfMgr::instance();
    const pgID* id = ins->m_conf_map.getNextKeyN(m_id);

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


pgID pgConf::getID() const
{
    return m_id;
}


bool pgConf::isValid() const
{
    return (m_err_line_no == 0);
}


u16 pgConf::getErrorLineNo() const
{
    if (isValid())
    {
        pgThrow(ExceptionInvalidCall);
    }

    return m_err_line_no;
}


u16 pgConf::getEntryNum(const char* ent_name) const
{
    if (!ent_name)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    u16 ent_num = 0;
    pgID ent_name_id = pgID::genID(ent_name);

    for (pgList<pgEnt>::Item* item = m_ent_list.getFirstN(); item; item = item->getNextN())
    {
        pgEnt* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            ent_num++;
        }
    }

    return ent_num;
}


pgEnt* pgConf::getEntryFromFirstN(const char* ent_name) const
{
    if (!ent_name)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgID ent_name_id = pgID::genID(ent_name);

    for (pgList<pgEnt>::Item* item = m_ent_list.getFirstN(); item; item = item->getNextN())
    {
        pgEnt* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


pgEnt* pgConf::getEntryFromLastN(const char* ent_name) const
{
    if (!ent_name)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgID ent_name_id = pgID::genID(ent_name);

    for (pgList<pgEnt>::Item* item = m_ent_list.getLastN(); item; item = item->getPrevN())
    {
        pgEnt* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


pgEnt* pgConf::getEntryBeforeN(const char* ent_name, pgEnt* ent) const
{
    if (!ent_name || !ent)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgID ent_name_id = pgID::genID(ent_name);

    for (pgList<pgEnt>::Item* item = ent->m_item.getPrevN(); item; item = item->getPrevN())
    {
        pgEnt* ent2 = item->getSelf();

        if (ent2->m_name_id == ent_name_id && ent2->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


pgEnt* pgConf::getEntryAfterN(const char* ent_name, pgEnt* ent) const
{
    if (!ent_name || !ent)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgID ent_name_id = pgID::genID(ent_name);

    for (pgList<pgEnt>::Item* item = ent->m_item.getNextN(); item; item = item->getNextN())
    {
        pgEnt* ent2 = item->getSelf();

        if (ent2->m_name_id == ent_name_id && ent2->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


pgEnt* pgConf::getFirstEntryN() const
{
    pgList<pgEnt>::Item* first = m_ent_list.getFirstN();

    return first ? first->getSelf() : NULL;
}


pgEnt* pgConf::getLastEntryN() const
{
    pgList<pgEnt>::Item* last = m_ent_list.getLastN();

    return last ? last->getSelf() : NULL;
}


static void* pogolynMalloc(u32 size)
{
    return pgMalloc(size);
}


static void pogolynFree(void* ptr)
{
    pgFree(ptr);
}


pgConf* pgConf::newPriorConfigBeforeInitialization(const char* filename)
{
    if (pgMemMgr::isCreated())
    {
        pgThrow(ExceptionInvalidCall);
    }

    if (!filename)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    void* fh;

    pgTry
    {
        fh = pgSysMgr::openFile(filename, pgSysMgr::FILE_MODE_READ);
    }
    pgCatch(pgSysMgr::ExceptionCannotOpenFile)
    {
        pgThrow(ExceptionCannotOpenFile);
    }

    u32 data_size;
    void* data = NULL;

    pgTry
    {
        data_size = pgSysMgr::getFileSize(fh);

        if (data_size == 0)
        {
            pgThrow(ExceptionCannotReadFile);
        }

        data = pgLowLevelAPI::malloc(data_size);

        pgSysMgr::readFile(data, 0, data_size, fh);
    }
    pgCatch(pgSysMgr::ExceptionCannotReadFile)
    {
        pgSysMgr::closeFile(fh);

        if (data)
        {
            pgFree(data);
        }

        pgThrow(ExceptionCannotReadFile);
    }

    pgSysMgr::closeFile(fh);

    pgConf* conf = new(pgLowLevelAPI::malloc(sizeof(pgConf)), NULL) pgConf(pgID::genID(pgUtil::getBasename(filename)), data, data_size, NULL);

    pgLowLevelAPI::free(data);

    return conf;
}


void pgConf::deletePriorConfig(pgConf* conf)
{
    if (!conf)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    if (conf->m_malloc == pogolynMalloc)
    {
        pgThrow(ExceptionInvalidCall);
    }

    conf->~pgConf();
    pgLowLevelAPI::free(conf);
}


pgConf::pgConf(pgID id, const void* data, u32 data_size)
{
    m_id = id;
    m_err_line_no = 0;
    m_malloc = pogolynMalloc;
    m_free = pogolynFree;

    parse(data, data_size);

    pgConfMgr::instance()->m_conf_map.add(m_id, this);
}


pgConf::pgConf(pgID id, const void* data, u32 data_size, void*)
{
    m_id = id;
    m_err_line_no = 0;
    m_malloc = pgLowLevelAPI::malloc;
    m_free = pgLowLevelAPI::free;

    parse(data, data_size);
}


pgConf::~pgConf()
{
    if (m_malloc == pogolynMalloc)
    {
        pgConfMgr::instance()->m_conf_map.remove(m_id);
    }

    while (pgList<pgEnt>::Item* item = m_ent_list.getFirstN())
    {
        deleteEntry(item->getSelf());
    }
}


PG_DEFINE_OPERATOR_EQUAL(pgConf)


pgEnt* pgConf::newEntry(const char* ent_name, u16 val_num)
{
    pgType<u8, pgEnt::ValueType>* val_type = static_cast<pgType<u8, pgEnt::ValueType>*>(m_malloc(sizeof(pgType<u8, pgEnt::ValueType>) * val_num));
    pgEnt::Value* val = static_cast<pgEnt::Value*>(m_malloc(sizeof(pgEnt::Value) * val_num));
    pgEnt* ent = new(m_malloc(sizeof(pgEnt)), NULL) pgEnt(ent_name, val_num, val_type, val);

    m_ent_list.addLast(&ent->m_item);

    return ent;
}


void pgConf::deleteEntry(pgEnt* ent)
{
    ent->~pgEnt();

    for (s32 i = 0; i < ent->m_val_num; i++)
    {
        if (ent->m_val_type[i] == pgEnt::TYPE_STRING)
        {
            m_free(const_cast<char*>(ent->m_val[i].val_str));
        }
    }

    m_free(ent->m_val_type);
    m_free(ent->m_val);
    m_free(ent);
}
