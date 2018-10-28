#
#  Copyright (c) 2018 BestSolution and Others. All rights reserved.
#
#  This program and the accompanying materials are made available under the
#  terms of the Eclipse Public License v. 2.0, which is available at
#  http://www.eclipse.org/legal/epl-2.0.
#
#  This Source Code may also be made available under the following Secondary
#  Licenses when the conditions for such availability set forth in the
#  Eclipse Public License v. 2.0 are satisfied: GNU General Public License,
#  version 2 with the GNU Classpath Exception, which is available at
#  https://www.gnu.org/software/classpath/license.html.
# 
#  SPDX-License-Identifier: EPL-2.0 OR GPL-2.0 WITH Classpath-exception-2.0
#
PROJECT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
SOURCE_DIR = $(PROJECT_DIR)src-native/
NATIVE_DIR = $(PROJECT_DIR)native/

MAKEFILE_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

JNI_INCLUDE_DIR = $(PROJECT_DIR)../JNIHeader/

INCLUDES += -I"$(JNI_INCLUDE_DIR)"

CXXFLAGS += -fPIC

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif



all: link copy
	@echo 'Produced $(ARTIFACT)'
	@echo ''

clean:
	rm -fr $(ARTIFACT) $(OBJS)

%.o:	$(SOURCE_DIR)/%.cpp
	@echo '[Compile] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -o $@ $<
	@echo ' '


OBJS += GLContext.o


OS := $(shell uname)
ifeq ($(OS),Darwin)
	include $(SOURCE_DIR)/cgl/Makefile
#	include $(SOURCE_DIR)/iosurface/Makefile
#	include $(SOURCE_DIR)/shiet/Makefile
endif
ifeq ($(OS),Linux)
	include $(SOURCE_DIR)/x11/Makefile
endif

ARTIFACT := libfxgl_sample_glcommon.$(EXT)


COPY_FILES = $(NATIVE_DIR)$(ARTIFACT)

$(NATIVE_DIR)$(ARTIFACT): $(ARTIFACT)

$(NATIVE_DIR)%:
	@echo 'MAKEFILE_DIR=$(MAKEFILE_DIR)'
	cp $< $@

$(ARTIFACT): $(OBJS)
	@echo '[Linking] $@'
	g++ -Wl,-rpath,\$$ORIGIN -shared $(LIBS) -o $@ $^
	@echo ' '


compile: $(OBJS)

link: $(ARTIFACT)

copy: $(COPY_FILES)

	
	
.PHONY: all clean compile link copy