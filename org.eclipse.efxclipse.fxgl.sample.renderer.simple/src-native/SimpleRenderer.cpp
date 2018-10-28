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

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include "../org.eclipse.efxclipse.fxgl/src-native/Texture.h"
#include "../org.eclipse.efxclipse.fxgl/src-native/GLContext.h"

#include <iostream>

#include <jni.h>

#include "SimpleRenderer.h"

#include "Utils.h"

using namespace org::eclipse::efxclipse::fxgl;
using namespace sample::renderer;
using namespace simple;

using namespace std;


SimpleRenderer::SimpleRenderer() {


}

SimpleRenderer::~SimpleRenderer() {

}

void SimpleRenderer::RenderFrame(int width, int height) {

	angle += 0.1;

	Texture target = renderTarget->GetNextTexture();
	glBindFramebuffer(GL_FRAMEBUFFER, textureFramebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target.textureId, 0);

	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glUniform1f(rotID, angle);

	// render
	glBindVertexArray(vaID);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);


	glFlush();
}

void SimpleRenderer::onScroll(events::ScrollEvent event) {
	cerr << "onScroll " << event.deltaY << endl;
	//scaleFactor += event.deltaY / 1000;
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



bool SimpleRenderer::InitializeResources() {

	if (renderTarget == NULL) {
		cout << "no renderTarget set; not starting" << endl;
		return false;
	}

	using namespace std::placeholders; // for `_1`
	renderTarget->SubscribeScrollEvent(std::bind(&SimpleRenderer::onScroll, this, _1));


	gl = renderTarget->CreateGLContext();


	cerr << "ObjRenderer::InitializeResources()" << endl;

	cerr << " setting ctx current " << endl;
	gl->SetCurrent();


	glGenFramebuffers(1, &textureFramebuffer);

	program = LoadShaders( "Triangle.vertexshader", "Triangle.fragmentshader" );
	rotID = glGetUniformLocation(program, "rot");

	// vertex array
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


	 return true;
}
void SimpleRenderer::DisposeResources() {

	glUseProgram(0);

	glDeleteVertexArrays(1, &vaID);

	glDeleteProgram(program);
	glDeleteFramebuffers(1, &textureFramebuffer);

	// destroy gl context
	delete gl;
}



extern "C" JNIEXPORT jlong JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_simple_SimpleRenderer_nCreate
(JNIEnv *env, jclass cls)
{
	Renderer::Initialize();
	SimpleRenderer* renderer = new SimpleRenderer();
	return (jlong) Renderer::RegisterRenderer((Renderer*) renderer);
}

extern "C" JNIEXPORT void JNICALL Java_org_eclipse_efxclipse_fxgl_sample_renderer_simple_SimpleRenderer_nDestroy
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

