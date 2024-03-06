#pragma once
#include "Singleton.h"
#include <backends/imgui_impl_sdl2.h>

#include <Windows.h>
#include <Xinput.h>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}
