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
$(info Artifact: $(ARTIFACT) Objects: $(OBJS))

all:	$(ARTIFACT) copy

$(NATIVE_DIR)$(ARTIFACT): $(ARTIFACT)
	@echo '[Copying] $(ARTIFACT)'
	@cp $(ARTIFACT) $(NATIVE_DIR)$(ARTIFACT)

%.so:	$(OBJS)
	@echo '[Linking] $@ ($^)'
	@$(CXX) -Wl,-rpath,\$$ORIGIN -shared $(LIBS) $(LDIR) -o $@ $^
%.dylib:	$(OBJS)
	@echo '[Linking] $@ ($^)'
	@$(CXX)  -Wl,-rpath,\$$ORIGIN -shared $(LIBS) $(LDIR) -o $@ $^
%.dll:	$(OBJS)
	@echo '[Linking] $@ ($^)'
	@$(CXX)  -Wl,-rpath,\$$ORIGIN -shared $(LIBS) $(LDIR) -o $@ $^
%.o:	%.c
	@echo '[Compile] $@ ($<)'
	@$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(INCLUDES) $(INCLUDE_PATH) -o $@ $<
%.o:	%.cpp
	@echo '[Compile] $@ ($<)'
	@$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDES) $(INCLUDE_PATH) -o $@ $<
%.o:	%.m
	@echo '[Compile] $@ ($<)'
	@$(CXX) -c $(CFLAGS)  $(MFLAGS)  $(INCLUDES) $(INCLUDE_PATH) -o $@ $<

copy: $(NATIVE_DIR)$(ARTIFACT)

clean:
	rm -fr $(ARTIFACT) $(OBJS)
	
.PHONY: all clean
.SECONDARY: $(OBJS)