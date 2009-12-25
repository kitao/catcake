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
    @ingroup ckRes
    The resource manager singleton.
*/
class CK_API ckResMgr
{
public:
    //! @cond
    ckDefineException(ExceptionCannotOpenFile);
    ckDefineException(ExceptionCannotReadFile);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionNotFound);
    ckDefineException(ExceptionNotInitialized);
    ckDefineException(ExceptionSameExtensionExists);
    ckDefineException(ExceptionSameIDExists);
    //! @endcond

    /*!
        The resource initializer type.
        @param[in] id The ID of the resource.
        @param[in] ext The extension of the resource.
        @param[in] data The data of the resource.
        @param[in] data_size The size of the data.
        @param[out] extinfo The extension data of the resource.
    */
    typedef void (*Initializer)(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);

    /*!
        The resource finalizer type.
        @param[in] id The ID of the resource.
        @param[in] ext The extension of the resource.
        @param[in] data The data of the resource.
        @param[in] extinfo The extension data of the resource.
    */
    typedef void (*Finalizer)(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    /*!
        Returns whether the resource manager singleton is created.
        @return Whether the resource manager singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the resource manager singleton.
    */
    static void createAfterTask();

    /*!
        Destroys the resource manager singleton.
    */
    static void destroyBeforeSys();

    /*!
        Returns the number of the registered types.
        @return The number of the registered types.
    */
    static u16 getTypeNum();

    /*!
        Returns the number of the resources.
    */
    static u16 getResourceNum();

    /*!
        Registers the specified.
        @param[in] ext The extension of a type.
        @param[in] init The initializer of a type.
        @param[in] final The finalizer of a type.
    */
    static void addType(ckStr<char, 3> ext, Initializer init, Finalizer final);

    /*!
        Unregistres the specified type.
        @param[in] ext The extension of a type.
    */
    static void removeType(ckStr<char, 3> ext);

    /*!
        Returns whether the specified resource is registered.
        @param[in] id The id of a resource.
        @return Whether the specified resource is registered.
    */
    static bool hasResource(ckID id);

    /*!
        Returns the specified resource.
        @param[in] id The id of a resource.
        @return The specified resource.
    */
    static ckRes getResource(ckID id);

    /*!
        Registers the specified resource.
        @param[in] id The id of a resource.
        @param[in] ext The extension of a resource.
        @param[in] data The data of a resource.
        @param[in] data_size The size of a data.
        @param[in] is_auto_free Whether releases the data automatically.
    */
    static void addResource(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free);

    /*!
        Unregisters the specified resource.
        @param[in] id The ID of a resource.
    */
    static void removeResource(ckID id);

    /*!
        Loads the specified resource.
        @param[in] filename The name of a file.
        @param[in] is_type_detect Whether enables auto initialize/finalize.
    */
    static void loadResource(const char* filename, bool is_type_detect);

    /*!
        Loads the specified resource as the specified ID.
        @param[in] id The id of a resource.
        @param[in] filename The name of a file.
        @param[in] is_type_detect Whether enables auto initialize/finalize.
    */
    static void loadResourceAs(ckID id, const char* filename, bool is_type_detect);

    /*!
        Returns the first registered type. If the first registered type doesn't exist, returns NULL.
        @return The first registered type.
    */
    static const ckStr<char, 3>* getFirstTypeN();

    /*!
        Returns the next registered type of the specified type.
        @param[in] ext The extension of a resource.
    */
    static const ckStr<char, 3>* getNextTypeN(ckStr<char, 3> ext);

    /*!
        Returns the first resource.
        @return The first resource. If the first resource doesn't exist, returns NULL.
    */
    static const ckRes* getFirstResourceN();

    /*!
        Returns the next resource of the specified resource. If the next resource doesn't exist, returns NULL.
        @param[in] id The ID of a resource.
    */
    static const ckRes* getNextResourceN(ckID id);

private:
    static const u32 TYPE_HASH_SIZE = 1;
    static const u32 RESOURCE_HASH_SIZE = 10;

    struct TypeInfo
    {
        ckStr<char, 3> ext;
        Initializer init;
        Finalizer final;
    };

    ckResMgr();
    ~ckResMgr();
    void operator=(const ckResMgr&);

    static ckResMgr* instance();

    ckMap<ckStr<char, 3>, TypeInfo> m_type_info_map;
    ckMap<ckID, ckRes> m_res_map;

    static ckResMgr* m_instance;
};
