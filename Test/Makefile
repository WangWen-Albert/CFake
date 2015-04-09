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
# This script is used to make a program to test the library CFakeLib
###########################################################################

Target := AutoTest.exe

CFLAGS := -Wall -O -g -fprofile-arcs -ftest-coverage

CFLAGS += -I"." 			\
          -I"../CFakeLib" 	\
		  -I"CMockeryLib"	\

LIB := "./CMockeryLib/libcmockery.a"

ifeq ($(OS),Windows_NT)
LIB += "../CFakeLib/libfake_x86win.a"
else
LIB += "../CFakeLib/libfake_x86linux.a"
endif

C_TEST_CODES :=			\
	Test_Main.c			\
	Test_GameOfLife.c	\
	Test_CFakeLib.c		\

C_TEST_OBJ	:=	$(patsubst %.c, %.o, ${C_TEST_CODES})

$(Target): $(C_TEST_OBJ)
	$(CC) -o $@ $(CFLAGS) $^ $(LIB)

%.o: %.c
	$(CC) -c $? $(INCLUDE) $(CFLAGS)

.PHONY : clean
clean:
	rm -f $(Target)  *.o *.gcda *.gcno
	rm -rf Report