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



