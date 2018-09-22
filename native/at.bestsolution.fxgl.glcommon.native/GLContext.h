#ifndef INCLUDE_GL_CONTEXT_H_
#define INCLUDE_GL_CONTEXT_H_

namespace at {
namespace bestsolution {
namespace fxgl {
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


#endif // INCLUDE_GL_CONTEXT_H_
