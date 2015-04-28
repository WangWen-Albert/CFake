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

Target := libcmockery.a

CFLAGS = -Wall -O -g -I"."

ifeq ($(LIB_SYS), 32)
	CFLAGS += -m32
endif

ifeq ($(LIB_SYS), 64)
	CFLAGS += -m64
endif

all: $(Target)

$(Target): cmockery.o
	ar cru $@ $^

%.o: %.c
	$(CC) -c $? $(INCLUDE) $(CFLAGS)

.PHONY : clean
clean:
	rm -f ${X86Win} $(Target) cmockery.o
