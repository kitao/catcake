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


#include <math.h>
#include <stdlib.h>

#include "ck_low_level_api.h"


r32 ckLowLevelAPI::sqrt(r32 x)
{
    return ::sqrt(x);
}


r32 ckLowLevelAPI::sin(r32 rad)
{
    return ::sin(rad);
}


r32 ckLowLevelAPI::cos(r32 rad)
{
    return ::cos(rad);
}


r32 ckLowLevelAPI::asin(r32 x)
{
    return ::asin(x);
}


r32 ckLowLevelAPI::acos(r32 x)
{
    return ::acos(x);
}


r32 ckLowLevelAPI::atan2(r32 y, r32 x)
{
    return ::atan2(y, x);
}


void ckLowLevelAPI::srand(s32 seed)
{
    ::srand(seed);
}


s32 ckLowLevelAPI::rand()
{
    return ::rand();
}
