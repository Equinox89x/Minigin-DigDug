#include "FPSCounterComponent.h"
#include "TextObjectComponent.h"
#include "TransformComponent.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include <Windows.h>
#include <memory>

dae::FPSCounterComponent::FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font) :
	m_prevFPS{ 0 },
	TextObjectComponent(text, font)
{ }

void dae::FPSCounterComponent::Update()
{
	TextObjectComponent::Update();

	//uint32_t fps{ Timer::GetInstance().GetFPS() };

	uint32_t fps{ 60 };
	if (m_prevFPS != fps) {
		TextObjectComponent::SetText(std::to_string(fps) + " FPS");
		m_prevFPS = fps;
	}
}

void dae::FPSCounterComponent::FixedUpdate()
{
}

void dae::FPSCounterComponent::Render() const
{
	TextObjectComponent::Render();
}

void dae::FPSCounterComponent::SetPosition(const float x, const float y)
{
	TextObjectComponent::SetPosition(x, y);
}


