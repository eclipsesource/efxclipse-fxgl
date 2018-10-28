#ifndef _CGLGLSURFACE_H
#define _CGLGLSURFACE_H

#include "../namespace.h"
#include "../GLContext.h"
#include "../GLSurface.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/CGLIOSurface.h>
#include <IOSurface/IOSurface.h>

#include <jni.h>

__FXGL_NAMESPACE_BEGIN
namespace cgl {


class CGLGLSurface : public GLSurface {
public:
	CGLGLSurface(jobject obj);
	virtual ~CGLGLSurface();

	virtual Texture GetNextTexture();
	virtual void SwapTexture();
	virtual GLContext* CreateGLContext();


private:
	GLContext *sharedContext;
	GLContext *outerContext;

	IOSurfaceRef ioSurface;
	unsigned int ioSurfaceWidth;
	unsigned int ioSurfaceHeight;

	GLuint sharedTexId;
	GLuint nextTexture;
	GLuint fbo;

	GLuint sharedContextReadFbo;
	GLuint sharedContextTargetFbo;


	GLuint sharedContextIOSurfaceTexId;

};

}
__FXGL_NAMESPACE_END


#endif
