#include "FPSCounterComponent.h"
#include "TextObjectComponent.h"
#include "TransformComponent.h"
#include <SDL_ttf.h>
#include <stdexcept>
#include <Windows.h>
#include <memory>

dae::FPSCounterComponent::FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font) :
	m_FPS{ 0 },
	TextObjectComponent(text, font)
{ }

void dae::FPSCounterComponent::Update()
{
	TextObjectComponent::Update();

	//uint32_t fps{ Timer::GetInstance().GetFPS() };

	m_EndTime = std::chrono::steady_clock::now();
	m_ElapsedTime = std::chrono::duration<float>(m_EndTime - m_StartTime).count();
	m_FPS = static_cast<int>(1.0f / m_ElapsedTime);

	TextObjectComponent::SetText(std::to_string(m_FPS) + " FPS");
	m_StartTime = m_EndTime;
}

void dae::FPSCounterComponent::Init()
{
	m_StartTime = std::chrono::steady_clock::now();
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


