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


uniform mat4 ck_local_to_screen;

attribute vec4 ck_vertex;
attribute vec4 ck_color;
attribute vec2 ck_texcoord;

attribute float ck_att_00, ck_att_01; // texcoords of texture2

varying vec4 vary_color;
varying vec2 vary_texcoord1;
varying vec2 vary_texcoord2;


void main(void)
{
    gl_Position = ck_local_to_screen * ck_vertex;

    vary_color = ck_color;

    vary_texcoord1.s = gl_Position.x / 2.0 + 0.5;
    vary_texcoord1.t = gl_Position.y / -2.0 + 0.5;

    vary_texcoord2.s = ck_att_00;
    vary_texcoord2.t = ck_att_01;

    // Because ck_texcoord isn't used in this shader,
    // you can also use it instead of ck_att_00 and ck_att_01.
}
