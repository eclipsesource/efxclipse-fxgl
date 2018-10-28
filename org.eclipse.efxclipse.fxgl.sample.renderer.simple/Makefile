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

GLCOMMON_LIB_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl.sample.glcommon/native/
GLCOMMON_INCLUDE_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl.sample.glcommon/src-native/

RENDERER_LIB_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl.sample.renderer/native/
RENDERER_INCLUDE_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl.sample.renderer/src-native/

INCLUDES += -I"$(FXGL_INCLUDE_DIR)"
INCLUDES += -I"$(JNI_INCLUDE_DIR)"
INCLUDES += -I"$(GLCOMMON_INCLUDE_DIR)"
INCLUDES += -I"$(RENDERER_INCLUDE_DIR)"

LIBS += -lfxgl 
LIBS += -lfxgl_sample_glcommon 
LIBS += -lfxgl_sample_renderer

LDIR += -L"$(FXGL_LIB_DIR)"
LDIR += -L"$(GLCOMMON_LIB_DIR)"
LDIR += -L"$(RENDERER_LIB_DIR)"

OS := $(shell uname)
ifeq ($(OS),Darwin)
	LIBS += -framework OpenGL
	EXT := dylib
	CFLAGS += -D __APPLE__
endif
ifeq ($(OS),Linux)
	LIBS += -lGLEW -lGL
	EXT := so
endif

$(info $(LIBS))

CXXFLAGS += -std=c++11

OBJS = SimpleRenderer.o Utils.o

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

ARTIFACT = libfxgl_sample_renderer_simple.$(EXT)

COPY_FILES = $(NATIVE_DIR)$(ARTIFACT)
$(info $(COPY_FILES))

BINS = $(ARTIFACT)

all:	$(BINS) $(COPY_FILES)

$(NATIVE_DIR)%:
	cp $< $@

%.so:		$(OBJS)
	@echo '[Linking] $@'
	$(CXX) $(LIBS) $(LDIR) -shared -o $@ $^
	@echo ' '
	
%.dylib:	$(OBJS)
	@echo '[Linking] $@'
	$(CXX) $(LIBS) $(LDIR) -shared -o $@ $^
	@echo ' '

%.o:	$(SOURCE_DIR)%.cpp
	@echo '[Compile] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) -o $@ $<
	@echo ' '


$(NATIVE_DIR)libfxgl_sample_renderer_simple.dylib: libfxgl_sample_renderer_simple.dylib
$(NATIVE_DIR)libfxgl_sample_renderer_simple.so: libfxgl_sample_renderer_simple.so




clean:
	rm -fr $(BINS) $(OBJS)


.PHONY: clean all