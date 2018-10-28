#define GL_SILENCE_DEPRECATION

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>

void *getCGLContextObj(void *nsJContext) {
	NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;
	return (void *) nsContext.CGLContextObj;
}