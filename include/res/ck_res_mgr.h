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


/*!
    @ingroup pgRes
    TODO
*/
class PG_API pgResMgr
{
public:
    pgDefineException(ExceptionCannotOpenFile);
    pgDefineException(ExceptionCannotReadFile);
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotFound);
    pgDefineException(ExceptionNotInitialized);
    pgDefineException(ExceptionSameExtensionExists);
    pgDefineException(ExceptionSameIDExists);

    typedef void (*Initializer)(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    typedef void (*Finalizer)(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    static bool isCreated();
    static void createAfterTask();
    static void destroyBeforeSys();

    static u16 getTypeNum();
    static u16 getResourceNum();

    static void addType(pgStr<char, 3> ext, Initializer init, Finalizer final);
    static void removeType(pgStr<char, 3> ext);

    static bool hasResource(pgID id);
    static pgRes getResource(pgID id);
    static void addResource(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free);
    static void removeResource(pgID id);

    static void loadResource(const char* filename, bool is_type_detect);
    static void loadResourceAs(pgID id, const char* filename, bool is_type_detect);

    static const pgStr<char, 3>* getFirstTypeN();
    static const pgStr<char, 3>* getNextTypeN(pgStr<char, 3> ext);

    static const pgRes* getFirstResourceN();
    static const pgRes* getNextResourceN(pgID id);

private:
    static const u32 TYPE_HASH_SIZE = 1;
    static const u32 RESOURCE_HASH_SIZE = 10;

    struct TypeInfo
    {
        pgStr<char, 3> ext;
        Initializer init;
        Finalizer final;
    };

    pgResMgr();
    ~pgResMgr();
    void operator=(const pgResMgr&);

    static pgResMgr* instance();

    pgMap<pgStr<char, 3>, TypeInfo> m_type_info_map;
    pgMap<pgID, pgRes> m_res_map;

    static pgResMgr* m_instance;
};
