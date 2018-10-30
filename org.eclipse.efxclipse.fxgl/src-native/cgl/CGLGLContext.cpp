#include "CGLGLContext.h"

#include "../namespace.h"
#include "../GLContext.h"

#include "cgl.h"

#include <cstdint>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLContext.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLDevice.h>
#include <OpenGL/gl.h>

#include <iostream>
#include <string>

using namespace std;
__FXGL_NAMESPACE_USING
using namespace cgl;


CGLGLContext::CGLGLContext(void *sharedHandle) {

	CGLContextObj sharedContext = NULL;
	CGLPixelFormatObj pix;
	if (sharedHandle != NULL) {
		sharedContext = (CGLContextObj) sharedHandle;
		pix = CGLGetPixelFormat(sharedContext);
	}
	else {
		CGLPixelFormatAttribute attributes[5] = {
			//kCGLPFAPBuffer,
			kCGLPFAAccelerated,
			kCGLPFAOpenGLProfile,
			(CGLPixelFormatAttribute) kCGLOGLPVersion_GL4_Core,
			(CGLPixelFormatAttribute) 0
		};
		GLint num;
		checkErr(CGLChoosePixelFormat( attributes, &pix, &num ), "CGLChoosePixelFormat");
	}

	checkErr(CGLCreateContext( pix, sharedContext, &contextObj ), "CGLCreateContext");


	if (sharedContext == NULL) {
		checkErr(CGLDestroyPixelFormat( pix ), "CGLDestroyPixelFormat");
	}

}

void CGLGLContext::CheckGLError(const char* msg) {
	GLenum glError = glGetError();
	switch (glError) {
	case GL_NO_ERROR: break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: cerr << "[GLError] GL_INVALID_FRAMEBUFFER_OPERATION " << msg << endl; break;
	case GL_INVALID_ENUM: cerr << "[GLError] GL_INVALID_ENUM " << msg << endl; break;
	case GL_INVALID_VALUE: cerr << "[GLError] GL_INVALID_VALUE " << msg << endl; break;
	case GL_INVALID_OPERATION: cerr << "[GLError] GL_INVALID_OPERATION " << msg << endl; break;
	case GL_OUT_OF_MEMORY: cerr << "[GLError] GL_OUT_OF_MEMORY " << msg << endl; break;
	default: cerr << "[GLError] code: " << glError << " " << msg << endl;
	}

}

CGLGLContext::~CGLGLContext() {
	cerr << "CGLGLContext Destructor" << endl;
	checkErr(CGLDestroyContext( contextObj ), "CGLDestroyContext");
}

void CGLGLContext::SetCurrent() {
	checkErr(CGLSetCurrentContext( contextObj ), "CGLSetCurrentContext");
}
void CGLGLContext::UnsetCurrent() {
	checkErr(CGLSetCurrentContext( NULL ), "CGLSetCurrentContext");
}
void* CGLGLContext::GetHandle() {
	return (void*) contextObj;
}
