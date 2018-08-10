#include "Renderer.h"
#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <comdef.h>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")


LPDIRECT3D9EX d3d;
LPDIRECT3DDEVICE9EX d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer

LPDIRECT3DTEXTURE9 targetTexture;
LPDIRECT3DSURFACE9 targetSurface, backBuffer;


void InitD3D(HWND hWnd, long sharedHandle);
void RenderFrame(void);
void CleanD3D(void);
void init_graphics(void);    // 3D declarations

struct CUSTOMVERTEX { FLOAT X, Y, Z; DWORD COLOR; };
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)


void report(HRESULT result) {
	switch (result) {
	case D3D11_ERROR_FILE_NOT_FOUND:
		OutputDebugStringW(L"D3D11_ERROR_FILE_NOT_FOUND");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		OutputDebugStringW(L"D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		OutputDebugStringW(L"D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS");
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		OutputDebugStringW(L"D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD");
		break;
	case DXGI_ERROR_INVALID_CALL:
		OutputDebugStringW(L"DXGI_ERROR_INVALID_CALL");
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		OutputDebugStringW(L"DXGI_ERROR_WAS_STILL_DRAWING");
		break;
	case E_FAIL:
		OutputDebugStringW(L"E_FAIL");
		break;
	case E_INVALIDARG:
		OutputDebugStringW(L"E_INVALIDARG");
		break;
	case E_OUTOFMEMORY:
		OutputDebugStringW(L"E_OUTOFMEMORY");
		break;
	case S_FALSE:
		OutputDebugStringW(L"S_FALSE");
		break;
	case S_OK:
		OutputDebugStringW(L"S_OK");
		break;
	default:
		OutputDebugStringW(L"We do not know why!");
	}
	OutputDebugStringW(L"\n");
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void InitD3D(HWND hWnd, long sharedHandle) {
	HRESULT ra = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d);
	
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;

	D3DDISPLAYMODEEX d3dm;
	ZeroMemory(&d3dm, sizeof(d3dm));
	

	HRESULT rb = d3d->CreateDeviceEx(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		NULL,
		&d3ddev);


	// init render target
	d3ddev->GetRenderTarget(0, &backBuffer);

	HANDLE pSharedHandle = (HANDLE) sharedHandle;
	HRESULT hr = d3ddev->CreateTexture(800, 600, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &targetTexture, &pSharedHandle);
	if (FAILED(hr)) {
		OutputDebugStringW(L"shared resource: ");
		report(hr);

		d3ddev->CreateTexture(800, 600, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &targetTexture, NULL);
	}
	targetTexture->GetSurfaceLevel(0, &targetSurface);

	init_graphics();    // call the function to initialize the triangle
}

float rotator = 0;


void RenderFrame() {
	// update rotation
	rotator += 0.1f;
	if (rotator > 360) {
		rotator -= 360;
	}

	// TODO Animation!!
	D3DXMATRIX matRot;
	//D3DXMatrixTranslation(&matRot, .0f, 0.f, 0.f);
	D3DXMatrixRotationZ(&matRot, D3DXToRadian(rotator));


	d3ddev->SetRenderTarget(0, backBuffer);
	{
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
		d3ddev->BeginScene();
		d3ddev->SetTransform(D3DTS_WORLD, &matRot);
		// select which vertex format we are using
		d3ddev->SetFVF(CUSTOMFVF);
		// select the vertex buffer to display
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		// copy the vertex buffer to the back buffer
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		d3ddev->EndScene();
	}
	d3ddev->SetRenderTarget(0, targetSurface);
	{
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), .0f, 0);
		d3ddev->BeginScene();
		d3ddev->SetTransform(D3DTS_WORLD, &matRot);
		// select which vertex format we are using
		d3ddev->SetFVF(CUSTOMFVF);
		// select the vertex buffer to display
		d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		// copy the vertex buffer to the back buffer
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		d3ddev->EndScene();
	}
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void CleanD3D() {
	
	targetSurface->Release();
	targetTexture->Release();
	backBuffer->Release();
	d3ddev->Release();
	d3d->Release();
	v_buffer->Release();    // close and release the vertex buffer
}
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Run(long sharedHandle) {
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"WindowClass",
		L"My C Window Direct3D 9 Context",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, SW_SHOW);

	InitD3D(hWnd, sharedHandle);

	// main loop
	MSG msg;

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
		}
		RenderFrame();
	}

	CleanD3D();
}

// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
	// create the vertices using the CUSTOMVERTEX struct
	CUSTOMVERTEX vertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DCOLOR_XRGB(0, 0, 255) },
	{ 0.45f, -0.5, 0.0f, D3DCOLOR_XRGB(0, 255, 0) },
	{ -0.45f, -0.5f, 0.0f, D3DCOLOR_XRGB(255, 0, 0) }
	};

	// create a vertex buffer interface called v_buffer
	HRESULT hr = d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_DEFAULT,
		&v_buffer,
		NULL);

	VOID* pVoid;    // a void pointer

					// lock v_buffer and load the vertices into it
	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();
}