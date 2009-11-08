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


#include "ck_res_all.h"

#include "ck_sys_all.h"
#include "ck_util_all.h"
#include "ck_private_macro.h"


ckResMgr* ckResMgr::m_instance = NULL;


CK_DEFINE_MANAGER_IS_CREATED(ckResMgr)


CK_DEFINE_MANAGER_CREATE(ckResMgr, AfterTask, BeforeSys)


CK_DEFINE_MANAGER_DESTROY(ckResMgr, BeforeSys)


u16 ckResMgr::getTypeNum()
{
    return instance()->m_type_info_map.getDataNum();
}


u16 ckResMgr::getResourceNum()
{
    return instance()->m_res_map.getDataNum();
}


void ckResMgr::addType(ckStr<char, 3> ext, Initializer init, Finalizer final)
{
    ckResMgr* ins = instance();

    if (ext == "" || !init || !final)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ext = ckRes::toUppercase(ext);

    if (ins->m_type_info_map.getN(ext))
    {
        ckThrow(ExceptionSameExtensionExists);
    }

    TypeInfo ti;
    ti.ext = ext;
    ti.init = init;
    ti.final = final;

    ins->m_type_info_map.add(ext, ti);
}


void ckResMgr::removeType(ckStr<char, 3> ext)
{
    ckResMgr* ins = instance();

    if (ext == "")
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ext = ckRes::toUppercase(ext);

    ckTry
    {
        ins->m_type_info_map.remove(ext);
    }
    ckCatch(ckMap<ckStr<char, 3>, TypeInfo>::ExceptionNotFound)
    {
        ckThrow(ExceptionNotFound);
    }
}


bool ckResMgr::hasResource(ckID id)
{
    ckResMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_res_map.getN(id) ? true : false;
}


ckRes ckResMgr::getResource(ckID id)
{
    ckResMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes* res = ins->m_res_map.getN(id);

    if (!res)
    {
        ckThrow(ExceptionNotFound);
    }

    return *res;
}


void ckResMgr::addResource(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free)
{
    ckResMgr* ins = instance();

    if (id == ckID::ZERO || !data || data_size == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ext = ckRes::toUppercase(ext);

    ckTry
    {
        ckRes res;
        res.init(id, ext, data, data_size, NULL, is_auto_free);

        ins->m_res_map.add(id, res);
    }
    ckCatch(ckMap<ckID, ckRes>::ExceptionSameKeyExists)
    {
        ckThrow(ExceptionSameIDExists);
    }

    TypeInfo* ti = ins->m_type_info_map.getN(ext);

    if (ti)
    {
        ckRes* res = ins->m_res_map.get(id);

        (*ti->init)(res->m_id, res->m_ext, res->m_data, res->m_data_size, &res->m_exinfo);
    }
}


void ckResMgr::removeResource(ckID id)
{
    ckResMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes* res;

    ckTry
    {
        res = ins->m_res_map.get(id);
    }
    ckCatch(ckMap<ckID, ckRes>::ExceptionNotFound)
    {
        ckThrow(ExceptionNotFound);
    }

    TypeInfo* ti = ins->m_type_info_map.getN(res->m_ext);

    if (ti)
    {
        (*ti->final)(res->m_id, res->m_ext, res->m_data, res->m_data_size, res->m_exinfo);
    }

    if (res->m_is_auto_free)
    {
        ckFree(const_cast<void*>(res->m_data));
    }

    ins->m_res_map.remove(id);
}


void ckResMgr::loadResource(const char* filename, bool is_type_detect)
{
    instance();

    if (!filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return loadResourceAs(ckID::genID(ckUtil::getBasename(filename)), filename, is_type_detect);
}


void ckResMgr::loadResourceAs(ckID id, const char* filename, bool is_type_detect)
{
    instance();

    if (id == ckID::ZERO || !filename)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    void* fh = 0;

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

        data = ckMalloc(data_size);

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

    ckTry
    {
        addResource(id, is_type_detect ? ckUtil::getExtension(filename) : "", data, data_size, true);
    }
    ckCatch(ExceptionSameIDExists)
    {
        ckFree(data);

        ckThrow(ExceptionSameIDExists);
    }
}


const ckStr<char, 3>* ckResMgr::getFirstTypeN()
{
    return instance()->m_type_info_map.getFirstKeyN();
}


const ckStr<char, 3>* ckResMgr::getNextTypeN(ckStr<char, 3> ext)
{
    ckResMgr* ins = instance();

    if (ext == "")
    {
        ckThrow(ExceptionInvalidArgument);
    }

    return ins->m_type_info_map.getNextKeyN(ext);
}


const ckRes* ckResMgr::getFirstResourceN()
{
    ckResMgr* ins = instance();
    const ckID* id = ins->m_res_map.getFirstKeyN();

    return id ? ins->m_res_map.get(*id) : NULL;
}


const ckRes* ckResMgr::getNextResourceN(ckID id)
{
    ckResMgr* ins = instance();

    if (id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    const ckID* id2 = ins->m_res_map.getNextKeyN(id);

    return id2 ? ins->m_res_map.get(*id2) : NULL;
}


ckResMgr::ckResMgr()
{
    m_type_info_map.init(TYPE_HASH_SIZE);
    m_res_map.init(RESOURCE_HASH_SIZE);
}


ckResMgr::~ckResMgr()
{
    while (const ckRes* res = getFirstResourceN())
    {
        removeResource(res->m_id);
    }
}


CK_DEFINE_OPERATOR_EQUAL(ckResMgr)


CK_DEFINE_MANAGER_INSTANCE(ckResMgr)
