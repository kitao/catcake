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


#include <math.h>
#include <stdlib.h>

#include "ck_low_level_api.h"


r32 ckLowLevelAPI::sqrt(r32 x)
{
    return ::sqrtf(x);
}


r32 ckLowLevelAPI::sin(r32 rad)
{
    return ::sinf(rad);
}


r32 ckLowLevelAPI::cos(r32 rad)
{
    return ::cosf(rad);
}


r32 ckLowLevelAPI::asin(r32 x)
{
    return ::asinf(x);
}


r32 ckLowLevelAPI::acos(r32 x)
{
    return ::acosf(x);
}


r32 ckLowLevelAPI::atan2(r32 y, r32 x)
{
    return ::atan2f(y, x);
}


void ckLowLevelAPI::srand(s32 seed)
{
    ::srand(seed);
}


s32 ckLowLevelAPI::rand()
{
    return ::rand();
}
