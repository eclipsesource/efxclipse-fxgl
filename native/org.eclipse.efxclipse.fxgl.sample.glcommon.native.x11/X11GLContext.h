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
#ifndef INCLUDE_X11GLCONTEXT_H_
#define INCLUDE_X11GLCONTEXT_H_

#include "GLContext.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace org {
namespace eclipse {
namespace efxclipse {
namespace fxgl {
namespace sample {
namespace glcommon {
namespace glx {

	typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
	typedef Bool (*glXMakeContextCurrentARBProc)(Display*, GLXDrawable, GLXDrawable, GLXContext);
//	static glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
//	static glXMakeContextCurrentARBProc   glXMakeContextCurrentARB   = NULL;

	class X11GLContext : public GLContext {

	public:
		X11GLContext(int major, int minor, long sharedHandle);

		virtual void MakeCurrent();

		virtual void Dispose();

		virtual long GetHandle();

	private:
		Display* display;
		GLXContext glxContext;
		GLXPbuffer pBuffer;


	};

}
}
}
}
}
}
}



#endif /* INCLUDE_X11GLCONTEXT_H_ */
