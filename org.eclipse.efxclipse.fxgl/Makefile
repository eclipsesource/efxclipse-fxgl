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
MAKEFILE_DIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
PROJECT_DIR = $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
SOURCE_DIR = $(PROJECT_DIR)src-native/
NATIVE_DIR = $(PROJECT_DIR)native/

OBJS = GLSurface.o GLSurfaceAPI.o GLContext.o

INCLUDE_PATH += -I"$(PROJECT_DIR)../JNIHeader"

INCLUDES = $(INCLUDE_PATH)

CXXFLAGS += -std=c++11 -fPIC

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif


OS := $(shell uname)
ifeq ($(OS),Darwin)
	include $(SOURCE_DIR)/cgl/Makefile
endif
ifeq ($(OS),Linux)
	include $(SOURCE_DIR)/x11/Makefile
endif



COPY_FILES = $(NATIVE_DIR)libfxgl.dylib $(NATIVE_DIR)libfxgl.so

all:	libfxgl.dylib libfxgl.so $(COPY_FILES)

libfxgl.so:	$(OBJS)
	$(CXX) -Wl,-rpath,\$$ORIGIN -shared $(LIBS) -o $@ $^
	
libfxgl.dylib:	$(OBJS)
	$(CXX)  -Wl,-rpath,\$$ORIGIN -shared $(LIBS) -o $@ $^

%.o:	$(SOURCE_DIR)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE_PATH) -o $@ $<




$(NATIVE_DIR)libfxgl.dylib: libfxgl.dylib
$(NATIVE_DIR)libfxgl.so: libfxgl.so

$(NATIVE_DIR)%:
	cp $< $@


clean:
	rm -fr libfxgl.dylib libfxgl.so $(OBJS)

.PHONY: all clean