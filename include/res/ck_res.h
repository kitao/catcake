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


class pgResMgr;


/*!
    @ingroup pgRes
    TODO
*/
class PG_API pgRes
{
    friend class pgResMgr;

public:
    pgRes();
    pgID getID() const;
    pgStr<char, 3> getExtension() const;

    template<class T> const T* getData() const
    {
        return reinterpret_cast<const T*>(m_data);
    }

    u32 getDataSize() const;

    template<class T> T* getExInfo() const
    {
        return reinterpret_cast<T*>(m_exinfo);
    }

    bool isAutoFree() const;

private:
    void init(pgID id, pgStr<char, 3> ext, const void* data, u32 data_size, void* exinfo, bool is_auto_free);

    static pgStr<char, 3> toUppercase(pgStr<char, 3> str);

    pgID m_id;
    pgStr<char, 3> m_ext;
    const void* m_data;
    u32 m_data_size;
    void* m_exinfo;
    bool m_is_auto_free;
};
