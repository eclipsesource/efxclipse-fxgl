#define GL_SILENCE_DEPRECATION

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>

void *createIOSurface(int width, int height) {
	NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
					  [NSNumber numberWithInt: width], kIOSurfaceWidth,
					  [NSNumber numberWithInt: height], kIOSurfaceHeight,
					  [NSNumber numberWithInt:4], kIOSurfaceBytesPerElement,
					  nil];
	void *ioSurface = IOSurfaceCreate((CFDictionaryRef)dict);
	CFRelease(dict);
	return ioSurface;
}

void *getCGLContextObj(void *nsJContext) {
	NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;
	return (void *) nsContext.CGLContextObj;
}