#include "cgl.h"

#include "../namespace.h"
#include "../GLSurface.h"

#include "CGLGLSurface.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLContext.h>
#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLDevice.h>

#include <iostream>
#include <string>

using namespace std;

void checkErr(CGLError cglError, string msg) {
	switch (cglError) {
	case kCGLNoError:
		break;
	case kCGLBadAttribute:
		cerr << "[CGLError] 10000 kCGLBadAttribute: " << msg << " (Invalid pixel format attribute.) "<< endl;
		break;
	case kCGLBadProperty:
		cerr << "[CGLError] 10001 kCGLBadProperty: " << msg << " (Invalid renderer property.) " << endl;
		break;
	case kCGLBadPixelFormat:
		cerr << "[CGLError] 10002 kCGLBadPixelFormat: " << msg << " (Invalid pixel fomat object.) " << endl;
		break;
	case kCGLBadRendererInfo:
		cerr << "[CGLError] 10003 kCGLBadRendererInfo: " << msg << " (Invalid renderer information object.)" << endl;
		break;
	case kCGLBadContext:
		cerr << "[CGLError] 10004 kCGLBadContext: " << msg << " (Invalid context object.)" << endl;
		break;
	case kCGLBadDrawable:
		cerr << "[CGLError] 10005 kCGLBadDrawable: " << msg << " (Invalid drawable.) " << endl;
		break;
	case kCGLBadDisplay:
		cerr << "[CGLError] 10006 kCGLBadDisplay: " << msg << "(Invalid display.)" << endl;
		break;
	case kCGLBadState:
		cerr << "[CGLError] 10007 kCGLBadState: " << msg << "(Invalid context state.)" << endl;
		break;
	case kCGLBadValue:
		cerr << "[CGLError] 10008 kCGLBadValue: " << msg << "(Invalid numerical value.) " << endl;
		break;
	case kCGLBadMatch:
		cerr << "[CGLError] 10009 kCGLBadMatch: " << msg << "(Invalid share context.) " << endl;
		break;
	case kCGLBadEnumeration:
		cerr << "[CGLError] 10010 kCGLBadEnumeration: " << msg << "(Invalid share enumerant. This error is caused by the use of an invalid constant.) " << endl;
		break;
	case kCGLBadOffScreen:
		cerr << "[CGLError] 10011 kCGLBadOffScreen: " << msg << "(Invalid off-screen drawable.) " << endl;
		break;
	case kCGLBadFullScreen:
		cerr << "[CGLError] 10012 kCGLBadFullScreen: " << msg << "(Invalid full-screen drawable.) " << endl;
		break;
	case kCGLBadWindow:
		cerr << "[CGLError] 10013 kCGLBadWindow: " << msg << "(Invalid window.) " << endl;
		break;
	case kCGLBadAddress:
		cerr << "[CGLError] 10014 kCGLBadAddress: " << msg << "(Invalid memory address. This error occurs when you pass an invalid pointer into a function that requires a non-NULL memory address.) " << endl;
		break;
	case kCGLBadCodeModule:
		cerr << "[CGLError] 10015 kCGLBadCodeModule: " << msg << "(Invalid code module.) " << endl;
		break;
	case kCGLBadAlloc:
		cerr << "[CGLError] 10016 kCGLBadAlloc: " << msg << "(Invalid memory allocation. This error occurs when CGL is unable to allocate memory.) " << endl;
		break;
	case kCGLBadConnection:
		cerr << "[CGLError] 10017 kCGLBadConnection: " << msg << "(Invalid Core Graphics connection.) " << endl;
		break;
	default:
		cerr << "Error occured " << msg << ": " << cglError << endl;
		break;
	}
	cerr << flush;

}

__FXGL_NAMESPACE_USING
using namespace internal;
using namespace cgl;

long InternalGLSurface::RegisterSurface(jobject obj) {
	GLSurface::apiMutex.lock();
	if (GLSurface::javaVM == NULL) {
		cerr << "fxgl not initialized" << endl;
		throw;
	}
	long surfaceId = GLSurface::nextSurfaceId++;
	GLSurface* surface = new CGLGLSurface(obj);
	(*GLSurface::Surfaces)[surfaceId] = surface;
	GLSurface::apiMutex.unlock();
	return surfaceId;
}
