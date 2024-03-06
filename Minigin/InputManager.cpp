#include <SDL.h>
#include "InputManager.h"
#include <backends/imgui_impl_sdl2.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

bool dae::InputManager::ProcessInput()
{
	XINPUT_STATE m_Currentstate;
	ZeroMemory(&m_Currentstate, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_Currentstate);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			//return true;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			//return true;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
		// etc...
	}

	return true;
}
