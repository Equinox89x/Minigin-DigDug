#include "PlayerComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Timer.h"
#include "Renderer.h"
#include "InputComponent.h"
#include <SDL_rect.h>


void dae::PlayerComponent::Update()
{
	
}

void dae::PlayerComponent::Render() const
{
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomLeft);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRight);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftRect);
}

void dae::PlayerComponent::CheckMovement(/*const std::vector<std::pair<SDL_Rect, GameObject*>>& rects*/)
{
	m_Rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();

	//const auto xLeft{ m_Rect.x };
	//const auto xRight{ m_Rect.x + m_Rect.w };
	//m_BottomLeft = SDL_Rect{ xLeft, m_Rect.y + m_Rect.h, m_Rect.w / 3, m_Rect.h / 3 };
	//m_BottomRight = SDL_Rect{ xRight, m_Rect.y + m_Rect.h, m_Rect.w / 3, m_Rect.h / 3 };
	//m_BottomRight.x -= m_BottomRight.w;

	//m_LeftRect = m_Rect;
	//m_LeftRect.w /= 5;
	//m_LeftRect.x -= m_LeftRect.w;
	//m_LeftRect.h /= 2;
	//m_LeftRect.y += m_Rect.h / 4;
}