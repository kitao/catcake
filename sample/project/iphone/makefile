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


.PHONY: all clean

all: clean
	for i in $(BASE_DIR)/sample/sample*; do [ -d $$i ] && cp makefile.sub $$i/makefile; done
	mv $(BASE_DIR)/sample/sample01_debug/catcake_logo.cpp $(BASE_DIR)/sample
	for i in $(BASE_DIR)/sample/sample*; do [ -d $$i ] && cp $(BASE_DIR)/sample/catcake_logo.cpp $$i/.; done
	rm $(BASE_DIR)/sample/catcake_logo.cpp
	for i in $(BASE_DIR)/sample/sample*; do [ -d $$i ] && make -C $$i; done

clean:
	rm -rf $(BASE_DIR)/sample/bin/*
	rm -f $(BASE_DIR)/sample/sample*/*.o
	rm -f $(BASE_DIR)/sample/sample*/makefile
	mv $(BASE_DIR)/sample/sample01_debug/catcake_logo.cpp $(BASE_DIR)/sample
	rm -f $(BASE_DIR)/sample/sample*/catcake_logo.cpp
	mv $(BASE_DIR)/sample/catcake_logo.cpp $(BASE_DIR)/sample/sample01_debug
