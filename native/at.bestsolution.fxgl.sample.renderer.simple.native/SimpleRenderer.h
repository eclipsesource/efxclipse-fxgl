#ifndef SIMPLERENDERER_H_
#define SIMPLERENDERER_H_

#include <string>
#include <mutex>
#include "../at.bestsolution.fxgl.native/ScrollEvent.h"
#include "../at.bestsolution.fxgl.glcommon.native/GLContext.h"
#include "../at.bestsolution.fxgl.sample.renderer.native/Renderer.h"

#include <GL/glew.h>

namespace at {
namespace bestsolution {
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



#endif /* SIMPLERENDERER_H_ */



