#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <iostream>

#include "../include/jni.h"

#include "Texture.h"
#include "SharedMemory.h"

using namespace std;
using namespace at::bestsolution::fxgl;

jobject buffer;

TextureMemory* tex;

Texture getForeground() {
	return tex->textures[tex->foreground];
}
Texture getBackground() {
	return tex->textures[tex->background];
}

Texture getNextTexture() {
	int index = tex->nextTexture;
	return tex->textures[index];
	tex->usedTextureId = tex->textures[index].textureId;
}

void renderTexture() {
	tex->renderTextureId = tex->usedTextureId;
}

extern "C" JNIEXPORT jobject JNICALL Java_at_bestsolution_fxgl_internal_SharedMemory_nGetSharedBuffer(JNIEnv *env, jclass cls, jlong surfaceId) {
	cout << "nGetSharedBuffer" << endl;
	if (tex == 0) {
		key_t key = 0;
		size_t size = 2 * sizeof(jint);
		int texId = shmget(key, sizeof(TextureMemory), IPC_CREAT | 0666);
		tex = (TextureMemory*) shmat(texId, 0, 0);

		int texturesId = shmget(key, sizeof(Texture) * 3, IPC_CREAT | 0666);
		tex->textures = (Texture*) shmat(texturesId, 0, 0);

		buffer = env->NewDirectByteBuffer(tex->textures, sizeof(Texture) * 3);

		// init

		tex->foreground = 1;
		tex->background = 2;
	}

	return buffer;
}

extern "C" JNIEXPORT long JNICALL Java_at_bestsolution_fxgl_internal_SharedMemory_nGetForegroundAddress(JNIEnv *env, jclass cls) {
	return (long) &(tex->foreground);
}

extern "C" JNIEXPORT long JNICALL Java_at_bestsolution_fxgl_internal_SharedMemory_nGetBackgroundAddress(JNIEnv *env, jclass cls) {
	return (long) &(tex->background);
}

extern "C" JNIEXPORT long JNICALL Java_at_bestsolution_fxgl_internal_SharedMemory_nGetNextTextureAddress(JNIEnv *env, jclass cls) {
	return (long) &(tex->nextTexture);
}
extern "C" JNIEXPORT long JNICALL Java_at_bestsolution_fxgl_internal_SharedMemory_nGetRenderTextureAddress(JNIEnv *env, jclass cls) {
	return (long) &(tex->renderTextureId);
}

extern "C" JNIEXPORT long JNICALL Java_at_bestsolution_fxgl_internal_SharedMemory_nOutput(JNIEnv *env, jclass cls) {
	cout << "TextureMemory" << endl
			<< "0: " << tex->textures[0].textureId << " (" << tex->textures[0].width << "x" << tex->textures[0].height << ")" << endl
			<< "1: " << tex->textures[1].textureId << " (" << tex->textures[1].width << "x" << tex->textures[1].height << ")" << endl
			<< "2: " << tex->textures[2].textureId << " (" << tex->textures[2].width << "x" << tex->textures[2].height << ")" << endl
			<< "foreground: " << tex->foreground << endl
			<< "background: " << tex->background << endl;
}
