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

all: help

## Build all CC&S and DSPHWAPI targets
.PHONY: x86win32
x86win32:
	$(MAKE) -f CFakeBuild.mk FAKE_HW=x86 FAKE_OS=win FAKE_SYS=32

.PHONY: x86win64
x86win64:
	$(MAKE) -f CFakeBuild.mk FAKE_HW=x86 FAKE_OS=win FAKE_SYS=64

.PHONY: x86linux32
x86linux32:
	$(MAKE) -f CFakeBuild.mk FAKE_HW=x86 FAKE_OS=linux FAKE_SYS=32

.PHONY: x86linux64
x86linux64:
	$(MAKE) -f CFakeBuild.mk FAKE_HW=x86 FAKE_OS=linux FAKE_SYS=64

.PHONY: clean
clean:
	$(RM) ../libfake.a *.o *.d

## help
.PHONY: help
help:
	@echo ""
	@echo "######################## CFake GNU makefile ############################"
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
	@echo "#  x86win32          Make static library for windows 32 system         #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "#  x86win64          Make static library for windows 64 system         #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "#  x86linux32        Make static library for linux 32 system           #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "#  x86linux64        Make static library for linux 32 system           #"
	@echo "#                    on x86 hw architecture                            #"
	@echo "########################################################################"
