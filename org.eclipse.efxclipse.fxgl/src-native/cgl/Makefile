OBJS += CGLGLContext.o CGLGLSurface.o cgl.o cgl_context.o
LIBS := -framework OpenGL
LIBS += -framework Cocoa
LIBS += -framework IOSurface
LIBS += -framework CoreFoundation
LIBS += -lobjc
EXT := dylib

%.o:	$(MAKEFILE_DIR)%.cpp
	@echo '[COMPILE] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)  $(INCLUDES)  -o $@ $<
	@echo ' '

%.o:	$(MAKEFILE_DIR)%.m
	@echo '[COMPILE] $<'
	$(CXX) -c $(CFLAGS)  $(MFLAGS)  $(INCLUDES)  -o $@ $<
	@echo ' '