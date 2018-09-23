#ifndef OBJRENDERER_H_
#define OBJRENDERER_H_

#include <string>
#include <mutex>
#include "../at.bestsolution.fxgl.native/ScrollEvent.h"
#include "../at.bestsolution.fxgl.glcommon.native/GLContext.h"
#include "../at.bestsolution.fxgl.sample.renderer.native/Renderer.h"

#include "obj-master/obj.h"

#include <GL/glew.h>

#include "obj-master/etc/GLFundamentals.hpp"

namespace at {
namespace bestsolution {
namespace fxgl {
namespace sample {
namespace renderer {
namespace objrenderer {

class ObjRenderer : public Renderer {
public:
	ObjRenderer();
	virtual ~ObjRenderer();


	virtual void LoadObj(std::string name);

protected:
	virtual void RenderFrame(int width, int height);
	virtual bool InitializeResources();
	virtual void DisposeResources();

	void onScroll(events::ScrollEvent event);


private:
	glcommon::GLContext* gl;

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
}
}
}



#endif /* OBJRENDERER_H_ */



