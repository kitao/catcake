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
    @ingroup pgConf
    TODO
*/
class PG_API pgConfMgr
{
    friend class pgConf;

public:
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionNotFound);
    pgDefineException(ExceptionNotInitialized);
    pgDefineException(ExceptionSameIDExists);

    static bool isCreated();
    static void createAfterRes();
    static void destroyBeforeRes();

    static bool hasConfig(pgID id);
    static pgConf* getConfig(pgID id);
    static pgConf* newConfig(pgID id, const void* data, u32 data_size);
    static void deleteConfig(pgID id);

    static pgConf* getFirstConfigN();
    static pgConf* getLastConfigN();

private:
    static const u32 CONFIG_HASH_SIZE = 10;

    pgConfMgr();
    ~pgConfMgr();
    void operator=(const pgConfMgr&);

    static pgConfMgr* instance();

    static void configInitializer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void configFinalizer(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    pgMap<pgID, pgConf*> m_conf_map;

    static pgConfMgr* m_instance;
};
