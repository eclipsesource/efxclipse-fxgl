OBJS += CocoaGLContext.o  shiet.o
LIBS := -framework OpenGL
LIBS += -framework Cocoa
LIBS += -lobjc
LIBS += -ObjC

EXT := dylib


%.o:	$(MAKEFILE_DIR)%.cpp
	@echo '[COMPILE] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)  $(INCLUDES)  -o $@ $<
	@echo ' '

%.o:	$(MAKEFILE_DIR)%.m
	@echo '[COMPILE] $<'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(MFLAGS)  $(INCLUDES)  -o $@ $<
	@echo ' '