#include "../namespace.h"
#include "../GLContext.h"
#include "../GLSurface.h"

#include "CGLGLSurface.h"
#include "CGLGLContext.h"

#include "cgl.h"

#include <cstdint>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/CGLContext.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLDevice.h>
#include <OpenGL/CGLIOSurface.h>

#include <IOSurface/IOSurface.h>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFNumber.h>

#include <jni.h>

#include <iostream>
using namespace std;


__FXGL_NAMESPACE_USING
using namespace cgl;

CGLGLSurface::CGLGLSurface(jobject obj) : GLSurface(obj) {

	void* nsContextHandle = (void*) GetContextHandle();
	void* cglContextHandle = (void*) getCGLContextObj(nsContextHandle);

	sharedContext = new CGLGLContext(cglContextHandle);

	outerContext = new CGLGLContext(NULL);

	GLuint ioSurfaceId;


	fbo = 0;
	sharedTexId = 0;
	nextTexture = 0;

	ioSurface = NULL;
	ioSurfaceHeight = 0;
	ioSurfaceWidth = 0;

	sharedContextReadFbo = 0;
	sharedContextTargetFbo = 0;


	sharedContextIOSurfaceTexId = 0;



	// init shared context
//	sharedContext->SetCurrent();
//	glGenFramebuffers(1, &sharedContextReadFbo);
//	glGenFramebuffers(1, &sharedContextTargetFbo);

}

CGLGLSurface::~CGLGLSurface() {
	cerr << "CGLGLSurface Destructor" << endl;
	if (ioSurface != NULL) {
		releaseIOSurface(ioSurface);
	}

	delete outerContext;
	delete sharedContext;
}

Texture CGLGLSurface::GetNextTexture() {
	//IOSurfaceLock(ioSurface, 0, 0);

	Texture t = GLSurface::GetNextTexture();

	// ensure context is current
	outerContext->SetCurrent();

	outerContext->CheckGLError("GetNextTexture()");


	sharedTexId = t.textureId;

	if (ioSurfaceWidth != t.width || ioSurfaceHeight != t.height) {
		if (ioSurface != NULL) {
			releaseIOSurface(ioSurface);
		}
		ioSurface = createIOSurface(t.width, t.height);

		ioSurfaceWidth = t.width;
		ioSurfaceHeight = t.height;

		//cerr << "Created IOSurface: " << ioSurface << " id:" << id << " size:(" << t.width << "x" << t.height << ")" << endl;

		if (nextTexture == 0) {
			glGenTextures(1, &nextTexture);
			outerContext->CheckGLError("glGenTextures();");
		}


		CGLContextObj outerCtx = ((CGLGLContext*)outerContext)->contextObj;
		CGLContextObj sharedCtx = ((CGLGLContext*)sharedContext)->contextObj;

		outerContext->SetCurrent();

		glBindTexture(GL_TEXTURE_RECTANGLE, nextTexture);
		outerContext->CheckGLError("glBindTexture(GL_TEXTURE_RECTANGLE, nextTexture);");
		checkErr(
			CGLTexImageIOSurface2D(outerCtx, GL_TEXTURE_RECTANGLE, GL_RGBA,
						t.width, t.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, ioSurface, 0), "CGLTexImageIOSurface2D outerContext");
		outerContext->CheckGLError("CGLTexImageIOSurface2D outerContext");
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);
		outerContext->CheckGLError("glBindTexture(0);");

		sharedContext->SetCurrent();
		if (sharedContextIOSurfaceTexId == 0) {
			glGenTextures(1, &sharedContextIOSurfaceTexId);
		}

		glEnable(GL_TEXTURE_RECTANGLE);
		sharedContext->CheckGLError("glEnable(GL_TEXTURE_RECTANGLE);");
		glBindTexture(GL_TEXTURE_RECTANGLE, sharedContextIOSurfaceTexId);
		sharedContext->CheckGLError("glBindTexture(GL_TEXTURE_RECTANGLE, sharedContextIOSurfaceTexId);");
		checkErr(
			CGLTexImageIOSurface2D(sharedCtx, GL_TEXTURE_RECTANGLE, GL_RGBA,
						t.width, t.height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, ioSurface, 0), "CGLTexImageIOSurface2D sharedContext");
		sharedContext->CheckGLError("CGLTexImageIOSurface2D sharedContext");
		glBindTexture(GL_TEXTURE_RECTANGLE, 0);
		sharedContext->CheckGLError("glBindTexture(0);");

//    GL_BGRA                         GL_UNSIGNED_INT_8_8_8_8_REV             GL_RGBA                 GL_RGBA8

		/*
		GLenum target = GL_TEXTURE_RECTANGLE;
		GLenum internal_format = GL_RGBA;
		GLsizei width = t.width;
		GLsizei height = t.height;
		GLenum format = GL_BGRA;
		GLenum type = GL_UNSIGNED_INT_8_8_8_8_REV;
		IOSurfaceRef ioSurface = this->ioSurface;
		GLuint plane = 0;

		CGLEnable(((CGLGLContext *)outerContext)->contextObj, kCGLCEStateValidation);


		glBindTexture(target, nextTexture);
		outerContext->CheckGLError("glBindTexture();");

//		glTexImage2D(target, 0, internal_format, width, height, 0, format, type, 0);
//		outerContext->CheckGLError("glTexImage2D");

		cerr << "calling CGLTexImageIOSurface2D(" <<
				"ctx: " << ctx << ", " <<
				"target: " << target << ", " <<
				"internal_format: " << internal_format << ", " <<
				"width: " << width << ", " <<
				"height: " << height << ", " <<
				"format: " << format << ", " <<
				"type: " << type << ", " <<
				"ioSurface: " << ioSurface << ", " <<
				"plane: " << plane <<
				")" << endl;

		checkErr(CGLTexImageIOSurface2D(ctx, target, internal_format,
				width, height, format, type, ioSurface, plane), "CGLTexImageIOSurface2D in GetNextTexture()");
		outerContext->CheckGLError("CGLTexImageIOSurface2D");


		glBindTexture(GL_TEXTURE_RECTANGLE, 0);
		outerContext->CheckGLError("glBindTexture(0);");
		*/
	}


	Texture result;
	result.width = t.width;
	result.height = t.height;
	result.textureId = nextTexture;

	//IOSurfaceUnlock((ioSurface, 0, 0));

	outerContext->SetCurrent();
	return result;
}

void CGLGLSurface::SwapTexture() {
	outerContext->SetCurrent();
	glFlush();

	//transfer texture
	// for now we need to copy the io surface texture to the jfx shared texture
	// because the GL_TEXTURE_RECTANGLE (iosurface) cannot be bound to the GL_TEXTURE_2D (java)
	// also the framebuffer copy api below is actually GL3 but seems to work within the GL2 context

	sharedContext->SetCurrent();

	GLuint fbos[2];

	glGenFramebuffers(2, &fbos[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sharedContextIOSurfaceTexId, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, sharedTexId, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbos[0]);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbos[1]);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlitFramebuffer(0, 0, ioSurfaceWidth, ioSurfaceHeight, 0, 0, ioSurfaceWidth, ioSurfaceHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glFlush();

	glDeleteFramebuffers(2, &fbos[0]);


	GLSurface::SwapTexture();

	// ensure outer context is active
	outerContext->SetCurrent();
}

GLContext* CGLGLSurface::CreateGLContext() {
	return outerContext;
}
