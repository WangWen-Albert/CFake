# Copyright 2015 Nokia
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################
# GNU Makefile
#
# Describe:
# This script is used to make static library for CFakeLib
###########################################################################

X86Win   := ../libfake_x86win.a
X86Linux := ../libfake_x86linux.a

C_CODES_COMMON		:=	\
	CFake.c				\
	CFakePlatform.c		\
	CFakeDatabase.c		\
	CFakeLog.c			\

C_CODES_X86_WIN		:=	\
	$(C_CODES_COMMON)	\
	CFakeOsWin.c		\
	CFakeHwX86.c		\

C_OBJ_X86_WIN   	:=	$(patsubst %.c, %.o, ${C_CODES_X86_WIN})

C_CODES_X86_LINUX	:=	\
	$(C_CODES_COMMON)	\
	CFakeOsLinux.c		\
	CFakeHwX86.c		\

C_OBJ_X86_Linux		:= $(patsubst %.c, %.o, ${C_CODES_X86_LINUX})

CFLAGS += -Wall -O -g -I".."

all: $(X86Win) $(X86Linux)
	rm *.o

$(X86Win): $(C_OBJ_X86_WIN)
	ar cru $@ $^

$(X86Linux): $(C_OBJ_X86_Linux)
	ar cru $@ $^

%.o: %.c
	$(CC) -c $? $(INCLUDE) $(CFLAGS)

.PHONY : clean
clean:
	rm -f ${X86Win} $(X86Linux) $(C_OBJ_X86_WIN) $(C_OBJ_X86_Linux)
