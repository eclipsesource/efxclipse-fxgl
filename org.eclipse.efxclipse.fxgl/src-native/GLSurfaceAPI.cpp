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
#include "jni.h"
//#include <jni.h>
#include <jni_helper.h>
#include "GLSurface.h"


#include <iostream>

using namespace std;
using namespace org::eclipse::efxclipse::fxgl;
using namespace events;

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_internal_GLSurfaceAPI_nInitialize(JNIEnv *env, jclass cls) {
	cerr << "nInitialize()" << endl;
	cout << "nInitialize()" << endl;
	JavaVM* jvm;
	env->GetJavaVM(&jvm);
	internal::InternalGLSurface::Initialize(jvm);
}


extern "C" JNIEXPORT jlong JNICALL Java_org_eclipse_efxclipse_fxgl_internal_GLSurfaceAPI_nCreateGLSurface(JNIEnv *env, jclass cls, jobject surfaceObj)
{
	return internal::InternalGLSurface::RegisterSurface(surfaceObj);
}

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_internal_GLSurfaceAPI_nDisposeGLSurface(JNIEnv *env, jclass cls, jlong surfaceId)
{
	internal::InternalGLSurface::DestroySurface(surfaceId);
}

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_internal_GLSurfaceAPI_nFireSizeChanged(JNIEnv *env, jclass cls, jlong surfaceId)
{
	GLSurface::GetSurface(surfaceId)->FireSizeChanged();
}

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_internal_GLSurfaceAPI_nFireScrollEvent(JNIEnv *env, jclass cls, jlong surfaceId, jobject event)
{
		jclass eventCls = env->GetObjectClass(event);
		jfieldID fieldDeltaX = env->GetFieldID(eventCls, "deltaX", "F");
		jfieldID fieldDeltaY = env->GetFieldID(eventCls, "deltaY", "F");

		ScrollEvent ev;
		ev.deltaX = env->GetFloatField(event, fieldDeltaX);
		ev.deltaY = env->GetFloatField(event, fieldDeltaY);

		GLSurface::GetSurface(surfaceId)->FireScrollEvent(ev);
}
