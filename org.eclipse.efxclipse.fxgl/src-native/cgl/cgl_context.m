#define GL_SILENCE_DEPRECATION

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>
#import <IOSurface/IOSurface.h>

IOSurfaceRef createIOSurface(int width, int height) {
	NSDictionary* dict = [NSDictionary dictionaryWithObjectsAndKeys:
					  [NSNumber numberWithInt: width], kIOSurfaceWidth,
					  [NSNumber numberWithInt: height], kIOSurfaceHeight,
					  [NSNumber numberWithInt:4], kIOSurfaceBytesPerElement,
					  nil];
	IOSurfaceRef ioSurface = IOSurfaceCreate((CFDictionaryRef)dict);
	CFRelease(dict);
	return ioSurface;
}

void releaseIOSurface(IOSurfaceRef ioSurface) {
	CFRelease(ioSurface);
}

void *getCGLContextObj(void *nsJContext) {
	NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;
	return (void *) nsContext.CGLContextObj;
}