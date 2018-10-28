#define GL_SILENCE_DEPRECATION

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>

#import <OpenGL/gl.h>
#import <OpenGL/CGLTypes.h>


//#import "../macosx-window-system.h"
//#import "com_sun_prism_es2_GLPixelFormat.h"
//#import "com_sun_prism_es2_GLPixelFormat_Attributes.h"

#import "jni.h"

void *createPixelFormat() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSOpenGLPixelFormatAttribute attribs[] = {
			NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
			0
	};
	
	NSOpenGLPixelFormat *fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
	
	[pool release];
	return fmt;
}
/*
void *createPixelFormat(jint *ivalues) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSOpenGLPixelFormatAttribute attribs[20];
    int index = 0;

    if (ivalues == NULL) {
        return NULL;
    }
    attribs[index++] = NSOpenGLPFAAccelerated;

    //if (ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_DOUBLEBUFFER] != 0) {
        attribs[index++] = NSOpenGLPFADoubleBuffer;
    //}

    attribs[index++] = NSOpenGLPFAAlphaSize;
    attribs[index++] = ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_ALPHA_SIZE];

    attribs[index++] = NSOpenGLPFAColorSize;
    attribs[index++] =
            ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_RED_SIZE]
            + ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_GREEN_SIZE]
            + ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_BLUE_SIZE]
            + ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_ALPHA_SIZE];

    attribs[index++] = NSOpenGLPFADepthSize;
    attribs[index++] = ivalues[com_sun_prism_es2_GLPixelFormat_Attributes_DEPTH_SIZE];


    // Lets OpenGL know this context is offline renderer aware.
    attribs[index++] = NSOpenGLPFAAllowOfflineRenderers;

    // Zero-terminate
    attribs[index++] = 0;

    NSOpenGLPixelFormat *fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes : attribs];

    [pool release];
    return fmt;
}
*/

void deletePixelFormat(void *pixelFormat) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSOpenGLPixelFormat *fmt = (NSOpenGLPixelFormat *) pixelFormat;
    [fmt release];
    [pool release];
}

void *createContext(void *shareContext, void *view, void *pixelFormat,
        int *viewNotReady) {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    /*
    NSView *nsView = NULL;
    NSObject *nsObj = (NSObject *) view;

    if (nsObj != NULL && [nsObj isKindOfClass : [NSView class]]) {
        nsView = (NSView *) nsObj;
    }

    if (nsView != NULL) {
        jboolean viewReady = true;

        if ([nsView lockFocusIfCanDraw] == NO) {
            viewReady = false;
        } else {
            NSRect frame = [nsView frame];
            if ((frame.size.width == 0) || (frame.size.height == 0)) {
                [nsView unlockFocus];
                viewReady = false;
            }
        }

        if (!viewReady) {
            if (viewNotReady != NULL) {
                *viewNotReady = 1;
            }

            // the view is not ready yet
            [pool release];
            return NULL;
        }
    }

    */
    
    NSOpenGLContext *nsContext = [[NSOpenGLContext alloc]
            initWithFormat : (NSOpenGLPixelFormat *) pixelFormat
            shareContext : (NSOpenGLContext*) shareContext];

  /*  if (nsContext != nil) {
        if (nsView != nil) {
            [nsContext setView : nsView];
            [nsView unlockFocus];
        }
    }*/

    [pool release];
    return nsContext;
}

void *getCurrentContext() {
    NSOpenGLContext *nsContext = NULL;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    nsContext = [NSOpenGLContext currentContext];
    [pool release];
    return nsContext;
}

void *getPixelFormat(void *nsJContext) {
	NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;
	
	//void* pointer = nsContext.cglContextObj;
	
	NSLog (@"cglContextObj %li", (long) nsContext.CGLContextObj);
	
	NSOpenGLPixelFormat *fmt = [nsContext pixelFormat];
	
	NSLog (@"bla bla %li", (long) fmt);
	
	
	return fmt;
}

jboolean makeCurrentContext(void *nsJContext) {
    NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [nsContext makeCurrentContext];
    [pool release];
    return JNI_TRUE;
}

jboolean clearCurrentContext(void *nsJContext) {
    NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSOpenGLContext *currentNSContext = [NSOpenGLContext currentContext];
    if (currentNSContext != nsContext) {
        [nsContext makeCurrentContext];
    }
    [NSOpenGLContext clearCurrentContext];
    [pool release];
    return JNI_TRUE;
}

jboolean deleteContext(void *nsJContext) {
    NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    [nsContext clearDrawable];
    [nsContext release];
    [pool release];
    return JNI_TRUE;
}

jboolean flushBuffer(void *nsJContext) {
    NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    [nsContext flushBuffer];
    [pool release];
    return JNI_TRUE;
}

void setSwapInterval(void *nsJContext, int swapInterval) {
    NSOpenGLContext *nsContext = (NSOpenGLContext *) nsJContext;
    [nsContext setValues : &swapInterval forParameter : NSOpenGLCPSwapInterval];
}