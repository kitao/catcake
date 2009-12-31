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


#ifndef CK_PRIVATE_MACRO_H_
#define CK_PRIVATE_MACRO_H_


#define CK_DEFINE_COPY_CONSTRUCTOR(name) name::name(const name&) {}


#define CK_DEFINE_OPERATOR_EQUAL(name) void name::operator=(const name&) {}


#define CK_DEFINE_MANAGER_IS_CREATED(name) \
    bool name::isCreated() \
    { \
        return m_instance ? true : false; \
    }


#define CK_DEFINE_MANAGER_CREATE(name, create_suffix, destroy_suffix) \
    void name::create##create_suffix() \
    { \
        destroy##destroy_suffix(); \
    \
        m_instance = ckNew(name); \
    }


#define CK_DEFINE_MANAGER_DESTROY(name, suffix) \
    void name::destroy##suffix() \
    { \
        if (m_instance) \
        { \
            ckDelete(m_instance, name); \
            m_instance = NULL; \
        } \
    }


#define CK_DEFINE_MANAGER_INSTANCE(name) \
    name* name::instance() \
    { \
        if (!isCreated()) \
        { \
            ckThrow(ExceptionNotInitialized); \
        } \
    \
        return m_instance; \
    }


#define CK_VSPRINTF(buf, buf_size, format) \
    va_list ap; \
    \
    va_start(ap, str); \
    ckLowLevelAPI::vsprintf(buf, buf_size, format, &ap); \
    va_end(ap); \
    \
    buf[buf_size - 1] = '\0'


#define CK_VSWPRINTF(buf, buf_size, format) \
    va_list ap; \
    \
    va_start(ap, str); \
    ckLowLevelAPI::vswprintf(buf, buf_size, format, &ap); \
    va_end(ap); \
    \
    buf[buf_size - 1] = L'\0'


#endif // !CK_PRIVATE_MACRO_H_
