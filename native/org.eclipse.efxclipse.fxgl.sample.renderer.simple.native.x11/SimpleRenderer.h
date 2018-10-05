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
#ifndef SIMPLERENDERER_H_
#define SIMPLERENDERER_H_

#include <string>
#include <mutex>
#include "../org.eclipse.efxclipse.fxgl.native.x11/ScrollEvent.h"
#include "../org.eclipse.efxclipse.fxgl.sample.glcommon.native.x11/GLContext.h"
#include "../org.eclipse.efxclipse.fxgl.sample.renderer.native.x11/Renderer.h"

#include <GL/glew.h>

namespace org {
namespace eclipse {
namespace efxclipse {
namespace fxgl {
namespace sample {
namespace renderer {
namespace simple {

class SimpleRenderer : public Renderer {
public:
	SimpleRenderer();
	virtual ~SimpleRenderer();


protected:
	virtual void RenderFrame(int width, int height);
	virtual bool InitializeResources();
	virtual void DisposeResources();

	void onScroll(events::ScrollEvent event);


private:
	glcommon::GLContext* gl;

	GLuint textureFramebuffer;
	GLuint program;
	GLuint rotID;
	GLfloat angle;

	GLuint vaID;

};

}
}
}
}
}
}
}



#endif /* SIMPLERENDERER_H_ */



