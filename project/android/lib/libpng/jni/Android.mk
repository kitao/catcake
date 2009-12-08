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

LOCAL_MODULE := libpng

LOCAL_SRC_FILES := \
	libpng-1.2.40/png.c \
	libpng-1.2.40/pngerror.c \
	libpng-1.2.40/pngget.c \
	libpng-1.2.40/pngmem.c \
	libpng-1.2.40/pngpread.c \
	libpng-1.2.40/pngread.c \
	libpng-1.2.40/pngrio.c \
	libpng-1.2.40/pngrtran.c \
	libpng-1.2.40/pngrutil.c \
	libpng-1.2.40/pngset.c \
	libpng-1.2.40/pngtrans.c \
	libpng-1.2.40/pngwio.c \
	libpng-1.2.40/pngwrite.c \
	libpng-1.2.40/pngwtran.c \
	libpng-1.2.40/pngwutil.c

LOCAL_C_INCLUDES := apps/libpng/jni/libpng-1.2.40 apps/zlib/jni/zlib-1.2.3

LOCAL_CFLAGS := -O2 -Wall -s -DNDEBUG

include $(BUILD_STATIC_LIBRARY)
