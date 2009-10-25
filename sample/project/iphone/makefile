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


POGOLYN_DIR = ../../..


.PHONY: all clean

all: clean
	for i in $(POGOLYN_DIR)/sample/sample*; do [ -d $$i ] && cp makefile.sub $$i/makefile; done
	mv $(POGOLYN_DIR)/sample/sample01_debug/pogolyn_logo.cpp $(POGOLYN_DIR)/sample
	for i in $(POGOLYN_DIR)/sample/sample*; do [ -d $$i ] && cp $(POGOLYN_DIR)/sample/pogolyn_logo.cpp $$i/.; done
	rm $(POGOLYN_DIR)/sample/pogolyn_logo.cpp
	for i in $(POGOLYN_DIR)/sample/sample*; do [ -d $$i ] && make -C $$i; done
	chmod -R 777 $(POGOLYN_DIR)/sample/bin/*.app
	chmod -R 777 $(POGOLYN_DIR)/sample/data
	cp -rf $(POGOLYN_DIR)/sample/bin/*.app /Applications
	cp -rf $(POGOLYN_DIR)/sample/data /Applications

clean:
	rm -rf $(POGOLYN_DIR)/sample/bin/*
	rm -f $(POGOLYN_DIR)/sample/sample*/*.o
	rm -f $(POGOLYN_DIR)/sample/sample*/makefile
	mv $(POGOLYN_DIR)/sample/sample01_debug/pogolyn_logo.cpp $(POGOLYN_DIR)/sample
	rm -f $(POGOLYN_DIR)/sample/sample*/pogolyn_logo.cpp
	mv $(POGOLYN_DIR)/sample/pogolyn_logo.cpp $(POGOLYN_DIR)/sample/sample01_debug
