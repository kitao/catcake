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


#ifndef PG_DEF_H_
#define PG_DEF_H_


/*!
    @defgroup pgDef pgDef -- Basic difinitions
    Provides the basic definitions used by the all modules of Pogolyn.
*/


// #define PG_NO_THROW_EXCEPTION


#ifndef PG_API
#ifdef _MSC_VER
#ifdef POGOLYN_EXPORTS
#define PG_API __declspec(dllexport)
#else
#define PG_API __declspec(dllimport)
#endif
#else
#define PG_API
#endif
#endif


#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif


#undef min
#undef max


#ifndef NULL
#define NULL 0
#endif


#ifdef __GNUC__
typedef __SIZE_TYPE__ size_t;
#endif


/*!
    @ingroup pgDef
    The basic types of Pogolyn.
*/
namespace pgBasicType
{
    typedef signed char s8; //!< 8-bit signed type.
    typedef signed short s16; //!< 16-bit signed type.
    typedef signed int s32; //!< 32-bit signed type.

    typedef unsigned char u8; //!< 8-bit unsigned type.
    typedef unsigned short u16; //!< 16-bit unsigned type.
    typedef unsigned int u32; //!< 32-bit unsigned type.

    typedef float r32; //!< 32-bit floating point type.
    typedef double r64; //!< 64-bit floating point type.

#if defined(__GNUC__)
    typedef signed long long s64; //!< 64-bit signed type.
    typedef unsigned long long u64; //!< 64-bit unsigned type.
#elif defined(_MSC_VER)
    typedef signed __int64 s64; //!< 64-bit signed type.
    typedef unsigned __int64 u64; //!< 64-bit unsigned type.
#else
#error
#endif
}

using namespace pgBasicType;


/*!
    @ingroup pgDef
    The version number of Pogolyn.
*/
const u32 POGOLYN_VERSION = 80; // 0.8.0


/*!
    @ingroup pgDef
    The base class for the exceptions of Pogolyn.
*/
class pgException
{
public:
    /*!
        Constructs and initializes a pgException.
        @param[in] exception The name of an exception.
        @param[in] file The file name in where an exception occurred.
        @param[in] line The line number at where an exception occurred.
    */
    pgException(const char* exception, const char* file, u32 line)
    {
        m_exception = exception;
        m_file = file;
        m_line = line;
    }

    /*!
        Returns the name of the exception.
        @return The name of the exception.
    */
    const char* getException() const
    {
        return m_exception;
    }

    /*!
        Returns the file name in where the exception occurred.
        @return The file name in where the exception occurred.
    */
    const char* getFile() const
    {
        return m_file;
    }

    /*!
        Returns the line number at where the exception occurred.
        @return the line number at where the exception occurred.
    */
    u32 getLine() const
    {
        return m_line;
    }

private:
    const char* m_exception;
    const char* m_file;
    u32 m_line;
};


#ifdef PG_NO_THROW_EXCEPTION

PG_API void pgSubstituteThrow(const char* exception, const char* file, u32 line);

#define pgDefineException(e)
#define pgTry if (true)
#define pgThrow(e) pgSubstituteThrow(#e, __FILE__, __LINE__)
#define pgCatch(...) if (false)

#else

/*!
    @ingroup pgDef
    Defines an exception class of Pogolyn.
    @param[in] e The name of an exception class.
*/
#define pgDefineException(e) \
    class e : public pgException \
    { \
    public: \
        e(const char* exception, const char* file, u32 line) : pgException(exception, file, line) {} \
    }


/*!
    @ingroup pgDef
    Tries to catch an exception in the following block.
*/
#define pgTry try


/*!
    @ingroup pgDef
    Throws an exception.
    @param[in] e The name of an exception class.
*/
#define pgThrow(e) throw e(#e, __FILE__, __LINE__)


/*!
    @ingroup pgDef
    Catches the specified exception.
    @param[in] ... The name of an exception class.
*/
#define pgCatch(...) catch (__VA_ARGS__)

#endif // PG_NO_THROW_EXCEPTION


#endif // !PG_DEF_H_
