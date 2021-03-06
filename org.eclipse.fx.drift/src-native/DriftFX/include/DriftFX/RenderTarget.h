/*
 * Copyright (c) 2019 BestSolution.at and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *     Christoph Caks <ccaks@bestsolution.at> - initial API and implementation
 */
#ifndef DRIFTFX_RENDERTARGET_H_
#define DRIFTFX_RENDERTARGET_H_

namespace driftfx {

class RenderTarget {

public:
	virtual unsigned int GetWidth() = 0;
	virtual unsigned int GetHeight() = 0;
	virtual unsigned int GetGLTexture() = 0;

protected:
	virtual ~RenderTarget();

};

}

#endif /* DRIFTFX_RENDERTARGET_H_ */
