#ifndef INCLUDE_X11GLCONTEXT_H_
#define INCLUDE_X11GLCONTEXT_H_

#include "GLContext.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace at {
namespace bestsolution {
namespace fxgl {
namespace glcommon {
namespace glx {

	typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
	typedef Bool (*glXMakeContextCurrentARBProc)(Display*, GLXDrawable, GLXDrawable, GLXContext);
//	static glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
//	static glXMakeContextCurrentARBProc   glXMakeContextCurrentARB   = NULL;

	class X11GLContext : public GLContext {

	public:
		X11GLContext(int major, int minor, long sharedHandle);

		virtual void MakeCurrent();

		virtual void Dispose();

		virtual long GetHandle();

	private:
		Display* display;
		GLXContext glxContext;
		GLXPbuffer pBuffer;


	};

}
}
}
}
}



#endif /* INCLUDE_X11GLCONTEXT_H_ */
