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
#ifndef INCLUDE_GL_CONTEXT_H_
#define INCLUDE_GL_CONTEXT_H_

namespace org {
namespace eclipse {
namespace efxclipse {
namespace fxgl {
namespace sample {
namespace glcommon {

class GLContext {
public:

	virtual void MakeCurrent();

	GLContext();

	virtual ~GLContext();

	virtual void Dispose();

	virtual long GetHandle();

	static GLContext* Create(int major, int minor);
	static GLContext* CreateShared(int major, int minor, long sharedHandle);


};


}
}
}
}
}
}


#endif // INCLUDE_GL_CONTEXT_H_
