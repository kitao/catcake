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
