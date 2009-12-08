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
	freetype-2.3.11/src/autofit/autofit.c \
	freetype-2.3.11/src/base/ftbase.c \
	freetype-2.3.11/src/base/ftbbox.c \
	freetype-2.3.11/src/base/ftbitmap.c \
	freetype-2.3.11/src/base/ftdebug.c \
	freetype-2.3.11/src/base/ftfstype.c \
	freetype-2.3.11/src/base/ftgasp.c \
	freetype-2.3.11/src/base/ftglyph.c \
	freetype-2.3.11/src/base/ftinit.c \
	freetype-2.3.11/src/base/ftmm.c \
	freetype-2.3.11/src/base/ftpfr.c \
	freetype-2.3.11/src/base/ftstroke.c \
	freetype-2.3.11/src/base/ftsynth.c \
	freetype-2.3.11/src/base/ftsystem.c \
	freetype-2.3.11/src/base/fttype1.c \
	freetype-2.3.11/src/base/ftwinfnt.c \
	freetype-2.3.11/src/bdf/bdf.c \
	freetype-2.3.11/src/cache/ftcache.c \
	freetype-2.3.11/src/cff/cff.c \
	freetype-2.3.11/src/cid/type1cid.c \
	freetype-2.3.11/src/gzip/ftgzip.c \
	freetype-2.3.11/src/lzw/ftlzw.c \
	freetype-2.3.11/src/pcf/pcf.c \
	freetype-2.3.11/src/pfr/pfr.c \
	freetype-2.3.11/src/psaux/psaux.c \
	freetype-2.3.11/src/pshinter/pshinter.c \
	freetype-2.3.11/src/psnames/psmodule.c \
	freetype-2.3.11/src/raster/raster.c \
	freetype-2.3.11/src/sfnt/sfnt.c \
	freetype-2.3.11/src/smooth/smooth.c \
	freetype-2.3.11/src/truetype/truetype.c \
	freetype-2.3.11/src/type1/type1.c \
	freetype-2.3.11/src/type42/type42.c \
	freetype-2.3.11/src/winfonts/winfnt.c

LOCAL_CFLAGS := -O2 -Wall -s -DNDEBUG -DFT2_BUILD_LIBRARY -I$(LOCAL_PATH)/freetype-2.3.11/include

LOCAL_LDLIBS :=

include $(BUILD_STATIC_LIBRARY)
