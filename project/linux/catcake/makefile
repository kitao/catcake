#
#   Copyright (c) 2007-2010 Takashi Kitao
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to deal
#   in the Software without restriction, including without limitation the rights
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in
#   all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#   THE SOFTWARE.
#


BASE_DIR = ../../..

CXX         = g++
CXXFLAGS    = -O2 -fno-strict-aliasing -Wall -s
DEFINES     = -DNDEBUG -DCK_LINUX
INCLUDES    = -I$(BASE_DIR)/include -I$(BASE_DIR)/source -I$(BASE_DIR)/project/include/zlib -I$(BASE_DIR)/project/include/libpng -I$(BASE_DIR)/project/include/freetype

AR          = ar
ARFLAGS     = rs
SRCS        = $(shell find $(BASE_DIR)/source -name "*.cpp")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = $(BASE_DIR)/lib/libcatcake.a


.PHONY: all clean

all: $(TARGET)
	cp $(BASE_DIR)/project/linux/lib/libz.a $(BASE_DIR)/lib
	cp $(BASE_DIR)/project/linux/lib/libpng.a $(BASE_DIR)/lib
	cp $(BASE_DIR)/project/linux/lib/libfreetype.a $(BASE_DIR)/lib

clean:
	rm -f $(TARGET) $(OBJS)
	rm -f $(BASE_DIR)/lib/libz.a
	rm -f $(BASE_DIR)/lib/libpng.a
	rm -f $(BASE_DIR)/lib/libfreetype.a

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJS)


.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
