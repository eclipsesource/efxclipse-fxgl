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
#include "Renderer.h"
#include "../org.eclipse.efxclipse.fxgl.native.x11/GLSurface.h"

#include "../include/jni.h"

#include <thread>
#include <functional>
#include <map>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace org::eclipse::efxclipse::fxgl;
using namespace sample::renderer;


long Renderer::nextRendererId = 1;
map<long, Renderer*>* Renderer::renderers = NULL;

void Renderer::Initialize()  {
	if (renderers == NULL) {
		cout << "Initialize Renderer" << endl;
		renderers = new map<long, Renderer*>();
	}
}
void Renderer::Dispose() {
	cout << "Dispose Renderer" << endl;
	delete renderers;
}

long Renderer::RegisterRenderer(Renderer* renderer) {
	long rendererId = nextRendererId++;
	(*renderers)[rendererId] = renderer;
	return rendererId;
}
void Renderer::DisposeRenderer(long rendererId) {
	Renderer* renderer = (*renderers)[rendererId];
	delete renderer;
	renderers->erase(rendererId);
}
Renderer* Renderer::GetRenderer(long rendererId) {
	return (*renderers)[rendererId];
}

Renderer::Renderer() :
		targetFramerate(60),
		renderTarget(NULL),
		running(false),
		thread(NULL)
{

}

Renderer::~Renderer() {

}

void Renderer::SetRenderTarget(GLSurface* surface) {
	renderTarget = surface;
}

void Renderer::SetTargetFramerate(float framerate) {
	targetFramerate = framerate;
}

void Renderer::Start() {
	if (!running) {
		running = true;
		thread = new std::thread(std::bind(&Renderer::Run, this), "MyRendererThread");
	}
}

void Renderer::Stop() {
	if (running) {
		cout << "Stopping native render thread" << endl;
		running = false;
		cout << " waiting for thread to terminate.." << endl;
		thread->join();
		cout << " thread ended" << endl;
		thread = NULL;
	}
}

void Renderer::RenderFrame(int width, int height) {}

double Renderer::GetAvgFps() {
	return avgFps;
}

void Renderer::Run() {

	// init gl context

	if (InitializeResources()) {


		cout << "Starting render loop" << endl;
		while (running) {

			microseconds frameBegin = duration_cast<microseconds>( system_clock::now().time_since_epoch() );

			Texture target = renderTarget->GetNextTexture();

			RenderFrame(target.width, target.height);

			renderTarget->SwapTexture();

			// fps computation
			microseconds now = duration_cast<microseconds>( system_clock::now().time_since_epoch() );

			microseconds oldFrameTime = frameTimes[frameTimeIndex];
			frameTimes[frameTimeIndex] = now;
			frameTimeIndex = (frameTimeIndex + 1) % 100;
			if (frameTimeIndex == 0) {
				arrayFilled = true;
			}
			if (arrayFilled) {
				microseconds elapsedTime = now - oldFrameTime;
				microseconds elapsedTimePerFrame = elapsedTime / 100;
				avgFps = 1000000.0 / elapsedTimePerFrame.count();
			}

			microseconds lastFrameDuration = now - frameBegin;

			microseconds targetFrameTime(static_cast<long>(1000000.0 / targetFramerate));
			microseconds sleep = targetFrameTime - lastFrameDuration;
			if (sleep > targetFrameTime) {
				sleep = targetFrameTime / 2;
			}

			this_thread::sleep_for(sleep);

		}
		cout << "Render loop terminated" << endl;

		DisposeResources();

		renderTarget->ReleaseThread();
	}

}

bool Renderer::InitializeResources() { return false; }
void Renderer::DisposeResources() {}


extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nInitializeLibrary
(JNIEnv *env, jclass)
{
	Renderer::Initialize();
}
extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nDisposeLibrary
(JNIEnv *env, jclass)
{
	Renderer::Initialize();
}


extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nStartRenderer
(JNIEnv *env, jclass, jlong rendererId)
{
	Renderer* renderer = Renderer::GetRenderer(rendererId);
	renderer->Start();
}
extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nStopRenderer
(JNIEnv *env, jclass, jlong rendererId)
{
	Renderer* renderer = Renderer::GetRenderer(rendererId);
	renderer->Stop();
}
extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nSetTargetFrameRate
(JNIEnv *env, jclass, jlong rendererId, jfloat framerate)
{
	Renderer* renderer = Renderer::GetRenderer(rendererId);
	renderer->SetTargetFramerate(framerate);
}
extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nSetRenderTarget
(JNIEnv *env, jclass, jlong rendererId, long surfaceId)
{
	Renderer* renderer = Renderer::GetRenderer(rendererId);
	GLSurface* surface = GLSurface::GetSurface(surfaceId);
	renderer->SetRenderTarget(surface);
}
extern "C" JNIEXPORT jdouble JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nGetAvgFps
(JNIEnv *env, jclass cls, jlong rendererId)
{
	Renderer* renderer = Renderer::GetRenderer(rendererId);
	return renderer->GetAvgFps();
}
extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_internal_RendererAPI_nSetTargetFramerate
(JNIEnv *env, jclass cls, jlong rendererId, jfloat framerate)
{
	Renderer* renderer = Renderer::GetRenderer(rendererId);
	renderer->SetTargetFramerate(framerate);
}
