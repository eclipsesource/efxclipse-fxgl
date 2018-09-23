
#include "../include/jni_md.h"
#include "../include/jni.h"
#include "../include/jni_helper.h"


#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <functional>
#include <map>
using namespace std;

#include <string.h> // for strdup

#include "GLSurface.h"

using namespace at::bestsolution::fxgl;
using namespace events;


long GLSurface::nextSurfaceId = 1;
map<long, GLSurface*>* GLSurface::Surfaces = NULL;
thread_local JNIEnv* GLSurface::javaEnv;

void GLSurface::Initialize() {
	if (Surfaces == NULL) {
		GLSurface::Surfaces = new map<long, GLSurface*>();
	}
}

void GLSurface::Dispose() {
	// TODO clean up all the surfaces
	delete GLSurface::Surfaces;
}

long GLSurface::RegisterSurface(JavaVM* javaVM, jobject obj) {
	long surfaceId = nextSurfaceId++;
	GLSurface* surface = new GLSurface(javaVM, obj);
	(*Surfaces)[surfaceId] = surface;
	return surfaceId;
}

void GLSurface::DisposeSurface(long surfaceId) {
	GLSurface* surface = (*Surfaces)[surfaceId];
	delete surface;
	Surfaces->erase(surfaceId);
}

GLSurface* GLSurface::GetSurface(long surfaceId) {
//	cerr << "C++ GetSurface(" << surfaceId << ")" << endl;
	return (*Surfaces)[surfaceId];
}

long GLSurface::GetContextHandle() {
	cerr << "C++ GetContextHandle()" << endl;

	JNIEnv* env = GetJavaEnv();



	jclass cls = env->GetObjectClass(javaObj);
	cerr << "using cls " << cls << endl;

	jmethodID methodID = env->GetMethodID(cls, "getContextHandle", "()J");
	cerr << "calling methodID " << methodID << endl;
	return env->CallLongMethod(javaObj, methodID);
}



JNIEnv* GLSurface::GetJavaEnv() {
	//cerr << "GetJavaEnv()" << endl << flush;
	if (javaEnv == NULL) {
		cerr << "Acquiring Java Environment..." << endl << flush;
		int stat = javaVM->GetEnv((void **)&javaEnv, JNI_VERSION_1_6);
		if (stat == JNI_EDETACHED) {
			cerr << " Attaching new thread to jvm" << endl;
			ostringstream os;
			os << "C++ Thread id:" << this_thread::get_id();
			JavaVMAttachArgs args;
			args.version = JNI_VERSION_1_6;
			args.name = strdup(os.str().c_str()); // java thread name
			args.group = NULL; // java thread group
			jint attachResult = javaVM->AttachCurrentThreadAsDaemon((void **) &javaEnv, &args);
			if (JNI_OK == attachResult) {
				cerr << " successfully attached thread to java" << endl;
			}
			else {
				cerr << " fatality! could not attach thread to java!" << endl << flush;
			}
		}
		else {
			cerr << " found java thread jni env" << endl;
		}
	}
	return javaEnv;
}

void GLSurface::ReleaseThread() {
	if (JNI_OK == javaVM->DetachCurrentThread()) {
		cerr << "successfully released thread" << endl;
	}
	else {
		cerr << "Error releasing thread!" << endl;
	}
}


Texture GLSurface::GetNextTexture() {
	JNIEnv* env = GetJavaEnv();

	jobject tex = env->CallObjectMethod(javaObj, javaMethodGetNextTexture);

	// TODO PERF those results may be cached
	jclass texCls = env->GetObjectClass(tex);
	jfieldID fieldTextureId = env->GetFieldID(texCls, "textureId", "J");
	jfieldID fieldWidth = env->GetFieldID(texCls, "width", "I");
	jfieldID fieldHeight = env->GetFieldID(texCls, "height", "I");

	Texture m;
	// map
	m.textureId = env->GetLongField(tex, fieldTextureId);
	m.width = env->GetIntField(tex, fieldWidth);
	m.height = env->GetIntField(tex, fieldHeight);

	return m;


}

void GLSurface::SwapTexture() {
	//cerr << "C++ SwapTexture()" << endl;

	GetJavaEnv()->CallVoidMethod(javaObj, javaMethodSwapTexture);


	std::chrono::milliseconds delta(1000);

	std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch());

	fpsFrameCount++;

	if (now - fpsLastTime > delta) {
		fps = fpsFrameCount;
		fpsFrameCount = 0;
		fpsLastTime = now;
		//cout << "UPDATING FPS " << fps << endl;
	}

}

void GLSurface::SubscribeSizeChanged(std::function<void(int)> callback) {
	// TODO support list of subscriptions = ?
	sizeChangedCallback = callback;
}

void GLSurface::FireSizeChanged() {
	cerr << "C++ FireSizeChanged()" << endl;
	if (sizeChangedCallback != NULL) {
		sizeChangedCallback(1);
	}
}

float GLSurface::GetFPS() {
	return fps;
}

void GLSurface::SubscribeScrollEvent(std::function<void(events::ScrollEvent)> callback) {
	// TODO support list of subscriptions = ?
	scrollEventCallback = callback;
}

void GLSurface::SubscribeMouseEvent(std::function<void(events::MouseEvent)> callback) {
	mouseEventCallback = callback;
}

void GLSurface::FireScrollEvent(events::ScrollEvent event) {
	if (scrollEventCallback != NULL) {
		scrollEventCallback(event);
	}
}

void GLSurface::FireMouseEvent(events::MouseEvent event) {
	if (mouseEventCallback != NULL) {
		mouseEventCallback(event);
	}
}

GLSurface::GLSurface(JavaVM* javaVM, jobject obj) {
	cerr << "GLSurface constructor javaVM=" << javaVM << ", obj=" << obj << endl << flush;

	this->javaVM = javaVM;

	JNIEnv* env = GetJavaEnv();

	// protect the references from garbage collection
	this->javaObj = reinterpret_cast<jobject>( env->NewGlobalRef(obj) );
	this->javaCls = reinterpret_cast<jclass>( env->NewGlobalRef( env->GetObjectClass(javaObj) ) );

	this->javaMethodGetNextTexture = env->GetMethodID(javaCls, "GetNextTexture", "()Lat/bestsolution/fxgl/es2/GLSurface$Texture;");
	this->javaMethodGetContextHandle = env->GetMethodID(javaCls, "getContextHandle", "()J");
	this->javaMethodSwapTexture = env->GetMethodID(javaCls, "SwapTexture", "()V");
	cerr << "C++ constructor - javaVM = " << javaVM << endl << flush;
	cerr << "C++ constructor - javaObj = " << javaObj << endl << flush;
	cerr << "C++ constructor - javaCls = " << javaCls << endl << flush;
	cerr << "C++ constructor - javaMethodGetContextHandle = " << javaMethodGetContextHandle << endl << flush;
	cerr << "C++ constructor - javaMethodGetNextTexture = " << javaMethodGetNextTexture << endl << flush;
	cerr << "C++ constructor - javaMethodSwapTexture = " << javaMethodSwapTexture << endl << flush;

	fpsLastTime = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now().time_since_epoch());

	//jlong contextHandle = javaEnv->CallLongMethod(javaObj, javaMethodGetContextHandle);
	//cerr << "contextHandle = " << contextHandle << endl << flush;
}

