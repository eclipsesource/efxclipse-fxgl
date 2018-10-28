
OBJS += X11GLContext.o
LIBS := -lGLX -lX11 -lGLEW
EXT := so

%.o:	$(MAKEFILE_DIR)%.cpp
	@echo '[COMPILE] $@'
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<
	@echo ' '

