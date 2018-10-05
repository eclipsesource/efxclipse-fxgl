#include <GL/glew.h>

#include "X11GLContext.h"


#include <iostream>
using namespace std;

using namespace org::eclipse::efxclipse::fxgl::sample::glcommon;
using namespace glx;

static glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
static glXMakeContextCurrentARBProc glXMakeContextCurrentARB   = (glXMakeContextCurrentARBProc)   glXGetProcAddressARB( (const GLubyte *) "glXMakeContextCurrent"      );

X11GLContext::X11GLContext(int major, int minor, long sharedHandle) {

	display = XOpenDisplay( NULL );

	static int visualAttribs[] = { None };
	int fbConfigsCount = 0;
	GLXFBConfig* fbConfigs = glXChooseFBConfig( display, DefaultScreen(display), visualAttribs, &fbConfigsCount );

	int context_attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, major,
			GLX_CONTEXT_MINOR_VERSION_ARB, minor,
			GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
	};

	glxContext = glXCreateContextAttribsARB( display, fbConfigs[0], (GLXContext) ((void*) sharedHandle), True, context_attribs);

	int pbufferAttribs[] = {
	    GLX_PBUFFER_WIDTH,  32,
	    GLX_PBUFFER_HEIGHT, 32,
	    None
	};
	pBuffer = glXCreatePbuffer( display, fbConfigs[0], pbufferAttribs );

	// clean up:
	XFree( fbConfigs );
	XSync( display, False );
}

void X11GLContext::MakeCurrent() {
	glXMakeContextCurrentARB(display, pBuffer, pBuffer, glxContext);
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to init glew" << endl;
		exit(-1);
	}
}

long X11GLContext::GetHandle() {
	return (long) glxContext;
}

void X11GLContext::Dispose() {
	glXMakeCurrent( display, 0, 0 );
	glXDestroyPbuffer(display, pBuffer);
	glXDestroyContext(display, glxContext);
	XCloseDisplay(display);
}


GLContext::GLContext() {

}

GLContext::~GLContext() {

}

void GLContext::Dispose() {

}

long GLContext::GetHandle() {
	return 0;
}

void GLContext::MakeCurrent() {

}

GLContext* GLContext::Create(int major, int minor) {
	glx::X11GLContext *ctx = new glx::X11GLContext(major, minor, 0);
	return (GLContext*) ctx;
}

GLContext* GLContext::CreateShared(int major, int minor, long sharedHandle) {
	glx::X11GLContext *ctx = new glx::X11GLContext(major, minor, sharedHandle);

	return (GLContext*) ctx;
}
