#ifndef INCLUDE_GLSURFACE_H_
#define INCLUDE_GLSURFACE_H_

#include <functional>
#include <map>
#include <chrono>
#include <mutex>

#include "MouseEvent.h"
#include "ScrollEvent.h"

#include "../include/jni.h"

#include "Texture.h"

namespace at {
namespace bestsolution {
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
	Texture GetNextTexture();

	/**
	 * Swaps the textures
	 */
	void SwapTexture();

	/**
	 * Returns the OpenGL context handle.
	 * Use it to create your shared context(s).
	 */
	long GetContextHandle();


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

private:



	static JavaVM* javaVM;
	static thread_local JNIEnv* javaEnv;

	static std::mutex apiMutex;
	static long nextSurfaceId;
	static std::map<long, GLSurface*>* Surfaces;

	static internal::JTextureCls* TextureCls;
	static internal::JGLSurfaceCls* GLSurfaceCls;

	jobject javaObj;

	GLSurface(jobject obj);

	std::function<void(void)> sizeChangedCallback;
	std::function<void(events::ScrollEvent)> scrollEventCallback;
	std::function<void(events::MouseEvent)> mouseEventCallback;

};

}
}
}

#endif /* INCLUDE_GLSURFACE_H_ */
