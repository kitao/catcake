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


class ckConfMgr;


/*!
    @ingroup ckConf
    A Configuration.
*/
class CK_API ckConf
{
    friend class ckConfMgr;

public:
    ckDefineException(ExceptionCannotOpenFile);
    ckDefineException(ExceptionCannotReadFile);
    ckDefineException(ExceptionInvalidArgument);
    ckDefineException(ExceptionInvalidCall);

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

    ckID getID() const;
    bool isValid() const;
    u16 getErrorLineNo() const;

    u16 getEntryNum(const char* ent_name) const;
    ckEnt* getEntryFromFirstN(const char* ent_name) const;
    ckEnt* getEntryFromLastN(const char* ent_name) const;
    ckEnt* getEntryBeforeN(const char* ent_name, ckEnt* ent) const;
    ckEnt* getEntryAfterN(const char* ent_name, ckEnt* ent) const;

    ckEnt* getFirstEntryN() const;
    ckEnt* getLastEntryN() const;

    static ckConf* newPriorConfigBeforeInitialization(const char* filename);
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
