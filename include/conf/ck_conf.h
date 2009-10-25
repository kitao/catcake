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


class pgConfMgr;


/*!
    @ingroup pgConf
    TODO
*/
class PG_API pgConf
{
    friend class pgConfMgr;

public:
    pgDefineException(ExceptionCannotOpenFile);
    pgDefineException(ExceptionCannotReadFile);
    pgDefineException(ExceptionInvalidArgument);
    pgDefineException(ExceptionInvalidCall);

    pgConf* getPrevN() const;
    pgConf* getNextN() const;

    pgID getID() const;
    bool isValid() const;
    u16 getErrorLineNo() const;

    u16 getEntryNum(const char* ent_name) const;
    pgEnt* getEntryFromFirstN(const char* ent_name) const;
    pgEnt* getEntryFromLastN(const char* ent_name) const;
    pgEnt* getEntryBeforeN(const char* ent_name, pgEnt* ent) const;
    pgEnt* getEntryAfterN(const char* ent_name, pgEnt* ent) const;

    pgEnt* getFirstEntryN() const;
    pgEnt* getLastEntryN() const;

    static pgConf* newPriorConfigBeforeInitialization(const char* filename);
    static void deletePriorConfig(pgConf* conf);

private:
    pgConf(pgID id, const void* data, u32 data_size);
    pgConf(pgID id, const void* data, u32 data_size, void* dummy);
    ~pgConf();
    void operator=(const pgConf&);

    pgEnt* newEntry(const char* ent_name, u16 val_num);
    void deleteEntry(pgEnt* ent);

    void parse(const void* data, u32 data_size);

    pgID m_id;
    u16 m_err_line_no;
    pgList<pgEnt> m_ent_list;
    void* (*m_malloc)(u32 size);
    void (*m_free)(void* ptr);
};
