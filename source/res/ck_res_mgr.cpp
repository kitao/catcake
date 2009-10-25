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


#include "pg_res_all.h"

#include "pg_sys_all.h"
#include "pg_util_all.h"
#include "pg_private_macro.h"


pgResMgr* pgResMgr::m_instance = NULL;


PG_DEFINE_MANAGER_IS_CREATED(pgResMgr)


PG_DEFINE_MANAGER_CREATE(pgResMgr, AfterTask, BeforeSys)


PG_DEFINE_MANAGER_DESTROY(pgResMgr, BeforeSys)


u16 pgResMgr::getTypeNum()
{
    return instance()->m_type_info_map.getDataNum();
}


u16 pgResMgr::getResourceNum()
{
    return instance()->m_res_map.getDataNum();
}


void pgResMgr::addType(pgStr<char, 3> ext, Initializer init, Finalizer final)
{
    pgResMgr* ins = instance();

    if (ext == "" || !init || !final)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ext = pgRes::toUppercase(ext);

    if (ins->m_type_info_map.getN(ext))
    {
        pgThrow(ExceptionSameExtensionExists);
    }

    TypeInfo ti;
    ti.ext = ext;
    ti.init = init;
    ti.final = final;

    ins->m_type_info_map.add(ext, ti);
}


void pgResMgr::removeType(pgStr<char, 3> ext)
{
    pgResMgr* ins = instance();

    if (ext == "")
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ext = pgRes::toUppercase(ext);

    pgTry
    {
        ins->m_type_info_map.remove(ext);
    }
    pgCatch(pgMap<pgStr<char, 3>, TypeInfo>::ExceptionNotFound)
    {
        pgThrow(ExceptionNotFound);
    }
}


bool pgResMgr::hasResource(pgID id)
{
    pgResMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_res_map.getN(id) ? true : false;
}


pgRes pgResMgr::getResource(pgID id)
{
    pgResMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes* res = ins->m_res_map.getN(id);

    if (!res)
    {
        pgThrow(ExceptionNotFound);
    }

    return *res;
}


void pgResMgr::addResource(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free)
{
    pgResMgr* ins = instance();

    if (id == pgID::ZERO || !data || data_size == 0)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    ext = pgRes::toUppercase(ext);

    pgTry
    {
        pgRes res;
        res.init(id, ext, data, data_size, NULL, is_auto_free);

        ins->m_res_map.add(id, res);
    }
    pgCatch(pgMap<pgID, pgRes>::ExceptionSameKeyExists)
    {
        pgThrow(ExceptionSameIDExists);
    }

    TypeInfo* ti = ins->m_type_info_map.getN(ext);

    if (ti)
    {
        pgRes* res = ins->m_res_map.get(id);

        (*ti->init)(res->m_id, res->m_ext, res->m_data, res->m_data_size, &res->m_exinfo);
    }
}


void pgResMgr::removeResource(pgID id)
{
    pgResMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    pgRes* res;

    pgTry
    {
        res = ins->m_res_map.get(id);
    }
    pgCatch(pgMap<pgID, pgRes>::ExceptionNotFound)
    {
        pgThrow(ExceptionNotFound);
    }

    TypeInfo* ti = ins->m_type_info_map.getN(res->m_ext);

    if (ti)
    {
        (*ti->final)(res->m_id, res->m_ext, res->m_data, res->m_data_size, res->m_exinfo);
    }

    if (res->m_is_auto_free)
    {
        pgFree(const_cast<void*>(res->m_data));
    }

    ins->m_res_map.remove(id);
}


void pgResMgr::loadResource(const char* filename, bool is_type_detect)
{
    instance();

    if (!filename)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return loadResourceAs(pgID::genID(pgUtil::getBasename(filename)), filename, is_type_detect);
}


void pgResMgr::loadResourceAs(pgID id, const char* filename, bool is_type_detect)
{
    instance();

    if (id == pgID::ZERO || !filename)
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

        data = pgMalloc(data_size);

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

    pgTry
    {
        addResource(id, is_type_detect ? pgUtil::getExtension(filename) : "", data, data_size, true);
    }
    pgCatch(ExceptionSameIDExists)
    {
        pgFree(data);

        pgThrow(ExceptionSameIDExists);
    }
}


const pgStr<char, 3>* pgResMgr::getFirstTypeN()
{
    return instance()->m_type_info_map.getFirstKeyN();
}


const pgStr<char, 3>* pgResMgr::getNextTypeN(pgStr<char, 3> ext)
{
    pgResMgr* ins = instance();

    if (ext == "")
    {
        pgThrow(ExceptionInvalidArgument);
    }

    return ins->m_type_info_map.getNextKeyN(ext);
}


const pgRes* pgResMgr::getFirstResourceN()
{
    pgResMgr* ins = instance();
    const pgID* id = ins->m_res_map.getFirstKeyN();

    return id ? ins->m_res_map.get(*id) : NULL;
}


const pgRes* pgResMgr::getNextResourceN(pgID id)
{
    pgResMgr* ins = instance();

    if (id == pgID::ZERO)
    {
        pgThrow(ExceptionInvalidArgument);
    }

    const pgID* id2 = ins->m_res_map.getNextKeyN(id);

    return id2 ? ins->m_res_map.get(*id2) : NULL;
}


pgResMgr::pgResMgr()
{
    m_type_info_map.init(TYPE_HASH_SIZE);
    m_res_map.init(RESOURCE_HASH_SIZE);
}


pgResMgr::~pgResMgr()
{
    while (const pgRes* res = getFirstResourceN())
    {
        removeResource(res->m_id);
    }
}


PG_DEFINE_OPERATOR_EQUAL(pgResMgr)


PG_DEFINE_MANAGER_INSTANCE(pgResMgr)
