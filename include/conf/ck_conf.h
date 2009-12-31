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


class ckConfMgr;


/*!
    @ingroup ckConf
    A configuration script.
*/
class CK_API ckConf
{
    friend class ckConfMgr;

public:
    //! @cond
    ckDefineException(ExceptionCannotOpenFile);
    ckDefineException(ExceptionCannotReadFile);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);
    //! @endcond

    /*!
        Returns the previous configuration. If the previous configuration doesn't exist, returns NULL.
        @return The previous configuration.
    */
    ckConf* getPrevN() const;

    /*!
        Returns the next configuration. If the next configuration doesn't exist, returns NULL.
        @return The next configuration.
    */
    ckConf* getNextN() const;

    /*!
        Returns the ID of this configuration.
        @return The ID of this configuration.
    */
    ckID getID() const;

    /*!
        Returns whether this configuration is valid.
        @return Whether this configuration is valid.
    */
    bool isValid() const;

    /*!
        Returns the line number of this configuration.
        @return The line number of this configuration.
    */
    u16 getErrorLineNo() const;

    /*!
        Returns the number of the entries.
        @param[in] ent_name The name of an entry.
        @return The number of the entries.
    */
    u16 getEntryNum(const char* ent_name) const;

    /*!
        Returns the specified entry. The entry is searched from the first. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @return The specified entry.
    */
    ckEnt* getEntryFromFirstN(const char* ent_name) const;

    /*!
        Returns the specified entry. The entry is searched from the last. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @return The specified entry.
    */
    ckEnt* getEntryFromLastN(const char* ent_name) const;

    /*!
        Returns the specified entry. The entry is searched from the previous of the some entry. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @param[in] ent An entry.
        @return The specified entry.
    */
    ckEnt* getEntryBeforeN(const char* ent_name, ckEnt* ent) const;

    /*!
        Returns the specified entry. The entry is searched from the next of the some entry. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @param[in] ent An entry.
        @return The specified entry.
    */
    ckEnt* getEntryAfterN(const char* ent_name, ckEnt* ent) const;

    /*!
        Returns the first entry. If the first entry doesn't exist, return NULL.
        @return The first entry.
    */
    ckEnt* getFirstEntryN() const;

    /*!
        Returns the last entry. If the last entry doesn't exist, return NULL.
        @return The last entry.
    */
    ckEnt* getLastEntryN() const;

    /*!
        Creates an configuration from the specified file before the system initialization.
        @param[in] filename The name of a file.
        @return An entry.
    */
    static ckConf* newPriorConfigBeforeInitialization(const char* filename);

    /*!
        Deletes the specified configuration created before the system initialization.
        @param[in] conf A configuration.
    */
    static void deletePriorConfig(ckConf* conf);

private:
    ckConf(ckID id, const void* data, u32 data_size);
    ckConf(ckID id, const void* data, u32 data_size, void* dummy);
    ~ckConf();
    void operator=(const ckConf&);

    ckEnt* newEntry(const char* ent_name, u16 val_num);
    void deleteEntry(ckEnt* ent);

    void parse(const void* data, u32 data_size);

    ckID m_id;
    u16 m_err_line_no;
    ckList<ckEnt> m_ent_list;
    void* (*m_malloc)(u32 size);
    void (*m_free)(void* ptr);
};
