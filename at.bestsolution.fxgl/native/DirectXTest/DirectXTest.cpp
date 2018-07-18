#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <comdef.h>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

IDXGISwapChain *swapchain;
ID3D11Device *dev;
ID3D11DeviceContext *devCon;
ID3D11RenderTargetView *backbuffer;
ID3D11RenderTargetView *targetBuffer;

ID3D11RenderTargetView* multiTarget[2];

ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer


									   // a struct to define a single vertex
struct VERTEX { FLOAT X, Y, Z; D3DXCOLOR Color; };

struct VS_CONSTANT_BUFFER {
	float fRot;
};

float rotator = 0;

ID3D11Buffer *pConstantBuffer;
VS_CONSTANT_BUFFER VSConstData;


void InitD3D(HWND hWnd);
void CleanD3D(void);
void RenderFrame(void);

void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

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
		L"My C Window Direct3D 11 Context",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right- wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	InitD3D(hWnd);

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

	return msg.wParam;
}

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

void CleanD3D() {
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devCon->Release();
}

LPCTSTR convert(HRESULT result) {
	_com_error err(result);
	return err.ErrorMessage();
}

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

ID3D11Texture2D *targetTexture;

void InitD3D(HWND hWnd) {

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = true;


	HRESULT result = D3D11CreateDeviceAndSwapChain(
		NULL, // pAdapter
		D3D_DRIVER_TYPE_HARDWARE, // DriverType
		NULL, // Software
		NULL, // Flags
		NULL, // pFeatureLevels
		NULL, // FeatureLevels
		D3D11_SDK_VERSION, // SDKVersion
		&scd, // pSwapChainDesc
		&swapchain, // ppSwapChain
		&dev, // ppDevice
		NULL, // pFeatureLevel
		&devCon); // ppImmediateContext

	OutputDebugStringW(L"D3D initialized\n");
	OutputDebugStringW(dev==nullptr?L"dev is null!":L"");OutputDebugStringW(L"\n");
	report(result);

	// init backbuffer rendertarget

	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	

	ID3D11Texture2D *sharedTarget;
	
	/************************/
	/************************/
	/************************/
	HANDLE sharedHandle = (HANDLE) (long)-1073725246; // must be set to NULL to create, can use a valid handle here to open in D3D9 

	ID3D11Resource * tempResource11;
	
	


	HRESULT hr = dev->OpenSharedResource((HANDLE)sharedHandle, __uuidof(ID3D11Resource), (void**)(&tempResource11));
	if (FAILED(hr)) {
		OutputDebugStringW(L"shared resource: ");
		report(hr);
	}
	if (tempResource11 != 0) {
		tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&sharedTarget));
		tempResource11->Release();

		dev->CreateRenderTargetView(sharedTarget, NULL, &targetBuffer);

		
		multiTarget[0] = targetBuffer;
		multiTarget[1] = backbuffer;

		// TODO rendering to 2 textures at once seems not to work (yet)
		devCon->OMSetRenderTargets(1, &multiTarget[0], NULL);
	}
	else {
		OutputDebugStringW(L"Die SCheisse goat ned!!!! \n");
		devCon->OMSetRenderTargets(1, &backbuffer, NULL);
	}

	// viewport

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devCon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}

void RenderFrame() {
	devCon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f));
	//devCon->ClearRenderTargetView(targetBuffer, D3DXCOLOR(0.0f, 0.5f, 0.5f, 1.0f));
	devCon->ClearRenderTargetView(targetBuffer, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devCon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// select which primtive type we are using
	devCon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// update rotation
	rotator += 0.1f;
	if (rotator > 360) {
		rotator -= 360;
	}
	VSConstData.fRot = rotator / 180.0f * M_PI;
	
	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hr = devCon->Map(pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(hr)) {
		report(hr);
	}

	((VS_CONSTANT_BUFFER*)mapped.pData)->fRot = rotator / 180.0f * M_PI;

	devCon->Unmap(pConstantBuffer, 0);


	//VS_CONSTANT_BUFFER tmp;
	//tmp.fRot = rotator / 180.0f * M_PI;
	//// update const buffer in graka memory
	//devCon->UpdateSubresource(pConstantBuffer, 0, 0, &tmp, 0, 0);
	//// update const buffer in VS
	//devCon->VSSetConstantBuffers(0, 1, &pConstantBuffer);

	

	// draw the vertex buffer to the back buffer

	// HACKY 2 Buffer output (elegant version above does not work yet)
	if (targetBuffer != NULL) {
		devCon->OMSetRenderTargets(1, &targetBuffer, NULL);
		devCon->Draw(3, 0);
	}

	devCon->OMSetRenderTargets(1, &backbuffer, NULL);
	devCon->Draw(3, 0);
	


	swapchain->Present(0, 0);


	// update shared texture
	//devCon->Flush();
}

// this is the function that creates the shape to render
void InitGraphics()
{
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
	{ 0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
	{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	devCon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	devCon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}




// this function loads and prepares the shaders
void InitPipeline()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	ID3D10Blob *errBlob = nullptr;
	HRESULT r = D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &errBlob, 0);
	report(r);
	if (FAILED(r)) {
		if (errBlob) {
			OutputDebugStringA((char*)errBlob->GetBufferPointer());
			errBlob->Release();
		}
	}
	r = D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &errBlob, 0);
	report(r);
	if (FAILED(r)) {
		if (errBlob) {
			OutputDebugStringA((char*)errBlob->GetBufferPointer());
			errBlob->Release();
		}
	}
	// encapsulate both shaders into shader objects
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	devCon->VSSetShader(pVS, 0, 0);
	devCon->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devCon->IASetInputLayout(pLayout);


	D3D11_BUFFER_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.ByteWidth = max(16, sizeof(VS_CONSTANT_BUFFER)); // MULTIPLE OF 16 needed!!!!!!
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	VSConstData.fRot = 30.0f / 180.0f * M_PI ;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = &VSConstData;



	HRESULT hr = dev->CreateBuffer(&bDesc, &InitData, &pConstantBuffer);
	if (FAILED(hr)) {
		OutputDebugString(L"Constant Buffer Init failed!"); report(hr);
	}

	devCon->VSSetConstantBuffers(0, 1, &pConstantBuffer);
}

