#ifndef RENDERER_H_
#define RENDERER_H_

#include <thread>
#include <map>
#include <chrono>
#include "../at.bestsolution.fxgl.native/GLSurface.h"

namespace at {
namespace bestsolution {
namespace fxgl {
namespace sample {
namespace renderer {

class Renderer {
public:

	Renderer();
	virtual ~Renderer();

	virtual void Start();
	virtual void Stop();
	virtual void SetTargetFramerate(float framerate);
	virtual void SetRenderTarget(GLSurface* surface);

	virtual double GetAvgFps();

	static void Initialize();
	static void Dispose();
	static long RegisterRenderer(Renderer* renderer);
	static void DisposeRenderer(long rendererId);
	static Renderer* GetRenderer(long rendererId);

protected:

	virtual void RenderFrame(int width, int height);


	float targetFramerate;
	GLSurface* renderTarget;


	virtual bool InitializeResources();
	virtual void DisposeResources();

private:

	bool running;
	std::thread* thread;
	void Run();

	std::chrono::microseconds lastFrameTime;
	std::chrono::microseconds frameTimes[100];
	int frameTimeIndex = 0;
	bool arrayFilled = false;

	double avgFps = 0;


	static long nextRendererId;
	static std::map<long, Renderer*>* renderers;



};


}
}
}
}
}



#endif /* RENDERER_H_ */
