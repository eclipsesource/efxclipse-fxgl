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
#ifndef INCLUDE_CGLGLCONTEXT_H_
#define INCLUDE_CGLGLCONTEXT_H_

#include "../GLContext.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>

namespace org {
namespace eclipse {
namespace efxclipse {
namespace fxgl {
namespace sample {
namespace glcommon {
namespace cocoa {


	class CocoaGLContext : public GLContext {

	public:
		CocoaGLContext(int major, int minor, long sharedHandle);

		virtual void MakeCurrent();

		virtual void Dispose();

		virtual long GetHandle();

	private:
		void *context;
	};

}
}
}
}
}
}
}

#endif /* INCLUDE_CGLGLCONTEXT_H_ */
