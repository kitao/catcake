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


uniform mat4 pg_local_to_screen;

attribute vec4 pg_vertex;
attribute vec4 pg_color;
attribute vec2 pg_texcoord;

attribute float pg_att_00, pg_att_01; // texcoords of texture2

varying vec4 vary_color;
varying vec2 vary_texcoord1;
varying vec2 vary_texcoord2;


void main(void)
{
	gl_Position = pg_local_to_screen * pg_vertex;
	
	vary_color = pg_color;
	
	vary_texcoord1.s = gl_Position.x / 2.0 + 0.5;
	vary_texcoord1.t = gl_Position.y / -2.0 + 0.5;
	
	vary_texcoord2.s = pg_att_00;
	vary_texcoord2.t = pg_att_01;
	
	// Because pg_texcoord isn't used in this shader,
	// you can also use it instead of pg_att_00 and pg_att_01.
}
