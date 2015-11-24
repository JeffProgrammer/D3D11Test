//-----------------------------------------------------------------------------
// Copyright (c) 2015, Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of D3D11Test nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

// Require MSVC compiler for this application
#ifndef _MSC_VER
#error "You must use MSVC to compile this application."
#endif // _MSC_VER

#include <stdlib.h>
#include <SDL.h>
#include <SDL_syswm.h>

// Windows specific header + D3D11 SDK header
#include <Windows.h>
#include <d3d11.h>

// link with D3D11.lib
#pragma comment(lib, "D3D11.lib")

// SDL2 hack.
#ifdef main
#undef main
#endif // main

// used to signify that the variable is being outputted
#define OUT 

HWND getWindowHandle(SDL_Window *window);
bool createD3D11DeviceAndSwapChain(HWND window, OUT ID3D11Device *device, OUT IDXGISwapChain *swapChain, OUT ID3D11DeviceContext *context);

int main(int argc, const char **argv) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	// create window
	SDL_Window *window = SDL_CreateWindow("D3D11 Test Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		SDL_Quit();
		return 1;
	}

	// get window handle from the window
	HWND windowHandle = getWindowHandle(window);
	if (windowHandle == nullptr) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 2;
	}

	ID3D11Device *device = nullptr;
	IDXGISwapChain *swapChain = nullptr;
	ID3D11DeviceContext *context = nullptr;
	bool r = createD3D11DeviceAndSwapChain(windowHandle, device, swapChain, context);
	if (r == false) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 3;
	}


	SDL_Event event;
	bool running = true;
	while (running) {
		// Run event loop.
		while (SDL_PollEvent(&event)) {
			// check to see if we fired a quit event. If we did, we're done
			// running the program.
			if (event.type == SDL_QUIT) {
				running = false;
				break;
			}
		}
	}

	// cleanup
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

HWND getWindowHandle(SDL_Window *window) {
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	if (!SDL_GetWindowWMInfo(window, &info))
		return nullptr;

	return info.info.win.window;
}

bool createD3D11DeviceAndSwapChain(HWND window, OUT ID3D11Device *device, OUT IDXGISwapChain *swapChain, OUT ID3D11DeviceContext *context) {
	// create swap chain description
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.BufferCount = 1; // how many back buffers
	swapDesc.BufferDesc.Width = 1336; // window width
	swapDesc.BufferDesc.Height = 768; // window height
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // standard backbuffer format
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; // vsync
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; // vsync
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // yes we are rendering to a window
	swapDesc.OutputWindow = window; // the window we are outputting to
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0; // Antialiasing
	swapDesc.Windowed = true; // we are not in fullscreen

	// request feature level 11_0
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT featureLevelCount = 1;
	D3D_FEATURE_LEVEL featureLevelSupported;

	HRESULT r = D3D11CreateDeviceAndSwapChain(
		NULL,								// adapter
		D3D_DRIVER_TYPE_HARDWARE,	// device type
		NULL,								// do we want software emulation for features not supported?
		0,									// device flags
		&featureLevel,					// feature level(s) requested
		featureLevelCount,			// amount of feature level(s) requested
		D3D11_SDK_VERSION,			// version of the DX11 SDK
		&swapDesc,						// swap chain description
		&swapChain,						// OUT the swap chain pointer
		&device,							// OUT the d3d11 device
		&featureLevelSupported,		// OUT the d3d11 feature level that is supported on this computer.
		&context							// OUT the d3d11 context
	);
	if (r == S_OK)
		return true;
	return false;
}