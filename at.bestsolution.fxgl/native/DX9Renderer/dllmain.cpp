#include "Renderer.h"

HINSTANCE hInstance;

BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	hInstance = hinstDLL;
	return TRUE;
}

