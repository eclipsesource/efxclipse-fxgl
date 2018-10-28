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
OBJECT_DIR = $(PROJECT_DIR)build/obj/

include $(PROJECT_DIR)functions.mk

OBJS = $(call find_objs,cpp,$(SOURCE_DIR))

INCLUDE_PATH += -I"$(PROJECT_DIR)../JNIHeader"

INCLUDES = $(INCLUDE_PATH)

CXXFLAGS += -std=c++11 -fPIC


ifeq ($(OS),Darwin)
	OBJS += $(call find_objs,cpp,$(SOURCE_DIR)cgl/)
	OBJS += $(call find_objs,m,$(SOURCE_DIR)cgl/)
	
	LIBS := -framework OpenGL
	LIBS += -framework Cocoa
	LIBS += -framework IOSurface
	LIBS += -framework CoreFoundation
	LIBS += -lobjc

	vpath %.cpp $(SOURCE_DIR)cgl/
	vpath %.m $(SOURCE_DIR)cgl/
	
endif
ifeq ($(OS),Linux)
	
endif


ARTIFACT := libfxgl.$(EXT)

vpath %.cpp $(SOURCE_DIR)

include $(PROJECT_DIR)rules.mk
