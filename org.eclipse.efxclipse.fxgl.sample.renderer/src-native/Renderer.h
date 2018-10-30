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
#ifndef RENDERER_H_
#define RENDERER_H_

#include <thread>
#include <map>
#include <chrono>
#include "GLSurface.h"

namespace org {
namespace eclipse {
namespace efxclipse {
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

	virtual void RenderFrame(unsigned int textureId, int width, int height);


	float targetFramerate;
	GLSurface* renderTarget;


	virtual bool InitializeResources();
	virtual void DisposeResources();

private:

	std::atomic<bool> running;
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
}

#endif /* RENDERER_H_ */
