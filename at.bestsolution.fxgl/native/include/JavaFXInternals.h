#ifndef __JAVAFX_INTERNALS_H__
#define __JAVAFX_INTERNALS_H__

#include <d3d9.h>

#ifndef jlong_to_ptr
#ifdef _WIN64
#define jlong_to_ptr(a) ((void*)(a))
#define ptr_to_jlong(a) ((jlong)(a))
#else
/* Double casting to avoid warning messages looking for casting of */
/* smaller sizes into pointers */
#define jlong_to_ptr(a) ((void*)(int)(a))
#define ptr_to_jlong(a) ((jlong)(int)(a))
#endif
#endif

#define NUM_TEXTURE_CACHE 8

// copied from JAVAFX\modules\graphics\src\main\native-prism-d3d\D3DContext.h
// valid only for JRE8 update 60 to update 152
struct D3DContext
{
	struct FrameStats {
		int numTrianglesDrawn;
		int numDrawCalls;
		int numBufferLocks;
		int numTextureLocks;
		int numTextureTransferBytes;
		int numSetTexture;
		int numSetPixelShader;
		int numRenderTargetSwitch;
	} stats;

	struct State {
		bool wireframe;
		int cullMode;
	} state;

	IDirect3DVertexShader9 *pPassThroughVS;
	IDirect3DVertexDeclaration9 *pVertexDecl;
	IDirect3DIndexBuffer9 *pIndices;
	struct D3DVertexBufferResource  *pVertexBufferRes;

	D3DMATRIX world;
	D3DMATRIX projection;
	BOOL depthTest;
	float pixadjustx, pixadjusty;

	IDirect3DDevice9        *pd3dDevice;
	IDirect3DDevice9Ex      *pd3dDeviceEx;
	IDirect3DSurface9       *currentSurface;
	IDirect3D9              *pd3dObject;
	IDirect3D9Ex            *pd3dObjectEx;

	D3DPOOL defaulResourcePool;

	struct D3DResourceManager      *pResourceMgr;

	UINT adapterOrdinal;
	BOOL bIsHWRasterizer;

	D3DPRESENT_PARAMETERS   curParams;
	D3DCAPS9                devCaps;

	BOOL    bBeginScenePending;

	struct D3DPhongShader *phongShader;
	struct TextureUpdateCache {
		IDirect3DTexture9 *texture;
		IDirect3DSurface9 *surface;
		int width, height;
	} textureCache[NUM_TEXTURE_CACHE];
};

struct IManagedResource {
	void* virtualTable;

	IManagedResource* pPrev;
	IManagedResource* pNext;
};

struct D3DResource {
	IManagedResource managedResource;
	IDirect3DResource9*  pResource;
	IDirect3DSwapChain9* pSwapChain;
	IDirect3DSurface9*   pSurface;
	IDirect3DSurface9*   pDepthSurface;
	IDirect3DTexture9*   pTexture;

	D3DSURFACE_DESC      desc;
};

struct D3DResourceManager {
	D3DResource*      pBlitOSPSurface;
	D3DContext*       pCtx;
	IManagedResource* pHead;
};


#endif // __JAVAFX_INTERNALS_H__
