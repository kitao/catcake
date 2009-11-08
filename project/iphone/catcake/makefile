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


BASE_DIR = ../../..

CXX         = arm-apple-darwin9-g++
CXXFLAGS    = -O2 -Wall -s
OBJCFLAGS   = -O2 -Wall -s
DEFINES     = -DNDEBUG -DCK_IPHONE -DCK_NO_THROW_EXCEPTION
INCLUDES    = -I$(BASE_DIR)/include -I$(BASE_DIR)/source -I$(BASE_DIR)/project/win32/include/zlib -I$(BASE_DIR)/project/win32/include/libpng -I$(BASE_DIR)/project/win32/include/freetype -I/var/include/gcc/darwin/4.0 -I/var/include/c++/4.0.0
AR          = ar
ARFLAGS     = rs
SRCS        = $(shell find $(BASE_DIR)/source -name "*.cpp")  $(shell find $(BASE_DIR)/source -name "*.mm")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = $(BASE_DIR)/lib/libcatcake.a


.PHONY: all clean

all: $(TARGET)
	cp $(BASE_DIR)/project/iphone/lib/libz.a $(BASE_DIR)/lib
	cp $(BASE_DIR)/project/iphone/lib/libpng.a $(BASE_DIR)/lib
	cp $(BASE_DIR)/project/iphone/lib/libfreetype.a $(BASE_DIR)/lib

clean:
	rm -f $(TARGET) $(OBJS)
	rm -f $(BASE_DIR)/lib/libz.a
	rm -f $(BASE_DIR)/lib/libpng.a
	rm -f $(BASE_DIR)/lib/libfreetype.a

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


.SUFFIXES: .o .cpp .mm

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

.mm.o:
	$(CXX) $(OBJCFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
