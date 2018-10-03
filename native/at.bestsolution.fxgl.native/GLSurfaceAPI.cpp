#include "../include/jni.h"
#include "../include/jni_helper.h"
#include "GLSurface.h"

#include <iostream>

using namespace std;
using namespace at::bestsolution::fxgl;
using namespace events;

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nInitialize(JNIEnv *env, jclass cls) {
	cerr << "nInitialize(()" << endl;
	JavaVM* jvm;
	env->GetJavaVM(&jvm);
	internal::InternalGLSurface::Initialize(jvm);
}


extern "C" JNIEXPORT jlong JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nCreateGLSurface(JNIEnv *env, jclass cls, jobject surfaceObj)
{
	return internal::InternalGLSurface::RegisterSurface(surfaceObj);
}

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nDisposeGLSurface(JNIEnv *env, jclass cls, jlong surfaceId)
{
	internal::InternalGLSurface::DestroySurface(surfaceId);
}

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nFireSizeChanged(JNIEnv *env, jclass cls, jlong surfaceId)
{
	GLSurface::GetSurface(surfaceId)->FireSizeChanged();
}

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nFireScrollEvent(JNIEnv *env, jclass cls, jlong surfaceId, jobject event)
{
		jclass eventCls = env->GetObjectClass(event);
		jfieldID fieldDeltaX = env->GetFieldID(eventCls, "deltaX", "F");
		jfieldID fieldDeltaY = env->GetFieldID(eventCls, "deltaY", "F");

		ScrollEvent ev;
		ev.deltaX = env->GetFloatField(event, fieldDeltaX);
		ev.deltaY = env->GetFloatField(event, fieldDeltaY);

		GLSurface::GetSurface(surfaceId)->FireScrollEvent(ev);
}
