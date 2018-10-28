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

CXXFLAGS += -std=c++11 -fPIC

FXGL_LIB_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl/native/
FXGL_INCLUDE_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl/src-native/

JNI_INCLUDE_DIR = $(PROJECT_DIR)../JNIHeader/

INCLUDES += -I"$(FXGL_INCLUDE_DIR)"
INCLUDES += -I"$(JNI_INCLUDE_DIR)"

CXXFLAGS += -std=c++11

LIBS := -lfxgl -L"$(FXGL_LIB_DIR)"

OBJS = Renderer.o RendererAPI.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

COPY_FILES = $(NATIVE_DIR)libfxgl_sample_renderer.dylib $(NATIVE_DIR)libfxgl_sample_renderer.so
$(info $(COPY_FILES))

all:	libfxgl_sample_renderer.dylib libfxgl_sample_renderer.so $(COPY_FILES)

$(NATIVE_DIR)%:
	cp $< $@

%.so:		$(OBJS)
	@echo '[Linking] $@'
	$(CXX) $(LIBS) -shared -o $@ $^
	@echo ' '
%.dylib:	$(OBJS)
	@echo '[Linking] $@'
	$(CXX) $(LIBS) -shared -o $@ $^
	@echo ' '

%.o:	$(SOURCE_DIR)%.cpp
	@echo '[Compile] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -o $@ $<
	@echo ' '


$(NATIVE_DIR)libfxgl_sample_renderer.dylib: libfxgl_sample_renderer.dylib
$(NATIVE_DIR)libfxgl_sample_renderer.so: libfxgl_sample_renderer.so




clean:
	rm -fr libfxgl_sample_renderer.dylib libfxgl_sample_renderer.so $(OBJS)


.PHONY: clean all