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
    @ingroup ckConf
    The configuration manager singleton.
*/
class CK_API ckConfMgr
{
    friend class ckConf;

public:
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionNotInitialized);
    ckDefineException(ExceptionSameIDExists);

    /*!
        Returns whether ckConfMgr singleton is created.
        @return Whether ckConfMgr singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the ckConfMgr singleton.
    */
    static void createAfterRes();

    /*!
        Destroys the ckConfMgr singleton.
    */
    static void destroyBeforeRes();

    /*!
        Returns whether the specified configuration exists.
        @param[in] id The ID of a configuration.
        @return Whether the specified configuration exists.
    */
    static bool hasConfig(ckID id);

    /*!
        Returns the specified configuration.
        @param[in] id The ID of a configuration.
        @return The specified configuration.
    */
    static ckConf* getConfig(ckID id);

    /*!
        Creates a configuration.
        @param[in] id The ID of a configuration.
        @param[in] data The data of configuration.
        @param[in] data_size The size of data.
        @return A configuration.
    */
    static ckConf* newConfig(ckID id, const void* data, u32 data_size);

    /*!
        Deletes the specified configuration.
        @param[in] id The ID of a configuration.
    */
    static void deleteConfig(ckID id);

    /*!
        Returns the first configuration. If the first configuration doesn't exist, return NULL.
        @return The first configuration.
    */
    static ckConf* getFirstConfigN();

    /*!
        Returns the last configuration. If the last configuration doesn't exist, return NULL.
        @return The last configuration.
    */
    static ckConf* getLastConfigN();

private:
    static const u32 CONFIG_HASH_SIZE = 10;

    ckConfMgr();
    ~ckConfMgr();
    void operator=(const ckConfMgr&);

    static ckConfMgr* instance();

    static void configInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void configFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    ckMap<ckID, ckConf*> m_conf_map;

    static ckConfMgr* m_instance;
};
