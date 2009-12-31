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
