
#include <jni.h>
#include "Renderer.h"

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_DX9Renderer_nRun
(JNIEnv *env, jclass, jlong sharedHandle)
{
	Run((long)sharedHandle);
}
