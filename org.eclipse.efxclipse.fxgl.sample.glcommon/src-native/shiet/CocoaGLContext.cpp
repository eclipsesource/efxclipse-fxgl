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
#include "CocoaGLContext.h"
#include "shiet.h"

#include <cstdint>


#include <iostream>
using namespace std;

using namespace org::eclipse::efxclipse::fxgl::sample::glcommon;
using namespace cocoa;

CocoaGLContext::CocoaGLContext(int major, int minor, long sharedHandle) {

	cout << "sharedHandle: " << sharedHandle <<endl;

	void *format = getPixelFormat((void*) sharedHandle);

	cout << "format:" << format << endl;

	context = createContext((void*) sharedHandle, format, 0, 0);

	cout << "context:" << context << endl;

	//deletePixelFormat(format);
}

void CocoaGLContext::MakeCurrent() {

	makeCurrentContext(context);

}

long CocoaGLContext::GetHandle() {
	return (long) context;
}

void CocoaGLContext::Dispose() {

	deleteContext(context);

}



GLContext* GLContext::Create(int major, int minor) {
	cocoa::CocoaGLContext *ctx = new cocoa::CocoaGLContext(major, minor, 0);
	return (GLContext*) ctx;
}

GLContext* GLContext::CreateShared(int major, int minor, long sharedHandle) {
	cocoa::CocoaGLContext *ctx = new cocoa::CocoaGLContext(major, minor, sharedHandle);

	return (GLContext*) ctx;
}
