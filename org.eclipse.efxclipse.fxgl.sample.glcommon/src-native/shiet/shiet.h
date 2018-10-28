#ifndef SHIET_H
#define SHIET_H

#include "jni.h"

extern "C" {

void *createPixelFormat();
void deletePixelFormat(void *pixelFormat);
void *getPixelFormat(void *nsJContext);

void *createContext(void *sharedContext, void *view, void *pixelFormat, void *viewNotReady);
jboolean makeCurrentContext(void *njsJContext);
jboolean clearCurrentContext(void *nsJContext);
jboolean deleteContext(void *nsJContext);
jboolean flushBuffer(void *nsJContext);

void setSwapInterval(void *nsJContext, int swapInterval);

}
#endif
