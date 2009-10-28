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


#ifndef CK_IPHONE_BRIDGE_H_
#define CK_IPHONE_BRIDGE_H_


class ckIPhoneBridge
{
public:
    /*
        methods called by Catcake
    */
    static void startApplication();

    static bool createFramebuffer();
    static void destroyFramebuffer();

    static int getFramebufferWidth();
    static int getFramebufferHeight();
    static void swapFramebuffer();

    static void getMousePos(int* x, int* y);
    static void getInnerMousePos(int* x, int* y);
    static void getOuterMousePos(int* x, int* y);
    static int getDeviceOrientation();
    static void getAcceleraion(float* x, float* y, float* z);

    /*
        methods called by iPhone
    */
    static void updateApplication();
    static void setMouseState(int button, bool is_on);
};


#endif // CK_IPHONE_BRIDGE_H_
