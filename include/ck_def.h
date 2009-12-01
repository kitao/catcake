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


#ifndef CK_DEF_H_
#define CK_DEF_H_


/*!
    @defgroup ckDef ckDef -- Basic difinitions
    Provides the basic definitions used by the all modules of Catcake.
*/


// #define CK_NO_THROW_EXCEPTION


#ifndef CK_API
#ifdef _MSC_VER
#ifdef CATCAKE_EXPORTS
#define CK_API __declspec(dllexport)
#else
#define CK_API __declspec(dllimport)
#endif
#else
#define CK_API
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
    @ingroup ckDef
    The basic types of Catcake.
*/
namespace ckBasicType
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

using namespace ckBasicType;


/*!
    @ingroup ckDef
    The version number of Catcake.
*/
const u32 CATCAKE_VERSION = 85; // 0.8.5


/*!
    @ingroup ckDef
    The base class for the exceptions of Catcake.
*/
class ckException
{
public:
    /*!
        Constructs an exception.
        @param[in] exception The name of an exception.
        @param[in] file The file name in where an exception occurred.
        @param[in] line The line number at where an exception occurred.
    */
    ckException(const char* exception, const char* file, u32 line)
    {
        m_exception = exception;
        m_file = file;
        m_line = line;
    }

    /*!
        Returns the name of this exception.
        @return The name of this exception.
    */
    const char* getException() const
    {
        return m_exception;
    }

    /*!
        Returns the file name in where this exception occurred.
        @return The file name in where this exception occurred.
    */
    const char* getFile() const
    {
        return m_file;
    }

    /*!
        Returns the line number at where this exception occurred.
        @return the line number at where this exception occurred.
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


#ifdef CK_NO_THROW_EXCEPTION

CK_API void ckSubstituteThrow(const char* exception, const char* file, u32 line);

#define ckDefineException(e)
#define ckTry if (true)
#define ckThrow(e) ckSubstituteThrow(#e, __FILE__, __LINE__)
#define ckCatch(...) if (false)

#else

/*!
    @ingroup ckDef
    Defines an exception class.
    @param[in] e The name of an exception class.
*/
#define ckDefineException(e) \
    class e : public ckException \
    { \
    public: \
        e(const char* exception, const char* file, u32 line) : ckException(exception, file, line) {} \
    }


/*!
    @ingroup ckDef
    Tries to catch an exception in the following block.
*/
#define ckTry try


/*!
    @ingroup ckDef
    Throws an exception.
    @param[in] e The name of an exception class.
*/
#define ckThrow(e) throw e(#e, __FILE__, __LINE__)


/*!
    @ingroup ckDef
    Catches the specified exception.
    @param[in] ... An exception class and its argument.
*/
#define ckCatch(...) catch (__VA_ARGS__)

#endif // CK_NO_THROW_EXCEPTION


#endif // !CK_DEF_H_
