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


#ifndef PG_PRIVATE_MACRO_H_
#define PG_PRIVATE_MACRO_H_


#define PG_DEFINE_COPY_CONSTRUCTOR(name) name::name(const name&) {}


#define PG_DEFINE_OPERATOR_EQUAL(name) void name::operator=(const name&) {}


#define PG_DEFINE_MANAGER_IS_CREATED(name) \
    bool name::isCreated() \
    { \
        return m_instance ? true : false; \
    }


#define PG_DEFINE_MANAGER_CREATE(name, create_suffix, destroy_suffix) \
    void name::create##create_suffix() \
    { \
        destroy##destroy_suffix(); \
    \
        m_instance = pgNew(name); \
    }


#define PG_DEFINE_MANAGER_DESTROY(name, suffix) \
    void name::destroy##suffix() \
    { \
        if (m_instance) \
        { \
            pgDelete(m_instance, name); \
            m_instance = NULL; \
        } \
    }


#define PG_DEFINE_MANAGER_INSTANCE(name) \
    name* name::instance() \
    { \
        if (!isCreated()) \
        { \
            pgThrow(ExceptionNotInitialized); \
        } \
    \
        return m_instance; \
    }


#define PG_VSPRINTF(buf, buf_size, format) \
    va_list ap; \
    \
    va_start(ap, str); \
    pgLowLevelAPI::vsprintf(buf, buf_size, format, &ap); \
    va_end(ap); \
    \
    buf[buf_size - 1] = '\0'


#define PG_VSWPRINTF(buf, buf_size, format) \
    va_list ap; \
    \
    va_start(ap, str); \
    pgLowLevelAPI::vswprintf(buf, buf_size, format, &ap); \
    va_end(ap); \
    \
    buf[buf_size - 1] = L'\0'


#endif // !PG_PRIVATE_MACRO_H_
