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
#ifndef INCLUDE_IOSURFACEGLCONTEXT_H_
#define INCLUDE_IOSURFACEGLCONTEXT_H_

#include "../GLContext.h"

#include "../cgl/CGLGLContext.h"

#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>

#include <OpenGL/CGLIOSurface.h>

namespace org {
namespace eclipse {
namespace efxclipse {
namespace fxgl {
namespace sample {
namespace glcommon {
namespace iosurface {


	class IOSurfaceGLContext :
			public glcommon::cgl::CGLGLContext {

	public:
		IOSurfaceGLContext(int major, int minor, long sharedHandle);

		virtual void Dispose();

	private:
		cgl::CGLGLContext *fxContext;


	};

}
}
}
}
}
}
}

#endif /* INCLUDE_IOSURFACEGLCONTEXT_H_ */
