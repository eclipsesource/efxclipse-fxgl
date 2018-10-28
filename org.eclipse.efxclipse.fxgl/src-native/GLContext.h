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

#include "namespace.h"

__FXGL_NAMESPACE_BEGIN

class GLContext {
public:

	GLContext();

	virtual ~GLContext();

	virtual void SetCurrent();

	virtual void UnsetCurrent();

	virtual void* GetHandle();

	virtual void CheckGLError(const char* msg);
};

__FXGL_NAMESPACE_END

#endif // INCLUDE_GL_CONTEXT_H_
