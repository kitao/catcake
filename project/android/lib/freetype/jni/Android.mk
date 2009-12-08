#
#   Copyright (c) 2007-2009 Takashi Kitao
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the above copyright
#   `  notice, this list of conditions and the following disclaimer.
#
#   2. Redistributions in binary form must reproduce the above copyright
#   `  notice, this list of conditions and the following disclaimer in the
#   `  documentation and/or other materials provided with the distribution.
#
#   3. The name of the author may not be used to endorse or promote products
#   `  derived from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := freetype

LOCAL_SRC_FILES := \
	../src/autofit/autofit.c \
	../src/base/ftbase.c \
	../src/base/ftbbox.c \
	../src/base/ftbitmap.c \
	../src/base/ftdebug.c \
	../src/base/ftfstype.c \
	../src/base/ftgasp.c \
	../src/base/ftglyph.c \
	../src/base/ftinit.c \
	../src/base/ftmm.c \
	../src/base/ftpfr.c \
	../src/base/ftstroke.c \
	../src/base/ftsynth.c \
	../src/base/ftsystem.c \
	../src/base/fttype1.c \
	../src/base/ftwinfnt.c \
	../src/bdf/bdf.c \
	../src/cache/ftcache.c \
	../src/cff/cff.c \
	../src/cid/type1cid.c \
	../src/gzip/ftgzip.c \
	../src/lzw/ftlzw.c \
	../src/pcf/pcf.c \
	../src/pfr/pfr.c \
	../src/psaux/psaux.c \
	../src/pshinter/pshinter.c \
	../src/psnames/psmodule.c \
	../src/raster/raster.c \
	../src/sfnt/sfnt.c \
	../src/smooth/smooth.c \
	../src/truetype/truetype.c \
	../src/type1/type1.c \
	../src/type42/type42.c \
	../src/winfonts/winfnt.c

LOCAL_CXXFLAGS := -O2 -Wall -s -DNDEBUG -DFT2_BUILD_LIBRARY -I$(LOCAL_PATH)/../include

LOCAL_LDLIBS :=

include $(BUILD_STATIC_LIBRARY)
