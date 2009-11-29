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
