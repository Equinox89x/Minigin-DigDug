#include "Timer.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "../DigDug/PathwayCreatorComponent.h"
#include "../DigDug/EntityMovementComponent.h"

void dae::InputComponent::UpdatePos(float dt)
{
	if (!m_CanMove) return;
	if (m_Movespeed.y > 0 || m_Movespeed.y < 0 || m_Movespeed.x > 0 || m_Movespeed.x < 0)
	{
		auto go{ GetGameObject() };

		glm::vec3 currentPosition = go->GetTransform()->GetPosition();
		glm::vec3 futurePosition = glm::vec3(currentPosition.x + (m_Movespeed.x * dt), currentPosition.y + (m_Movespeed.y * dt), 1);

		if (futurePosition.x < 0 || futurePosition.x >(WindowSizeX) - PlayerSize) return;

		if (!m_Movement[MathLib::Side::Top] && m_Movespeed.y < 0) return;
		if (!m_Movement[MathLib::Side::Bottom] && m_Movespeed.y > 0) return;
		if (!m_Movement[MathLib::Side::Left] && m_Movespeed.x < 0) return;
		if (!m_Movement[MathLib::Side::Right] && m_Movespeed.x > 0) return;
		go->GetTransform()->Translate(futurePosition.x, futurePosition.y, 0);
		//m_Movespeed = glm::vec3{ 0,0,0 };
	}
}

void dae::InputComponent::Update()
{

	auto dt{ Timer::GetInstance().GetDeltaTime() };

	auto pathwayComp{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator])->GetComponent<PathwayCreatorComponent>() };
	auto playerComp{ m_Scene->GetGameObject(EnumStrings[Names::Player0])->GetComponent<EntityMovementComponent>() };

	m_Movement[MathLib::Side::Left] = false;
	m_Movement[MathLib::Side::Right] = false;
	m_Movement[MathLib::Side::Bottom] = false;
	m_Movement[MathLib::Side::Top] = false;

	for (const auto& pathWay : pathwayComp->GetHorizontalPathways()) {
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::LEFT), pathWay)) {
			m_Movement[MathLib::Side::Left] = true;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::RIGHT), pathWay)) {
			m_Movement[MathLib::Side::Right] = true;
		}
	}
	for (const auto& pathWay : pathwayComp->GetVerticalPathways()) {
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::DOWN), pathWay)) {
			m_Movement[MathLib::Side::Bottom] = true;
		}
		if (MathLib::IsOverlapping(playerComp->GetPathCollider(MathLib::Movement::UP), pathWay)) {
			m_Movement[MathLib::Side::Top] = true;
		}
	}

	UpdatePos(dt);
}

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed)
{
	m_Movespeed = movespeed;
}

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed, MathLib::Movement direction)
{
	m_MoveSpeedList.insert(std::pair<MathLib::Movement, const glm::vec3>{ direction, movespeed });
	m_Movespeed = movespeed;
}

void dae::InputComponent::StopMovement(MathLib::Movement direction)
{
	m_MoveSpeedList.erase(direction);
	m_Movespeed = m_MoveSpeedList.size() > 0 ? m_MoveSpeedList.begin()->second : glm::vec3{ 0,0,0 };
}
