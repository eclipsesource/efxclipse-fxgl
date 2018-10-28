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
#include "IOSurfaceGLContext.h"

#include "../cgl/CGLGLContext.h"

#include <cstdint>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/CGLIOSurface.h>
#include <OpenGL/gl3.h>
#include <OpenGL/CGLDevice.h>

#include <iostream>
using namespace std;

using namespace org::eclipse::efxclipse::fxgl::sample::glcommon;
using namespace iosurface;
using namespace cgl;



IOSurfaceGLContext::IOSurfaceGLContext(int major, int minor, long sharedHandle)
	: CGLGLContext(major, minor, 0),
	  fxContext(new CGLGLContext(major, minor, sharedHandle)) {


}

void IOSurfaceGLContext::Dispose() {
	fxContext->Dispose();
	CGLGLContext::Dispose();
}

GLContext* GLContext::Create(int major, int minor) {
	iosurface::IOSurfaceGLContext *ctx = new iosurface::IOSurfaceGLContext(major, minor, 0);
	return (GLContext*) ctx;
}

GLContext* GLContext::CreateShared(int major, int minor, long sharedHandle) {
	iosurface::IOSurfaceGLContext *ctx = new iosurface::IOSurfaceGLContext(major, minor, sharedHandle);
	return (GLContext*) ctx;
}
