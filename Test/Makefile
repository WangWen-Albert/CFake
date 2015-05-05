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
# This script is used to build and run quick test case for CFakeLib
###########################################################################

all: help

## Build all CC&S and DSPHWAPI targets
.PHONY: x86win32
x86win32:
	@echo ""
	@echo "------------ x86win32: Build CMockery LIB      ----------"
	$(MAKE) -C CMockeryLib LIB_SYS=32
	@echo "------------ x86win32: Build CMockery Success  ----------"
	@echo ""
	@echo "------------ x86win32: Build CFake LIB         ----------"
	$(MAKE) -C ../CFakeLib/Source x86win32
	@echo "------------ x86win32: Build CFake Success     ----------"
	@echo ""
	@echo "------------ x86win32: Build Quick Test Case   ----------"
	$(MAKE) -f Test_Build.mk TEST_SYS=32
	@echo "------------ x86win32: Build Test Case Success ----------"
	@echo ""
	@echo "------------ x86win32: Run Quick Test Case     ----------"
	./AutoTest
	@echo "------------ x86win32: Run Test Case Success   ----------"
	@echo ""

.PHONY: x86win64
x86win64:
	@echo ""
	@echo "------------ x86win64: Build CMockery LIB      ----------"
	$(MAKE) -C CMockeryLib LIB_SYS=64
	@echo "------------ x86win64: Build CMockery Success  ----------"
	@echo ""
	@echo "------------ x86win64: Build CFake LIB         ----------"
	$(MAKE) -C ../CFakeLib/Source x86win64
	@echo "------------ x86win64: Build CFake Success     ----------"
	@echo ""
	@echo "------------ x86win64: Build Quick Test Case   ----------"
	$(MAKE) -f Test_Build.mk TEST_SYS=64
	@echo "------------ x86win64: Build Test Case Success ----------"
	@echo ""
	@echo "------------ x86win64: Run Quick Test Case     ----------"
	./AutoTest
	@echo "------------ x86win64: Run Test Case Success   ----------"
	@echo ""

.PHONY: x86linux32
x86linux32:
	@echo ""
	@echo "------------ x86linux32: Build CMockery LIB      ----------"
	$(MAKE) -C CMockeryLib LIB_SYS=32
	@echo "------------ x86linux32: Build CMockery Success  ----------"
	@echo ""
	@echo "------------ x86linux32: Build CFake LIB         ----------"
	$(MAKE) -C ../CFakeLib/Source x86linux32
	@echo "------------ x86linux32: Build CFake Success     ----------"
	@echo ""
	@echo "------------ x86linux32: Build Quick Test Case   ----------"
	$(MAKE) -f Test_Build.mk TEST_SYS=32
	@echo "------------ x86linux32: Build Test Case Success ----------"
	@echo ""
	@echo "------------ x86linux32: Run Quick Test Case     ----------"
	./AutoTest
	@echo "------------ x86linux32: Run Test Case Success   ----------"
	@echo ""

.PHONY: x86linux64
x86linux64:
	@echo ""
	@echo "------------ x86linux64: Build CMockery LIB      ----------"
	$(MAKE) -C CMockeryLib LIB_SYS=64
	@echo "------------ x86linux64: Build CMockery Success  ----------"
	@echo ""
	@echo "------------ x86linux64: Build CFake LIB         ----------"
	$(MAKE) -C ../CFakeLib/Source x86linux64
	@echo "------------ x86linux64: Build CFake Success     ----------"
	@echo ""
	@echo "------------ x86linux64: Build Quick Test Case   ----------"
	$(MAKE) -f Test_Build.mk TEST_SYS=64
	@echo "------------ x86linux64: Build Test Case Success ----------"
	@echo ""
	@echo "------------ x86linux64: Run Quick Test Case     ----------"
	./AutoTest
	@echo "------------ x86linux64: Run Test Case Success   ----------"
	@echo ""

.PHONY: clean
clean:
	@echo "------------ Clean Quick Test Case    ----------"
	$(MAKE) -C CMockeryLib clean
	$(MAKE) -C ../CFakeLib/Source clean
	$(RM) AutoTest*
	$(RM) *.o
	$(RM) *.d
	@echo "------------ Clean Test Case Success  ----------"

## help
.PHONY: help
help:
	@echo ""
	@echo "################### CFake Quick Test GNU makefile ######################"
	@echo "# Usage:                                                               #"
	@echo "#  make [help] | [clean] | [<target>]                                  #"
	@echo "#                                                                      #"
	@echo "# help:                                                                #"
	@echo "#  Help from this Makefile                                             #"
	@echo "#                                                                      #"
	@echo "# Clean:                                                               #"
	@echo "#  clean             Clean objects, depend files, etc                  #"
	@echo "#                                                                      #"
	@echo "# Targets (Unit Tests):                                                #"
	@echo "#  x86win32          Build and run quick test for windows 32 system    #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "#  x86win64          Build and run quick test for windows 64 system    #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "#  x86linux32        Build and run quick test for linux 32 system      #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "#  x86linux64        Build and run quick test for linux 64 system      #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "########################################################################"
