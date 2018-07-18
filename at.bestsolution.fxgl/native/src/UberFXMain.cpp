#include <windows.h>
#include <dxgiformat.h>

#include <jni.h>

#include "JavaFXInternals.h"


BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	return TRUE;
}

static D3DFORMAT DXGIToCrossAPID3D9Format(DXGI_FORMAT dxgiFormat)
{
	switch (dxgiFormat)
	{
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		return D3DFMT_A8R8G8B8;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		return D3DFMT_A8R8G8B8;
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		return D3DFMT_X8R8G8B8;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return D3DFMT_A8B8G8R8;
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		return D3DFMT_A8B8G8R8;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		return D3DFMT_A2B10G10R10;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return D3DFMT_A16B16G16R16F;
	default:
		return D3DFMT_UNKNOWN;
	};
}



extern "C" JNIEXPORT jint JNICALL Java_at_bestsolution_fxgl_NGGLCanvas_nRegisterTexture
(JNIEnv *env, jclass, jobject texture, jlong handle, jint format)
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

	HANDLE sharedHandle = (HANDLE)handle;
	IDirect3DTexture9 *pTexture = NULL;
	HRESULT res = context->pd3dDeviceEx->CreateTexture(d3dresource->desc.Width, d3dresource->desc.Height, 1, 0, DXGIToCrossAPID3D9Format((DXGI_FORMAT)format), D3DPOOL_DEFAULT, &pTexture, &sharedHandle);
	
	if (FAILED(res))
		return res;

	
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

	return 0;
}

extern "C" JNIEXPORT jint JNICALL Java_at_bestsolution_fxgl_NGGLCanvas_blub
(JNIEnv *env, jclass, jobject texture)
{
	return 99;
}