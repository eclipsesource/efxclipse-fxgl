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
#include "GLSurface.h"
#include "jni.h"
using namespace org::eclipse::efxclipse::fxgl;
using namespace sample::renderer;

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
