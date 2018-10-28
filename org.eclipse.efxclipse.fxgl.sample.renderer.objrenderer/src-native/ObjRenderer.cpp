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
#include "ObjRenderer.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

#include "Texture.h"
#include "GLContext.h"

#include <iostream>

#include "jni.h"

#include "obj-master/etc/GLFundamentals.hpp"
using namespace gl;

__FXGL_NAMESPACE_USING
using namespace sample::renderer;
using namespace objrenderer;

using namespace std;


ObjRenderer::ObjRenderer() :
	sun_rotation(0, 180),
	model_rotation(30, 0){


}

ObjRenderer::~ObjRenderer() {

}

void checkFramebuffer() {

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
	case GL_FRAMEBUFFER_COMPLETE: break;
	case GL_FRAMEBUFFER_UNDEFINED: cerr << "GL_FRAMEBUFFER_UNDEFINED" << endl; break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << endl; break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << endl; break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << endl; break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << endl; break;
	case GL_FRAMEBUFFER_UNSUPPORTED: cerr << "GL_FRAMEBUFFER_UNSUPPORTED" << endl; break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << endl; break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << endl; break;
	default: cerr << "Unknown framebuffer issue " << status << endl; break;
	}

}

void ObjRenderer::RenderFrame(unsigned int textureId, int width, int height) {
	gl->CheckGLError("RenderFrame#BEFORE");
	// TODO animation stuff
//	model_rotation[0] += 0.5;
	model_rotation[1] -= 1.5;

	sun_rotation[0] += 1.5;
	sun_rotation[1] += 1.5;

	// TODO put renderTarget setup somewhere else

	gl->SetCurrent();
	//cerr << "Setting up render target" << endl;
	gl->CheckGLError("RenderFrame#GetNextTexture()");

	glBindFramebuffer(GL_FRAMEBUFFER, textureFramebuffer);
	gl->CheckGLError("RenderFrame#glBindFramebuffer 1");

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureId, 0);
	gl->CheckGLError("RenderFrame#glFramebufferTexture color");

	// we need to recreate the depth tex in case the size was changed
	glBindTexture(GL_TEXTURE_2D, depthTex);
	gl->CheckGLError("RenderFrame#glBindTexture");
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	gl->CheckGLError("RenderFrame#glTexImage2D");

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTex, 0);
	gl->CheckGLError("RenderFrame#glFramebufferTexture depth");

	checkFramebuffer();

	//cerr << "RenderTarget setup done. " << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, textureFramebuffer);
	gl->CheckGLError("RenderFrame#glBindFramebuffer 2");

	glViewport(0, 0, width, height);
	gl->CheckGLError("RenderFrame#glViewport");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl->CheckGLError("RenderFrame#glClear");

	glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_TRUE, projection(width, height, 0.1f, 100.f));
	glUniformMatrix4fv(ModelViewMatrixLocation,  1, GL_TRUE, view());
	glUniformMatrix3fv(NormalMatrixLocation,     1, GL_TRUE, normal(view()));
	glUniform4fv      (LightPositionLocation,    1,          light());

	mtxObject.lock();
	if (object != NULL) {
		if (newObject) {
			cerr << "resetting.." << endl;
			obj_set_vert_loc(object, glGetAttribLocation(program, "vTangent"),
											 glGetAttribLocation(program, "vNormal"),
											 glGetAttribLocation(program, "vTexCoord"),
											 glGetAttribLocation(program, "vPosition"));

			obj_set_prop_loc(object, OBJ_KN, -1, glGetUniformLocation(program, "NormalTexture"), -1);
			obj_set_prop_loc(object, OBJ_KD, -1, glGetUniformLocation(program, "DiffuseTexture"), -1);
			obj_set_prop_loc(object, OBJ_KS, -1, glGetUniformLocation(program, "SpecularTexture"), -1);

			newObject = false;
		}
		obj_render(object);
	}
	mtxObject.unlock();


	glFlush();
	gl->CheckGLError("RenderFrame#glFlush");




}

void ObjRenderer::onSizeChanged() {
	//cerr << "onSizeChanged" << endl;
}

void ObjRenderer::onScroll(events::ScrollEvent event) {
	//cerr << "onScroll " << event.deltaY << endl;
	scaleFactor += event.deltaY / 1000;
}

void ObjRenderer::LoadObj(string name) {
	mtxObject.lock();
	if (object != NULL) {
		obj_delete(object);
		object = NULL;
	}

	object =  obj_create(name.c_str());
	newObject = true;
	mtxObject.unlock();
}


bool ObjRenderer::InitializeResources() {

	if (renderTarget == NULL) {
		cout << "no renderTarget set; not starting" << endl;
		return false;
	}

	using namespace std::placeholders; // for `_1`
	renderTarget->SubscribeScrollEvent(std::bind(&ObjRenderer::onScroll, this, _1));
	renderTarget->SubscribeSizeChanged(std::bind(&ObjRenderer::onSizeChanged, this));

	gl = renderTarget->CreateGLContext();


	cerr << "ObjRenderer::InitializeResources()_x" << endl;

	cerr << " setting ctx current " << endl;
	gl->SetCurrent();

	cerr << " loading program " << endl;
	program = gl::init_program("vertex.glsl", "fragment.glsl");


	cerr << " loaded program @" << program << endl;

	cerr << " ... " << endl;

	glUseProgram(program);

	ProjectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
	ModelViewMatrixLocation  = glGetUniformLocation(program, "ModelViewMatrix");
	NormalMatrixLocation     = glGetUniformLocation(program, "NormalMatrix");
	LightPositionLocation    = glGetUniformLocation(program, "LightPosition");
	AmbientLightLocation = glGetUniformLocation(program, "AmbientLight");

	glUniform4f(AmbientLightLocation, 0.2, 0.2, 0.2, 1.0);


	LoadObj("Chest.obj");


	 glClearColor(0.0, 0.0, 0.0, 0.0);
	 glEnable(GL_DEPTH_TEST);
	 cam_position = gl::vec3(0, 0, 3);


	 glGenFramebuffers(1, &textureFramebuffer);
	 cerr << " created framebuffer " << textureFramebuffer << endl;
	 glBindFramebuffer(GL_FRAMEBUFFER, textureFramebuffer);

	 glBindFramebuffer(GL_FRAMEBUFFER, 0);

	 glGenTextures(1, &depthTex);
	 cerr << " created depthTex " << depthTex << endl;

	 cerr << " finished initialization." << endl;

	 //glEnable(GL_DEBUG_OUTPUT);

	 return true;
}
void ObjRenderer::DisposeResources() {

	obj_delete(object);
	object = NULL;

	// delete gl resources
	cerr << " deleting framebuffer @" << textureFramebuffer << endl;
	glDeleteFramebuffers(1, &textureFramebuffer);
	cerr << " deleting program @" << program << endl;
	glDeleteProgram(program);
	cerr << " deleting texture depthTex @" << depthTex  << endl;
	glDeleteTextures(1, &depthTex);

	// destroy gl context
	delete gl;
}

mat4 ObjRenderer::view()
{
	return xrotation(to_radians(cam_rotation[0]))
		 * yrotation(to_radians(cam_rotation[1]))
		  * translation(-cam_position)
		  // TODO the javafx texture seems flipped; for now i invert here y
		   *scale(vec3(scaleFactor, -scaleFactor, scaleFactor))
			* xrotation(to_radians(model_rotation[0]))
				 * yrotation(to_radians(model_rotation[1]));
}
mat4 ObjRenderer::projection(int width, int height, GLfloat n, GLfloat f)
{
	const GLfloat a = GLfloat(width) / GLfloat(height);
	return perspective(to_radians(60.f), a, n, f);
}

vec4 ObjRenderer::light()
{
	return xrotation(to_radians(sun_rotation[0]))
		 * yrotation(to_radians(sun_rotation[1])) * vec4(0, 0, 1, 0);
}


extern "C" JNIEXPORT jlong JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_objrenderer_ObjRenderer_nCreate
(JNIEnv *env, jclass cls)
{
	Renderer::Initialize();
	ObjRenderer* renderer = new ObjRenderer();
	return (jlong) Renderer::RegisterRenderer((Renderer*) renderer);
}

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_objrenderer_ObjRenderer_nDestroy
(JNIEnv *env, jclass cls, jlong rendererId)
{
	Renderer::DisposeRenderer(rendererId);
}

std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_objrenderer_ObjRenderer_nLoadObj
(JNIEnv *env, jclass cls, jlong rendererId, jstring name)
{
	ObjRenderer* renderer = (ObjRenderer*) Renderer::GetRenderer(rendererId);
	renderer->LoadObj(jstring2string(env, name));
}
