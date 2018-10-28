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
#ifndef INCLUDE_GLSURFACE_H_
#define INCLUDE_GLSURFACE_H_


#include <functional>
#include <map>
#include <chrono>
#include <mutex>

#include "MouseEvent.h"
#include "ScrollEvent.h"

#include "GLContext.h"

#include <jni.h>

#include "Texture.h"

namespace org {
namespace eclipse {
namespace efxclipse {
namespace fxgl {

namespace internal {

class InternalGLSurface {

public:
	/**
	 * Initialises the GLSurface API
	 */
	static void Initialize(JavaVM* javaVM);
	/**
	 * Disposes the GLSurface API
	 */
	static void Destroy();

	/**
	 * Registers a new javafx surface with the GLSurface API
	 * <p>
	 * To access the newly created surface use <code>GLSurface::GetSurface(surfaceId)</code>.
	 * </p>
	 * \param javaVM the java vm
	 * \param obj the java GLSurface object
	 * \return the newly created GLSurface id
	 */
	static long RegisterSurface(jobject obj);

	/**
	 * Disposes a javafx surface within the GLSurface API
	 * \param surfaceId the surface id to dispose
	 */
	static void DestroySurface(long surfaceId);

private:
	static JavaVM* javaVM;

};

struct JTextureCls {
	jfieldID textureId;
	jfieldID width;
	jfieldID height;
};

struct JGLSurfaceCls {
	jmethodID GetContextHandle;
	jmethodID GetNextTexture;
	jmethodID SwapTexture;
};

}



class GLSurface {

	friend class internal::InternalGLSurface;

public:

	virtual ~GLSurface();

	/**
	 * Returns the existing surface by surfaceId
	 * \param surfaceId the id of the surface
	 * \return The pointer to the GLSurface instance
	 */
	static GLSurface* GetSurface(long surfaceId);

	// TODO move Fire methods away from public api
	void FireSizeChanged();
	void FireScrollEvent(events::ScrollEvent event);
	//void FireMouseEvent(events::MouseEvent event);

	void SubscribeSizeChanged(std::function<void(void)> callback);
	void SubscribeInput(std::function<void> callback);
	void SubscribeRedrawRequest(std::function<void> callback);

	void SubscribeScrollEvent(std::function<void(events::ScrollEvent)> callback);

	void SubscribeMouseEvent(std::function<void(events::MouseEvent)> callback);

	/**
	 * Request the next texture to render to
	 * \return the next texture
	 */
	virtual Texture GetNextTexture();

	/**
	 * Swaps the textures
	 */
	virtual void SwapTexture();


	/**
	 * Creates a GLContext for rendering.
	 */
	virtual GLContext* CreateGLContext();

	/**
	 * Releases the current thread from the Java Virtual Machine (jvm).
	 * Almost all calls to GLSurface methods attach the current thread to the jvm.
	 * Before ending a thread that used this GLSurface ReleaseThread should be called
	 * to cleanup the jvms resources
	 */
	static void ReleaseThread();

	/**
	 * Attaches the current thread to the Java Virtual Machine (jvm).
	 * Almost all calls to GLSurface methods need the current thread to be attached and
	 * will automatically attach the thread if it is not attached yet.
	 */
	static void AttachThread();

	/**
	 * Checks whether the current thread is attached to the Java Virtual Machine (jvm).
	 * Almost all calls to GLSurface methods need the current thread to be attached and
	 * will automatically attach the thread if it is not attached yet.
	 * It is also possible to manually attach the thread by calling AttachThread().
	 * \return true if the current thread is attached.
	 */
	static bool IsThreadAttached();

	/**
	 * Returns the current java environment. If there is no current environment, the thread is attached to the jvm to get one.
	 * \return the java environment
	 */
	static JNIEnv* GetJavaEnv();

protected:

	GLSurface(jobject obj);
	long GetContextHandle();

private:



	static JavaVM* javaVM;
	static thread_local JNIEnv* javaEnv;

	static std::mutex apiMutex;
	static long nextSurfaceId;
	static std::map<long, GLSurface*>* Surfaces;

	static internal::JTextureCls* TextureCls;
	static internal::JGLSurfaceCls* GLSurfaceCls;

	jobject javaObj;


	std::function<void(void)> sizeChangedCallback;
	std::function<void(events::ScrollEvent)> scrollEventCallback;
	std::function<void(events::MouseEvent)> mouseEventCallback;


};

}
}
}
}

#endif /* INCLUDE_GLSURFACE_H_ */
