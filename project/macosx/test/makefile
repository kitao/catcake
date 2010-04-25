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
LDFLAGS     =
DEFINES     = -DNDEBUG -DCK_MACOSX
INCLUDES    = -I$(BASE_DIR)/include -I$(BASE_DIR)/test
LIBS        = -L$(BASE_DIR)/lib -L/usr/X11R6/lib -lcatcake -lfreetype -lpng -lz
SRCS        = $(shell find $(BASE_DIR)/test -name "*.cpp")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = bin/test

FRAMEWORK   = -framework OpenGL -framework GLUT


.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS) $(FRAMEWORK)


.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
