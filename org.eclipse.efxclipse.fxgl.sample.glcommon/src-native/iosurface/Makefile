OBJS += IOSurfaceGLContext.o
LIBS += -framework OpenGL
LIBS += -framework Cocoa
LIBS += -lobjc
EXT := dylib

%.o:	$(MAKEFILE_DIR)%.cpp
	@echo '[COMPILE] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)  $(INCLUDES)  -o $@ $<
	@echo ' '

%.o:	$(MAKEFILE_DIR)%.m
	@echo '[COMPILE] $<'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(MFLAGS)  $(INCLUDES)  -o $@ $<
	@echo ' '