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
#include "../org.eclipse.efxclipse.fxgl/src-native/ScrollEvent.h"
#include "../org.eclipse.efxclipse.fxgl/src-native/GLContext.h"
#include "../org.eclipse.efxclipse.fxgl.sample.renderer/src-native/Renderer.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

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
	GLContext* gl;

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



