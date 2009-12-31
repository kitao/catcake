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
    @ingroup ckSys
    Reads the specified data in little endian.
    @param[in] data The data.
    @param[in] type The class type.
*/
#define ckReadLittleEndian(data, type) ckSysMgr::readLittleEndianForSystem<type>(data)

/*!
    @ingroup ckSys
    Writes the specified data in little endian.
    @param[out] dest The destination.
    @param[in] value The value to be written.
    @param[in] type The class type.
*/
#define ckWriteLittleEndian(dest, value, type) ckSysMgr::writeLittleEndianForSystem<type>(dest, value)


/*!
    @ingroup ckSys
    Creates all the managers of Catcake.
    @param[in] title The title of the application.
    @param[in] width The width of the framebuffer.
    @param[in] height The height of the framebuffer.
    @param[in] aim_fps The aim fps.
    @param[in] sys_flag System flag.
*/
CK_API void ckCreateCatcake(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag = 0);

/*!
    @ingroup ckSys
    Destroys all the managers of Catcake.
*/
CK_API void ckDestroyCatcake();

/*!
    @ingroup ckSys
    Starts the main loop of Catcake.
*/
CK_API void ckStartCatcake();

/*!
    @ingroup ckSys
    Ends the application.
*/
CK_API void ckEndCatcake();

/*!
    @ingroup ckSys
    Raise an error.
    @param msg An error message.
    @param ... Additional arguments.
*/
CK_API void ckError(const char* msg, ...);
