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
define find_objs
	$(patsubst %.$(1),%.o,$(notdir $(wildcard $(2)*.$(1))))
endef

ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

OS := $(shell uname)
ifeq ($(OS),Darwin)
	EXT := dylib
endif
ifeq ($(OS),Linux)
	EXT := so
endif