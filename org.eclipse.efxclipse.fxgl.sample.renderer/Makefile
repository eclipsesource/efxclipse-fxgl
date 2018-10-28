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
FXGL_DIR = $(PROJECT_DIR)../org.eclipse.efxclipse.fxgl/
SOURCE_DIR = $(PROJECT_DIR)src-native/
NATIVE_DIR = $(PROJECT_DIR)native/

include $(FXGL_DIR)functions.mk

CXXFLAGS += -std=c++11 -fPIC

FXGL_LIB_DIR = $(FXGL_DIR)native/
FXGL_INCLUDE_DIR = $(FXGL_DIR)src-native/

JNI_INCLUDE_DIR = $(PROJECT_DIR)../JNIHeader/

INCLUDES += -I"$(FXGL_INCLUDE_DIR)"
INCLUDES += -I"$(JNI_INCLUDE_DIR)"

CXXFLAGS += -std=c++11

LIBS := -lfxgl -L"$(FXGL_LIB_DIR)"

OBJS = $(call find_objs,cpp,$(SOURCE_DIR))


vpath %.cpp $(SOURCE_DIR)

ARTIFACT := libfxgl_sample_renderer.$(EXT)

include $(FXGL_DIR)rules.mk
