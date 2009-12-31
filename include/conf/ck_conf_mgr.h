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


/*!
    @ingroup ckConf
    The configuration manager singleton.
*/
class CK_API ckConfMgr
{
    friend class ckConf;

public:
    //! @cond
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionNotInitialized);
    ckDefineException(ExceptionSameIDExists);
    //! @endcond

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
