#ifndef _CGL_H
#define _CGL_H

#define GL_SILENCE_DEPRECATION
#include <OpenGL/CGLTypes.h>
#include <string>


void checkErr(CGLError cglError, std::string msg);

extern "C" void *getCGLContextObj(void *nsJContext);
extern "C" void *createIOSurface(int width, int height);

#endif
