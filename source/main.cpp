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

// Windows specific header + D3D11 SDK header
#include <Windows.h>
#include <d3d11.h>

// SDL2 hack.
#ifdef main
#undef main
#endif // main

int main(int argc, const char **argv) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	// create window
	SDL_Window *window = SDL_CreateWindow("D3D11 Test Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		SDL_Quit();
		return 1;
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