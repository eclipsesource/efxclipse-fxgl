#ifndef _CGLGLCONTEXT_H_
#define _CGLGLCONTEXT_H_

#include "../namespace.h"
#include "../GLContext.h"
#include "CGLGLSurface.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/CGLTypes.h>

__FXGL_NAMESPACE_BEGIN
namespace cgl {


class CGLGLContext : public GLContext {
	friend CGLGLSurface;

public:
	CGLGLContext(void *sharedHandle);
	virtual ~CGLGLContext();

	virtual void SetCurrent();
	virtual void UnsetCurrent();
	virtual void* GetHandle();
	virtual void CheckGLError(const char* msg);

private:
	CGLContextObj contextObj;

};

}
__FXGL_NAMESPACE_END

#endif
