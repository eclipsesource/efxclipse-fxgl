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
#ifndef OBJRENDERER_H_
#define OBJRENDERER_H_

#include "namespace.h"
#include <string>
#include <mutex>

#include "ScrollEvent.h"
#include "GLContext.h"
#include "Renderer.h"

#include "obj-master/obj.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include "obj-master/etc/GLFundamentals.hpp"

__FXGL_NAMESPACE_BEGIN
namespace sample {
namespace renderer {
namespace objrenderer {

class ObjRenderer : public Renderer {
public:
	ObjRenderer();
	virtual ~ObjRenderer();


	virtual void LoadObj(std::string name);

protected:
	virtual void RenderFrame(unsigned int textureId, int width, int height);
	virtual bool InitializeResources();
	virtual void DisposeResources();

	void onSizeChanged();
	void onScroll(events::ScrollEvent event);


private:
	GLContext* gl;

	GLuint program = 0;
	GLuint textureFramebuffer = 0;
	GLuint depthTex = 0;

	bool newObject = false;
	obj* object = NULL;
	std::mutex mtxObject;


	GLuint ProjectionMatrixLocation;
	GLuint ModelViewMatrixLocation;
	GLuint NormalMatrixLocation;
	GLuint LightPositionLocation;
	GLuint AmbientLightLocation;

	gl::vec3 cam_velocity;
	gl::vec2 cam_rotation;
	gl::vec2 sun_rotation;

	GLfloat scaleFactor = 1.0;

	bool drag_sun_rotation;
	bool drag_cam_rotation;
	gl::vec2 prev_sun_rotation;
	gl::vec2 prev_cam_rotation;
	int  prev_x;
	int  prev_y;
	int  curr_x;
	int  curr_y;

	gl::vec3 cam_position;
	gl::vec2 model_rotation;
	gl::mat4 view();
	gl::mat4 projection(int width= 800, int height=600, GLfloat n=0.10f, GLfloat f=100.f);
	gl::vec4 light();

};

}
}
}
__FXGL_NAMESPACE_END



#endif /* OBJRENDERER_H_ */



