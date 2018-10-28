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
#include "../cgl/CGLGLContext.h"

#include <cstdint>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/CGLDevice.h>

#include <iostream>
using namespace std;

using namespace org::eclipse::efxclipse::fxgl::sample::glcommon;
using namespace cgl;

#include "bridge.h"

CGLGLContext::CGLGLContext(int major, int minor, long sharedHandle) {

	// mac opengl does not allow different renderer / gl versions for shared contexts -.-

	CGLContextObj sharedContext = (CGLContextObj) getCGLContextObj((void*) sharedHandle);
	CGLError errorCode;

	//sharedContext = 0;

	CGLPixelFormatObj pix;
	if (sharedContext != NULL) {
		cout << "using pixelformat from shared context" << endl;
		pix = CGLGetPixelFormat(sharedContext);
		// Version: 2.1 NVIDIA-12.0.19 355.11.10.50.10.103
	}
	else {
		cout << "creating new pixelformat with 3.2" << endl;
		CGLPixelFormatAttribute attributes[4] = {
			kCGLPFAAccelerated,
			kCGLPFAOpenGLProfile,
			(CGLPixelFormatAttribute) kCGLOGLPVersion_3_2_Core,
			(CGLPixelFormatAttribute) 0
		};
		GLint num;
		errorCode = CGLChoosePixelFormat( attributes, &pix, &num );
		cout << "ChoosePixelFormat errorCode = " << errorCode << endl;
		// Version: 4.1 NVIDIA-12.0.19 355.11.10.50.10.103
	}

	errorCode = CGLCreateContext( pix, sharedContext, &context );
	cout << "CreateContext errorCode = " << errorCode << endl;

	cout << " context = " << context << endl;

	errorCode = CGLSetCurrentContext(context);
	cout << "SetCurrentContext errorCode = " << errorCode << endl;

	cout << "Version: " << glGetString(GL_VERSION) << endl;


	if (sharedContext == NULL) {
		errorCode = CGLDestroyPixelFormat( pix );
		cout << "DestroyPixelFormat errorCode = " << errorCode << endl;
	}


}

void CGLGLContext::MakeCurrent() {
	CGLError errorCode;
	errorCode = CGLSetCurrentContext( context );
	cout << "SetContextCurrent errorCode = " << errorCode << endl;
}

long CGLGLContext::GetHandle() {
	return (long) context;
}

void CGLGLContext::Dispose() {
	CGLError errorCode;
	CGLSetCurrentContext( 0 );
	errorCode = CGLDestroyContext( context );
	cout << "DestroyContext errorCode = " << errorCode << endl;
}



GLContext* GLContext::Create(int major, int minor) {
	cgl::CGLGLContext *ctx = new cgl::CGLGLContext(major, minor, 0);
	return (GLContext*) ctx;
}

GLContext* GLContext::CreateShared(int major, int minor, long sharedHandle) {
	cgl::CGLGLContext *ctx = new cgl::CGLGLContext(major, minor, sharedHandle);

	return (GLContext*) ctx;
}

