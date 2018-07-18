#include <windows.h>
#include <dxgiformat.h>

#include <jni.h>

#include "JavaFXInternals.h"

extern "C" JNIEXPORT jlong JNICALL Java_at_bestsolution_fxgl_DXUtils_noparam
(JNIEnv *env, jclass)
{
	return 0;
}
extern "C" JNIEXPORT jlong JNICALL Java_at_bestsolution_fxgl_DXUtils_texparam
(JNIEnv *env, jclass, jobject texture)
{
	return 1;
}
extern "C" JNIEXPORT jlong JNICALL Java_at_bestsolution_fxgl_DXUtils_nRecreateTexture
(JNIEnv *env, jclass, jobject texture)
{
	jclass d3dTextureCls = env->FindClass("com/sun/prism/d3d/D3DTexture");
	if (!d3dTextureCls) return -1;
	jboolean isd3d = env->IsInstanceOf(texture, d3dTextureCls);
	if (!isd3d) return -1;

	jmethodID getContextMth = env->GetMethodID(d3dTextureCls, "getContext", "()Lcom/sun/prism/d3d/D3DContext;");
	if (!getContextMth) return -1;
	jobject contextObj = env->CallObjectMethod(texture, getContextMth);
	if (!contextObj) return -1;

	jclass d3DContextCls = env->FindClass("com/sun/prism/d3d/D3DContext");
	if (!d3DContextCls) return -1;
	jmethodID getContextHandleMth = env->GetMethodID(d3DContextCls, "getContextHandle", "()J");
	if (!getContextHandleMth) return -1;
	jlong contextHandle = env->CallLongMethod(contextObj, getContextHandleMth);
	if (!contextHandle) return -1;

	jmethodID getNativeSourceHandleMth = env->GetMethodID(d3dTextureCls, "getNativeSourceHandle", "()J");
	if (!getNativeSourceHandleMth) return -1;
	jlong resourceHandle = env->CallLongMethod(texture, getNativeSourceHandleMth);
	if (!resourceHandle) return -1;

	D3DContext *context = (D3DContext*)jlong_to_ptr(contextHandle);
	D3DResource *d3dresource = (D3DResource*)jlong_to_ptr(resourceHandle);

	// cleanup old original java texture
	d3dresource->pSurface->Release();
	d3dresource->pTexture->Release();
	d3dresource->pResource->Release();

	d3dresource->pSurface = nullptr;
	d3dresource->pTexture = nullptr;
	d3dresource->pResource = nullptr;

	HANDLE pSharedHandle = NULL;
	IDirect3DTexture9 *pTexture = NULL;
	HRESULT res = context->pd3dDeviceEx->CreateTexture(d3dresource->desc.Width, d3dresource->desc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture, &pSharedHandle);
	//HRESULT res = context->pd3dDeviceEx->CreateTexture(800, 600, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &pTexture, NULL);

	if (FAILED(res))
		return -1;


	d3dresource->pResource = pTexture;

	// addRef is needed because both pResource and pTexture will be
	// Release()d, and they point to the same object
	d3dresource->pResource->AddRef();
	d3dresource->pTexture = (IDirect3DTexture9*)d3dresource->pResource;
	d3dresource->pTexture->GetSurfaceLevel(0, &d3dresource->pSurface);
	if (d3dresource->pSurface != NULL)
	{
		d3dresource->pSurface->GetDesc(&d3dresource->desc);
	}

	return (long) &pSharedHandle;
}

extern "C" JNIEXPORT jlong JNICALL Java_at_bestsolution_fxgl_DXUtils_nGetAdapterLUID
(JNIEnv *env, jclass)
{
	jclass graphicsPipelineCls = env->FindClass("com/sun/prism/GraphicsPipeline");
	if (!graphicsPipelineCls) return 0;
	jmethodID getDefaultResourceFactoryMth = env->GetStaticMethodID(graphicsPipelineCls, "getDefaultResourceFactory", "()Lcom/sun/prism/ResourceFactory;");
	if (!getDefaultResourceFactoryMth) return 0;
	jobject resourceFactoryObj = env->CallStaticObjectMethod(graphicsPipelineCls, getDefaultResourceFactoryMth);
	if (!resourceFactoryObj) return 0;

	jclass d3DResourceFactoryCls = env->FindClass("com/sun/prism/d3d/D3DResourceFactory");
	if (!d3DResourceFactoryCls) return 0;
	jboolean isd3d = env->IsInstanceOf(resourceFactoryObj, d3DResourceFactoryCls);
	if (!isd3d) return 0;

	jmethodID getContextMth = env->GetMethodID(d3DResourceFactoryCls, "getContext", "()Lcom/sun/prism/d3d/D3DContext;");
	if (!getContextMth) return 0;
	jobject contextObj = env->CallObjectMethod(resourceFactoryObj, getContextMth);
	if (!contextObj) return 0;

	jclass d3DContextCls = env->FindClass("com/sun/prism/d3d/D3DContext");
	if (!d3DContextCls) return 0;
	jmethodID getContextHandleMth = env->GetMethodID(d3DContextCls, "getContextHandle", "()J");
	if (!getContextHandleMth) return 0;
	jlong contextHandle = env->CallLongMethod(contextObj, getContextHandleMth);
	if (!contextHandle) return 0;

	D3DContext *context = (D3DContext*)jlong_to_ptr(contextHandle);

	union
	{
		LUID luid;
		unsigned long long luidAs64;
	};
	context->pd3dObjectEx->GetAdapterLUID(context->adapterOrdinal, &luid);

	return luidAs64;
}
