#include "../include/jni.h"
#include "../include/jni_helper.h"
#include "GLSurface.h"

#include <iostream>

using namespace std;
using namespace at::bestsolution::fxgl;
using namespace events;


extern "C" JNIEXPORT jlong JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nCreateGLSurface(JNIEnv *env, jclass cls, jobject surfaceObj)
{
	GLSurface::Initialize(); // for now we make here sure that the system is initialised
	JavaVM* jvm;
	env->GetJavaVM(&jvm);
	cerr << "got javavm " << jvm << endl << flush;
	return GLSurface::RegisterSurface(jvm, surfaceObj);
}

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nDisposeGLSurface(JNIEnv *env, jclass cls, jlong surfaceId)
{
	GLSurface::DisposeSurface(surfaceId);
}

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nFireSizeChanged(JNIEnv *env, jclass cls, jlong surfaceId)
{
	GLSurface::GetSurface(surfaceId)->FireSizeChanged();
}

extern "C" JNIEXPORT jfloat JNICALL Java_at_bestsolution_fxgl_internal_GLSurfaceAPI_nGetFPS(JNIEnv *env, jclass cls, jlong surfaceId)
{
	return (jfloat) GLSurface::GetSurface(surfaceId)->GetFPS();
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
