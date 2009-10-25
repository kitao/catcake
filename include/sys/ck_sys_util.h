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
    @ingroup pgSys
    TODO
*/
#define pgReadLittleEndian(data, type) pgSysMgr::readLittleEndianForSystem<type>(data)

/*!
    @ingroup pgSys
    TODO
*/
#define pgWriteLittleEndian(dest, value, type) pgSysMgr::writeLittleEndianForSystem<type>(dest, value)


/*!
    @ingroup pgSys
    TODO
*/
PG_API void pgCreatePogolyn(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag = 0);

/*!
    @ingroup pgSys
    TODO
*/
PG_API void pgDestroyPogolyn();

/*!
    @ingroup pgSys
    TODO
*/
PG_API void pgStartPogolyn();

/*!
    @ingroup pgSys
    TODO
*/
PG_API void pgEndPogolyn();

/*!
    @ingroup pgSys
    TODO
*/
PG_API void pgError(const char* msg, ...);
