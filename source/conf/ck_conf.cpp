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

#include "ck_sys_all.h"
#include "ck_util_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckConf* ckConf::getPrevN() const
{
    ckConfMgr* ins = ckConfMgr::instance();
    const ckID* id = ins->m_conf_map.getPrevKeyN(m_id);

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


ckConf* ckConf::getNextN() const
{
    ckConfMgr* ins = ckConfMgr::instance();
    const ckID* id = ins->m_conf_map.getNextKeyN(m_id);

    return id ? *ins->m_conf_map.get(*id) : NULL;
}


ckID ckConf::getID() const
{
    return m_id;
}


bool ckConf::isValid() const
{
    return (m_err_line_no == 0);
}


u16 ckConf::getErrorLineNo() const
{
    if (isValid())
    {
        ckThrow(ExceptionInvalidCall);
    }

    return m_err_line_no;
}


u16 ckConf::getEntryNum(const char* ent_name) const
{
    if (!ent_name)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    u16 ent_num = 0;
    ckID ent_name_id = ckID::genID(ent_name);

    for (ckList<ckEnt>::Item* item = m_ent_list.getFirstN(); item; item = item->getNextN())
    {
        ckEnt* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            ent_num++;
        }
    }

    return ent_num;
}


ckEnt* ckConf::getEntryFromFirstN(const char* ent_name) const
{
    if (!ent_name)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckID ent_name_id = ckID::genID(ent_name);

    for (ckList<ckEnt>::Item* item = m_ent_list.getFirstN(); item; item = item->getNextN())
    {
        ckEnt* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


ckEnt* ckConf::getEntryFromLastN(const char* ent_name) const
{
    if (!ent_name)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckID ent_name_id = ckID::genID(ent_name);

    for (ckList<ckEnt>::Item* item = m_ent_list.getLastN(); item; item = item->getPrevN())
    {
        ckEnt* ent = item->getSelf();

        if (ent->m_name_id == ent_name_id && ent->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


ckEnt* ckConf::getEntryBeforeN(const char* ent_name, ckEnt* ent) const
{
    if (!ent_name || !ent)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckID ent_name_id = ckID::genID(ent_name);

    for (ckList<ckEnt>::Item* item = ent->m_item.getPrevN(); item; item = item->getPrevN())
    {
        ckEnt* ent2 = item->getSelf();

        if (ent2->m_name_id == ent_name_id && ent2->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


ckEnt* ckConf::getEntryAfterN(const char* ent_name, ckEnt* ent) const
{
    if (!ent_name || !ent)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckID ent_name_id = ckID::genID(ent_name);

    for (ckList<ckEnt>::Item* item = ent->m_item.getNextN(); item; item = item->getNextN())
    {
        ckEnt* ent2 = item->getSelf();

        if (ent2->m_name_id == ent_name_id && ent2->m_name == ent_name)
        {
            return item->getSelf();
        }
    }

    return NULL;
}


ckEnt* ckConf::getFirstEntryN() const
{
    ckList<ckEnt>::Item* first = m_ent_list.getFirstN();

    return first ? first->getSelf() : NULL;
}


ckEnt* ckConf::getLastEntryN() const
{
    ckList<ckEnt>::Item* last = m_ent_list.getLastN();

    return last ? last->getSelf() : NULL;
}


static void* catcakeMalloc(u32 size)
{
    return ckMalloc(size);
}


static void catcakeFree(void* ptr)
{
    ckFree(ptr);
}


ckConf* ckConf::newPriorConfigBeforeInitialization(const char* filename)
{
    if (ckMemMgr::isCreated())
    {
        ckThrow(ExceptionInvalidCall);
    }

    if (!filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    void* fh;

    ckTry
    {
        fh = ckSysMgr::openFile(filename, ckSysMgr::FILE_MODE_READ);
    }
    ckCatch(ckSysMgr::ExceptionCannotOpenFile)
    {
        ckThrow(ExceptionCannotOpenFile);
    }

    u32 data_size;
    void* data = NULL;

    ckTry
    {
        data_size = ckSysMgr::getFileSize(fh);

        if (data_size == 0)
        {
            ckThrow(ExceptionCannotReadFile);
        }

        data = ckLowLevelAPI::malloc(data_size);

        ckSysMgr::readFile(data, 0, data_size, fh);
    }
    ckCatch(ckSysMgr::ExceptionCannotReadFile)
    {
        ckSysMgr::closeFile(fh);

        if (data)
        {
            ckFree(data);
        }

        ckThrow(ExceptionCannotReadFile);
    }

    ckSysMgr::closeFile(fh);

    ckConf* conf = new(ckLowLevelAPI::malloc(sizeof(ckConf)), NULL) ckConf(ckID::genID(ckUtil::getBasename(filename)), data, data_size, NULL);

    ckLowLevelAPI::free(data);

    return conf;
}


void ckConf::deletePriorConfig(ckConf* conf)
{
    if (!conf)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (conf->m_malloc == catcakeMalloc)
    {
        ckThrow(ExceptionInvalidCall);
    }

    conf->~ckConf();
    ckLowLevelAPI::free(conf);
}


ckConf::ckConf(ckID id, const void* data, u32 data_size)
{
    m_id = id;
    m_err_line_no = 0;
    m_malloc = catcakeMalloc;
    m_free = catcakeFree;

    parse(data, data_size);

    ckConfMgr::instance()->m_conf_map.add(m_id, this);
}


ckConf::ckConf(ckID id, const void* data, u32 data_size, void*)
{
    m_id = id;
    m_err_line_no = 0;
    m_malloc = ckLowLevelAPI::malloc;
    m_free = ckLowLevelAPI::free;

    parse(data, data_size);
}


ckConf::~ckConf()
{
    if (m_malloc == catcakeMalloc)
    {
        ckConfMgr::instance()->m_conf_map.remove(m_id);
    }

    while (ckList<ckEnt>::Item* item = m_ent_list.getFirstN())
    {
        deleteEntry(item->getSelf());
    }
}


CK_DEFINE_OPERATOR_EQUAL(ckConf)


ckEnt* ckConf::newEntry(const char* ent_name, u16 val_num)
{
    ckType<u8, ckEnt::ValueType>* val_type = static_cast<ckType<u8, ckEnt::ValueType>*>(m_malloc(sizeof(ckType<u8, ckEnt::ValueType>) * val_num));
    ckEnt::Value* val = static_cast<ckEnt::Value*>(m_malloc(sizeof(ckEnt::Value) * val_num));
    ckEnt* ent = new(m_malloc(sizeof(ckEnt)), NULL) ckEnt(ent_name, val_num, val_type, val);

    m_ent_list.addLast(&ent->m_item);

    return ent;
}


void ckConf::deleteEntry(ckEnt* ent)
{
    ent->~ckEnt();

    for (s32 i = 0; i < ent->m_val_num; i++)
    {
        if (ent->m_val_type[i] == ckEnt::TYPE_STRING)
        {
            m_free(const_cast<char*>(ent->m_val[i].val_str));
        }
    }

    m_free(ent->m_val_type);
    m_free(ent->m_val);
    m_free(ent);
}
