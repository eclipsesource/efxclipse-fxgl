/*
 * Copyright (c) 2018 BestSolution and Others. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0, which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception, which is available at
 * https://www.gnu.org/software/classpath/license.html.
 *
 * SPDX-License-Identifier: EPL-2.0 OR GPL-2.0 WITH Classpath-exception-2.0
 */
#include "../include/jni_md.h"
#include "../include/jni.h"
#include "../include/jni_helper.h"


#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <functional>
#include <map>
#include <mutex>
using namespace std;

#include <string.h> // for strdup

#include "GLSurface.h"

using namespace org::eclipse::efxclipse::fxgl;
using namespace events;


using namespace internal;


mutex GLSurface::apiMutex;
JavaVM* GLSurface::javaVM = NULL;
JTextureCls* GLSurface::TextureCls = NULL;
JGLSurfaceCls* GLSurface::GLSurfaceCls = NULL;
long GLSurface::nextSurfaceId = 1;
map<long, GLSurface*>* GLSurface::Surfaces = NULL;
thread_local JNIEnv* GLSurface::javaEnv;

void InternalGLSurface::Initialize(JavaVM* javaVM) {
	GLSurface::apiMutex.lock();
	if (GLSurface::javaVM == NULL) {
		GLSurface::javaVM = javaVM;
	}
	if (GLSurface::Surfaces == NULL) {
		GLSurface::Surfaces = new map<long, GLSurface*>();
	}
	if (GLSurface::TextureCls == NULL) {
		JNIEnv* env = GLSurface::GetJavaEnv();

		GLSurface::TextureCls = new JTextureCls();
		cerr << "got env 1 " << env << endl << flush;
		jclass cls =  env->FindClass("org/eclipse/efxclipse/fxgl/es2/GLSurface$Texture");
		GLSurface::TextureCls->textureId = env->GetFieldID(cls, "textureId", "J");
		GLSurface::TextureCls->width = env->GetFieldID(cls, "width", "I");
		GLSurface::TextureCls->height = env->GetFieldID(cls, "height", "I");
	}
	if (GLSurface::GLSurfaceCls == NULL) {
		JNIEnv* env = GLSurface::GetJavaEnv();
		cerr << "got env " << env << endl << flush;
		GLSurface::GLSurfaceCls = new JGLSurfaceCls();
		jclass cls = env->FindClass("org/eclipse/efxclipse/fxgl/es2/GLSurface");
		GLSurface::GLSurfaceCls->GetContextHandle = env->GetMethodID(cls, "getContextHandle", "()J");
		GLSurface::GLSurfaceCls->GetNextTexture = env->GetMethodID(cls, "GetNextTexture", "()Lorg/eclipse/efxclipse/fxgl/es2/GLSurface$Texture;");
		GLSurface::GLSurfaceCls->SwapTexture = env->GetMethodID(cls, "SwapTexture", "()V");
	}

	GLSurface::apiMutex.unlock();
}
void InternalGLSurface::Destroy() {
	GLSurface::apiMutex.lock();
	if (GLSurface::javaVM == NULL) {
		cerr << "fxgl not initialized" << endl;
		throw;
	}
	// TODO clean up all the surfaces
	delete GLSurface::Surfaces;
	delete GLSurface::TextureCls;
	delete GLSurface::GLSurfaceCls;
	GLSurface::javaVM = NULL;
	GLSurface::apiMutex.unlock();
}

long InternalGLSurface::RegisterSurface(jobject obj) {
	GLSurface::apiMutex.lock();
	if (GLSurface::javaVM == NULL) {
		cerr << "fxgl not initialized" << endl;
		throw;
	}
	long surfaceId = GLSurface::nextSurfaceId++;
	GLSurface* surface = new GLSurface(obj);
	(*GLSurface::Surfaces)[surfaceId] = surface;
	GLSurface::apiMutex.unlock();
	return surfaceId;
}

void InternalGLSurface::DestroySurface(long surfaceId) {
	GLSurface::apiMutex.lock();
	if (GLSurface::javaVM == NULL) {
		cerr << "fxgl not initialized" << endl;
		throw;
	}
	GLSurface* surface = (*GLSurface::Surfaces)[surfaceId];
	delete surface;
	GLSurface::Surfaces->erase(surfaceId);
	GLSurface::apiMutex.unlock();
}


GLSurface* GLSurface::GetSurface(long surfaceId) {
//	cerr << "C++ GetSurface(" << surfaceId << ")" << endl;
	apiMutex.lock();
	if (GLSurface::javaVM == NULL) {
		cerr << "fxgl not initialized" << endl;
		throw;
	}
	GLSurface* surface = (*Surfaces)[surfaceId];
	apiMutex.unlock();
	return surface;
}



long GLSurface::GetContextHandle() {
	cerr << "C++ GetContextHandle()" << endl;
	JNIEnv* env = GetJavaEnv();
	return env->CallLongMethod(javaObj, GLSurfaceCls->GetContextHandle);
}

JNIEnv* GLSurface::GetJavaEnv() {
	if (!IsThreadAttached()) {
		AttachThread();
	}
	return javaEnv;
}

bool GLSurface::IsThreadAttached() {
	if (javaEnv == NULL) {
		int stat = javaVM->GetEnv((void **)&javaEnv, JNI_VERSION_1_6);
		if (stat == JNI_EDETACHED) {
			return false;
		}
		else {
			return true;
		}
	}
	else {
		return true;
	}
}

void GLSurface::AttachThread() {
	if (javaEnv == NULL) {
		cerr << "Acquiring Java Environment..." << endl << flush;
		int stat = javaVM->GetEnv((void **)&javaEnv, JNI_VERSION_1_6);
		if (stat == JNI_EDETACHED) {
			cerr << " Attaching thread to jvm" << endl;
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
	}
}

void GLSurface::ReleaseThread() {
	if (JNI_OK == javaVM->DetachCurrentThread()) {
		javaEnv = NULL;
	}
	else {
		cerr << "Error detaching thread!" << endl;
	}
}


Texture GLSurface::GetNextTexture() {
	JNIEnv* env = GetJavaEnv();

	jobject tex = env->CallObjectMethod(javaObj, GLSurfaceCls->GetNextTexture);

	Texture m;
	// map
	m.textureId = env->GetLongField(tex, TextureCls->textureId);
	m.width = env->GetIntField(tex, TextureCls->width);
	m.height = env->GetIntField(tex, TextureCls->height);

	return m;


}

void GLSurface::SwapTexture() {
	//cerr << "C++ SwapTexture()" << endl;
	GetJavaEnv()->CallVoidMethod(javaObj, GLSurfaceCls->SwapTexture);
}

void GLSurface::SubscribeSizeChanged(std::function<void(void)> callback) {
	// TODO support list of subscriptions = ?
	sizeChangedCallback = callback;
}

void GLSurface::FireSizeChanged() {
	//cerr << "C++ FireSizeChanged()" << endl;
	if (sizeChangedCallback != NULL) {
		sizeChangedCallback();
	}
}

void GLSurface::SubscribeScrollEvent(std::function<void(events::ScrollEvent)> callback) {
	// TODO support list of subscriptions = ?
	scrollEventCallback = callback;
}

//void GLSurface::SubscribeMouseEvent(std::function<void(events::MouseEvent)> callback) {
//	mouseEventCallback = callback;
//}

void GLSurface::FireScrollEvent(events::ScrollEvent event) {
	if (scrollEventCallback != NULL) {
		scrollEventCallback(event);
	}
}

//void GLSurface::FireMouseEvent(events::MouseEvent event) {
//	if (mouseEventCallback != NULL) {
//		mouseEventCallback(event);
//	}
//}

GLSurface::GLSurface(jobject obj) {
	//cerr << "GLSurface constructor  obj=" << obj << endl << flush;
	JNIEnv* env = GetJavaEnv();

	// protect the references from garbage collection
	this->javaObj = reinterpret_cast<jobject>( env->NewGlobalRef(obj) );
}

GLSurface::~GLSurface() {
	JNIEnv* env = GetJavaEnv();
	env->DeleteGlobalRef(javaObj);
}

