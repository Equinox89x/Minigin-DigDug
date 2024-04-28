#include "EntityMovementComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Timer.h"
#include "Renderer.h"
#include "InputComponent.h"
#include <SDL_rect.h>
#include "PathwayCreatorComponent.h"
#include <iostream>


void dae::EntityMovementComponent::Update()
{

	m_Rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
	SDL_Rect smallerRect{ m_Rect.x, m_Rect.y, m_Rect.w / 4, m_Rect.h / 4 };
	m_LeftRect = smallerRect;
	m_RightRect = smallerRect;
	m_BottomRect = smallerRect;
	m_TopRect = smallerRect;

	m_LeftRect.x -= m_Rect.w / 2;
	m_RightRect.x += (m_Rect.w) + m_RightRect.w*2;
	m_BottomRect.y += (m_Rect.h) + m_BottomRect.h*2;
	m_TopRect.y -= m_Rect.h / 2;

	m_LeftRect.y += (m_Rect.h / 4) + (m_LeftRect.h / 2);
	m_RightRect.y += (m_Rect.h / 4) + (m_RightRect.h / 2);
	m_BottomRect.x += (m_Rect.w / 4) + (m_BottomRect.w / 2);
	m_TopRect.x += (m_Rect.w / 4) + (m_TopRect.w / 2);

	m_Rect.h /= 4;
	m_Rect.w /= 4;
	m_Rect.x += static_cast<int>(m_Rect.w*1.5f);
	m_Rect.y += static_cast<int>(m_Rect.h*1.5f);

	m_BottomMoveRect = m_Rect;
	m_TopMoveRect = m_Rect;
	m_LeftMoveRect = m_Rect;
	m_RightMoveRect = m_Rect;

	m_BottomMoveRect.w = static_cast<int>(m_BottomMoveRect.w / 1.5f);
	m_TopMoveRect.w = static_cast<int>(m_TopMoveRect.w / 1.5f);
	m_LeftMoveRect.w = static_cast<int>(m_LeftMoveRect.w / 1.5f);
	m_RightMoveRect.w = static_cast<int>(m_RightMoveRect.w / 1.5f);
	m_BottomMoveRect.h = static_cast<int>(m_BottomMoveRect.h / 1.5f);
	m_TopMoveRect.h = static_cast<int>(m_TopMoveRect.h / 1.5f);
	m_LeftMoveRect.h = static_cast<int>(m_LeftMoveRect.h / 1.5f);
	m_RightMoveRect.h = static_cast<int>(m_RightMoveRect.h / 1.5f);

	m_BottomMoveRect.y += m_Rect.h;
	m_TopMoveRect.y -= m_Rect.h;
	m_LeftMoveRect.x -= m_Rect.w;
	m_RightMoveRect.x += m_Rect.w;

	if (!m_CanMove) return;
	if (m_IsAutonomous) {
		auto comp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
		CheckMovement(comp->GetHorizontalPathways(), comp->GetVerticalPathways());

		float deltaTime{ Timer::GetInstance().GetDeltaTime() };
		float speed{ deltaTime * m_Speed };

		switch (m_State)
		{
		case MathLib::EMovingState::MovingUp:
			GetGameObject()->GetTransform()->AddTranslate(0, -speed);
			break;
		case MathLib::EMovingState::MovingLeft:
			GetGameObject()->GetTransform()->AddTranslate(-speed, 0);
			break;
		case MathLib::EMovingState::MovingRight:
			GetGameObject()->GetTransform()->AddTranslate(speed, 0);
			break;
		case MathLib::EMovingState::MovingDown:
			GetGameObject()->GetTransform()->AddTranslate(0, speed);
			break;
		default:
			break;
		}
	}
}

void dae::EntityMovementComponent::Render() const
{
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftRect);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightRect);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopRect);	
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRect);	
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Rect);

	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftMoveRect);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightMoveRect);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopMoveRect);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomMoveRect);

	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftMapBorder);
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightMapBorder);
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopMapBorder);
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomMapBorder);
}

void dae::EntityMovementComponent::Init()
{

}

void dae::EntityMovementComponent::CheckMovement(const std::vector<SDL_Rect>& horizontal, const std::vector<SDL_Rect>& vertical)
{
	for (auto& item : m_MovementDir) {
		item.second = false;
	}
	m_CanChangeState = false;

	m_MoveTimer -= Timer::GetInstance().GetDeltaTime();
	m_LastHorLeft = m_CurrentHorLeft;
	m_LastHorRight = m_CurrentHorRight;
	m_LastVertTop = m_CurrentVertTop;
	m_LastVertBottom = m_CurrentVertBottom;

	//only try to change state when a new rectangle is detected and when it overlaps
	for (auto item : horizontal)
	{
		//if (!MathLib::IsSameRect(m_CurrentHorRight, item)) {
			if (MathLib::IsOverlapping(m_RightMoveRect, item)) {
				m_MovementDir[MathLib::EMovingState::MovingRight] = true;
				m_CanChangeState = true;
			}
			/*}
			if (!MathLib::IsSameRect(m_CurrentHorLeft, item)) {*/
			if (MathLib::IsOverlapping(m_LeftMoveRect, item)) {
				m_MovementDir[MathLib::EMovingState::MovingLeft] = true;
				m_CanChangeState = true;
			}
			//}
	}

	for (auto item : vertical) {
		/*if (!MathLib::IsSameRect(m_CurrentVertTop, item)) {*/
		if (MathLib::IsOverlapping(m_TopMoveRect, item)) {
			m_MovementDir[MathLib::EMovingState::MovingUp] = true;
			m_CanChangeState = true;
		}
		/*}
		if (!MathLib::IsSameRect(m_CurrentVertBottom, item)) {*/
		if (MathLib::IsOverlapping(m_BottomMoveRect, item)) {
			m_MovementDir[MathLib::EMovingState::MovingDown] = true;
			m_CanChangeState = true;
		}
		//}
	}

	//change state depending on overlap results
	if (m_MoveTimer > 0) return;
	if (m_CanChangeState) {

		std::vector<MathLib::EMovingState> movementDir;
		for (auto& item : m_MovementDir) {
			if (item.second) {
				movementDir.push_back(item.first);
			}
		}
		int randIndex{ MathLib::CalculateChance(static_cast<int>(movementDir.size())-1) };

		if (movementDir.size() > 0) {
			if (m_State != movementDir[randIndex]) {
				m_State = movementDir[randIndex];
				m_MoveTimer = 2;
				std::cout << "change" << std::endl;

			}
		}
		else{
			std::cout << "change2" << std::endl;
			m_MoveTimer = 2;
			switch (m_State)
			{
			case MathLib::MovingLeft:
				m_State = MathLib::EMovingState::MovingRight;
					break;
			case MathLib::MovingRight:
				m_State = MathLib::EMovingState::MovingLeft;
					break;
			case MathLib::MovingUp:
				m_State = MathLib::EMovingState::MovingDown;
					break;
			case MathLib::MovingDown:
				m_State = MathLib::EMovingState::MovingUp;
				break;
			case MathLib::Still:
				break;
			default:
				break;
			}
		}

	}
		if (MathLib::IsOverlapping(m_RightMapBorder, m_RightMoveRect)) {
			m_State = MathLib::EMovingState::MovingLeft;
			m_MoveTimer = 2;
		}
		if (MathLib::IsOverlapping(m_LeftMapBorder, m_LeftMoveRect)) {
			m_State = MathLib::EMovingState::MovingRight;
			m_MoveTimer = 2;
		}
		if (MathLib::IsOverlapping(m_TopMapBorder, m_TopMoveRect)) {
			m_State = MathLib::EMovingState::MovingDown;
			m_MoveTimer = 2;
		}
		if (MathLib::IsOverlapping(m_BottomMapBorder, m_BottomMoveRect)) {
			m_State = MathLib::EMovingState::MovingUp;
			m_MoveTimer = 2;
		}
}

void dae::EntityMovementComponent::SetMovement(MathLib::Movement movement)
{
	if (!m_CanMove) return;
	m_Movement = movement;
	switch (movement)
	{
	case MathLib::UP:
		m_PathwayColliderRect = m_TopRect;
		break;
	case MathLib::DOWN:
		m_PathwayColliderRect = m_BottomRect;
		break;
	case MathLib::LEFT:
		m_PathwayColliderRect = m_LeftRect;
		break;
	case MathLib::RIGHT:
		m_PathwayColliderRect = m_RightRect;
		break;
	default:
		break;
	}
}

SDL_Rect dae::EntityMovementComponent::GetPathCollider(MathLib::Movement movement)
{
	switch (movement)
	{
	case MathLib::UP:
		return m_TopMoveRect;
		break;
	case MathLib::DOWN:
		return m_BottomMoveRect;
		break;
	case MathLib::LEFT:
		return m_LeftMoveRect;
		break;
	case MathLib::RIGHT:
		return m_RightMoveRect;
		break;
	default:
		return SDL_Rect{};
		break;
	}
}
