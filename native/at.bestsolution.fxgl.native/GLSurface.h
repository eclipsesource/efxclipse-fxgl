#ifndef INCLUDE_GLSURFACE_H_
#define INCLUDE_GLSURFACE_H_

#include <functional>
#include <map>
#include <chrono>

#include "MouseEvent.h"
#include "ScrollEvent.h"

#include "../include/jni.h"

#include "Texture.h"

namespace at {
namespace bestsolution {
namespace fxgl {

class GLSurface {

public:

	static void Initialize();
	static void Dispose();

	static long RegisterSurface(JavaVM* javaVM, jobject obj);
	static void DisposeSurface(long surfaceId);
	static GLSurface* GetSurface(long surfaceId);

	void FireSizeChanged();
	void FireScrollEvent(events::ScrollEvent event);
	void FireMouseEvent(events::MouseEvent event);

	void SubscribeSizeChanged(std::function<void(int)> callback);
	void SubscribeInput(std::function<void> callback);
	void SubscribeRedrawRequest(std::function<void> callback);

	void SubscribeScrollEvent(std::function<void(events::ScrollEvent)> callback);

	void SubscribeMouseEvent(std::function<void(events::MouseEvent)> callback);

	Texture GetNextTexture();
	void SwapTexture();


	long GetContextHandle();

	float GetFPS();

	void ReleaseThread();

private:
	jobject javaObj;
	jclass javaCls;

	JavaVM* javaVM;

	static thread_local JNIEnv* javaEnv;
	JNIEnv* GetJavaEnv();

	jmethodID javaMethodGetContextHandle;

	jmethodID javaMethodGetNextTexture;

	jmethodID javaMethodSwapTexture;

	GLSurface(JavaVM* javaVM, jobject obj);

	std::function<void(int)> sizeChangedCallback;
	std::function<void(events::ScrollEvent)> scrollEventCallback;
	std::function<void(events::MouseEvent)> mouseEventCallback;

	static long nextSurfaceId;
	static std::map<long, GLSurface*>* Surfaces;

	float fps;

	std::chrono::milliseconds fpsLastTime;
	long fpsFrameCount;

};

}
}
}

#endif /* INCLUDE_GLSURFACE_H_ */
