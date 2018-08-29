
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <GL/glew.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <GL/glx.h>

#include <GL/glu.h>

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

#include <glm/glm.hpp>

#include "Utils.h"

#include "jni.h"

void Err();

GLuint CreateFullscreenQuadVertexArray(float border) {
	GLuint ID;
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f + border, -1.0f + border, 0.0f,
		1.0f - border, -1.0f + border, 0.0f,
		-1.0f + border,  1.0f - border, 0.0f,
		-1.0f + border,  1.0f - border, 0.0f,
		1.0f - border, -1.0f + border, 0.0f,
		1.0f - border,  1.0f - border, 0.0f,
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

	return ID;
}

struct Color {
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};
struct Vertex {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	Color color;
};

GLuint CreateTriangleVertexArray() {
	GLuint vaID;
	glGenVertexArrays(1, &vaID);

	static const Vertex data2[] = {
		{  0.0f,   0.5f, 0.0f, {0.0f, 0.0f, 1.0f} },
		{  0.45f, -0.5f, 0.0f, {0.0f, 1.0f, 0.0f} },
		{ -0.45f, -0.5f, 0.0f, {1.0f, 0.0f, 0.0f} }
	};

	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data2), data2, GL_STATIC_DRAW);
	Err();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// build va
	glBindVertexArray(vaID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (3 * sizeof(GLfloat)) );

	glBindVertexArray(0);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	return vaID;
}


// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)
{
  const char *start;
  const char *where, *terminator;

  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}

static bool ctxErrorOccurred = false;
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}

GLXFBConfig FindFBConfig(Display *display) {
	int glx_major, glx_minor;
	// FBConfigs were added in GLX version 1.3.
	if ( !glXQueryVersion( display, &glx_major, &glx_minor ) ||
	   ( ( glx_major == 1 ) && ( glx_minor < 3 ) ) || ( glx_major < 1 ) )
	{
		printf("Invalid GLX version");
		exit(1);
	}
	static int visual_attribs[] =
	{
		GLX_X_RENDERABLE    , True,
		GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
		GLX_RENDER_TYPE     , GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
		GLX_RED_SIZE        , 8,
		GLX_GREEN_SIZE      , 8,
		GLX_BLUE_SIZE       , 8,
		GLX_ALPHA_SIZE      , 8,
		GLX_DEPTH_SIZE      , 24,
		GLX_STENCIL_SIZE    , 8,
		GLX_DOUBLEBUFFER    , True,
		//GLX_SAMPLE_BUFFERS  , 1,
		//GLX_SAMPLES         , 4,
		None
	};
	int fbcount;
	GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
	if (!fbc) {
		printf("Failed to retrieve a framebuffer config\n");
		exit(1);
	}
	printf("Found %d matching FB configs.\n", fbcount);
// Pick the FB config/visual with the most samples per pixel
	printf("Getting XVisualInfos\n");
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	int i;
	for (i = 0; i < fbcount; ++i) {
		XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
		if (vi) {
			int samp_buf, samples;
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS,
					&samp_buf);
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

			printf(
					"  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
							" SAMPLES = %d\n", i, vi->visualid, samp_buf,
					samples);

			if (best_fbc < 0 || samp_buf && samples > best_num_samp)
				best_fbc = i, best_num_samp = samples;
			if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
				worst_fbc = i, worst_num_samp = samples;
		}
		XFree(vi);
	}

	GLXFBConfig bestFbc = fbc[best_fbc];
// Be sure to free the FBConfig list allocated by glXChooseFBConfig()
	XFree(fbc);
	return bestFbc;
}

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;

void InitGLXExtension() {
	if (!glXCreateContextAttribsARB) {
		// NOTE: It is not necessary to create or make current to a context before
		// calling glXGetProcAddressARB
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
			             glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
	}
}

void Err() {
	// check OpenGL error
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error occured: " << err << ": " << gluErrorString(err) << std::endl;
	}

}

GLXContext CreateContext(Display *display, GLXFBConfig fbc, GLXContext shared_ctx) {
    InitGLXExtension();
	printf( "Creating context\n" );
	int context_attribs[] =
	  {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_DEBUG_BIT_ARB | GLX_CONTEXT_CORE_PROFILE_BIT_ARB,

		None
	  };

	GLXContext ctx = glXCreateContextAttribsARB(
			display,
			fbc,
			shared_ctx, //share_context
			True, // direct
			context_attribs );

	// Sync to ensure any errors generated are processed.
	XSync( display, False );
	if ( !ctxErrorOccurred && shared_ctx )
	  printf( "Created context\n" );

	return ctx;
}

void InitGLEW() {
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(-1);
	}
}

Window CreateWindow(Display *display, GLXFBConfig fbc, int width, int height) {
	// Get a visual
	XVisualInfo *vi = glXGetVisualFromFBConfig( display, fbc );
	printf( "Chosen visual ID = 0x%x\n", vi->visualid );

	printf( "Creating colormap\n" );
	XSetWindowAttributes swa;
	Colormap cmap;
	swa.colormap = cmap = XCreateColormap( display,
										  RootWindow( display, vi->screen ),
										  vi->visual, AllocNone );
	swa.background_pixmap = None ;
	swa.border_pixel      = 0;
	swa.event_mask        = StructureNotifyMask;

	printf( "Creating window\n" );
	Window win = XCreateWindow( display, RootWindow( display, vi->screen ),
							   0, 0, width, height, 0, vi->depth, InputOutput,
							   vi->visual,
							   CWBorderPixel|CWColormap|CWEventMask, &swa );
	if ( !win )
	{
	 printf( "Failed to create window.\n" );
	 exit(1);
	}

	// Done with the visual info data
	XFree( vi );

	XFreeColormap( display, cmap );

	return win;
}

int xerrorhandler(Display *dsp, XErrorEvent *error)
{
  char errorstring[128];
  XGetErrorText(dsp, error->error_code, errorstring, 128);

  std::cerr << "ack!fatal: X error--" << errorstring << std::endl;
  exit(-1);
}

#define jlong_to_ptr(value) ((void*)((long)value))
#define ptr_to_jlong(value) (jlong)((long)(value))

#define WIDTH 800
#define HEIGHT 600

GLuint triangleVA = 0;
GLuint GetTriangleVA() {
	if (!triangleVA) {
		triangleVA = CreateTriangleVertexArray();
		std::cout << " # VA: created triangle va " << triangleVA << std::endl;
	}
	return triangleVA;
}

GLuint sharedTexture = 0;
GLuint GetSharedTexture(GLuint sharedTextureId) {
	if (!sharedTexture) {
		sharedTexture = sharedTextureId;
		if (sharedTexture == 0) {
			glGenTextures(1, &sharedTexture);
			Err();
			// "Bind" the newly created texture : all future texture functions will modify this texture
			glBindTexture(GL_TEXTURE_2D, sharedTexture);
			Err();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			Err();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			Err();

			// Give an empty image to OpenGL ( the last "0" means "empty" )
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			Err();

			glBindTexture(GL_TEXTURE_2D, 0);
			Err();
			std::cout << " # Texture: created new texture " << sharedTexture << std::endl;
		}
		else {
			std::cout << " # Texture: using shared texture " << sharedTexture << std::endl;
		}
	}
	return sharedTexture;
}

GLuint textureFramebuffer = 0;
GLuint GetTextureFramebuffer(GLuint sharedTextureId) {
	if (!textureFramebuffer) {
		glGenFramebuffers(1, &textureFramebuffer);
		Err();
		glBindFramebuffer(GL_FRAMEBUFFER, textureFramebuffer);
		Err();

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GetSharedTexture(sharedTextureId), 0);
		Err();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){  //Check for FBO completeness
			std::cout << "Error! FrameBuffer is not complete" << std::endl;
			exit(-1);
		 }
		Err();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);    //unbind framebuffer
		Err();

		std::cout << " # Texture Framebuffer: " << textureFramebuffer << std::endl;
	}
	return textureFramebuffer;
}


GLuint triangleProgram = 0;
GLuint GetTriangleProgram() {
	if (!triangleProgram) {
		triangleProgram = LoadShaders( "Triangle.vertexshader", "Triangle.fragmentshader" );
		std::cout << " # created program " << triangleProgram << std::endl;
	}
	return triangleProgram;
}


void renderTriangle() {


	glBindVertexArray(GetTriangleVA());
	Err();

	glDrawArrays(GL_TRIANGLES, 0, 3);
	Err();

	glBindVertexArray(0);
	Err();
}


int start(long contextHandle, int textureId) {
	printf("starting up GL33Renderer\n");

	std::cout << std::flush;

	XSetErrorHandler(xerrorhandler);
	Display *display = XOpenDisplay(NULL);
	if (!display) {
		fprintf( stderr, "Failed to open X display");
	}





	// Get a matching FB config

	printf( "Getting matching framebuffer configs\n" );

	GLXFBConfig fbc = FindFBConfig(display);


	Window win = CreateWindow(display, fbc, WIDTH, HEIGHT);

	XStoreName( display, win, "GL 3.3 Core Profile Window" );

	printf( "Mapping window\n" );
	XMapWindow( display, win );




	GLXContext s = (GLXContext) jlong_to_ptr(contextHandle);


	std::cout << "..trying to create context" << std::endl << std::flush;

	GLXContext ctx = CreateContext(display, fbc, s);

	glXMakeCurrent( display, win, ctx );
	std::cout << " created shared context => " << glGetString(GL_VERSION) << std::endl;
	InitGLEW();

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);

	GetTriangleVA();
	GLuint prog = GetTriangleProgram();
	GLuint rotID = glGetUniformLocation(prog, "rot");

	float angle = 0;

	do{
		angle += 0.1;


//		 XWindowAttributes wndAttr;
//		 XGetWindowAttributes(display, win, &wndAttr);
//
//		 width = wndAttr.width;
//		 height = wndAttr.height;



		 // render to texture

		 glBindFramebuffer(GL_FRAMEBUFFER, GetTextureFramebuffer(textureId));
		 Err();
		 //glViewport(0, 0, WIDTH, HEIGHT);
		 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 glUseProgram(GetTriangleProgram());
		 glUniform1f(rotID, angle);

		 renderTriangle();


		 // render to window

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glViewport(0, 0, WIDTH, HEIGHT);
		 glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(GetTriangleProgram());
		glUniform1f(rotID, angle);

		renderTriangle();


		// Swap buffers
		glXSwapBuffers ( display, win );

		//usleep(20000);
	}
	while( true );

	glXMakeCurrent( display, 0, 0 );
	glXDestroyContext( display, ctx );
	XDestroyWindow( display, win );
	XCloseDisplay( display );

}

extern "C" JNIEXPORT void JNICALL Java_at_bestsolution_fxgl_es2_GL33Renderer_nRun
(JNIEnv *env, jclass, jlong contextHandle, jint textureId)
{
	start((long) contextHandle, (int) textureId);
}

