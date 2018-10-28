#include "namespace.h"
#include "GLContext.h"

__FXGL_NAMESPACE_USING

GLContext::GLContext() {
}

GLContext::~GLContext() {
}

void* GLContext::GetHandle() {
	return 0;
}

void GLContext::SetCurrent() {
}

void GLContext::UnsetCurrent() {
}

void GLContext::CheckGLError(const char* msg) {
}
