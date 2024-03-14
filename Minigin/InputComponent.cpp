#include "Timer.h"
#include "InputComponent.h"
#include "GameObject.h"

void dae::InputComponent::UpdatePos(float dt)
{
	if (m_Movespeed.y > 0 || m_Movespeed.y < 0 || m_Movespeed.x > 0 || m_Movespeed.x < 0)
	{
		auto go{ GetGameObject() };

		glm::vec3 curPos = go->GetTransform()->GetPosition();
		glm::vec3 furPos = glm::vec3(curPos.x + (m_Movespeed.x * dt), curPos.y + (m_Movespeed.y * dt), 1);

		if (furPos.x < 0 || furPos.x >(GameWindowSizeX) - PlayerSize) return;

		if (!m_Movement[MathLib::Side::Top] && m_Movespeed.y < 0) return;
		if (!m_Movement[MathLib::Side::Bottom] && m_Movespeed.y > 0) return;
		if (!m_Movement[MathLib::Side::Left] && m_Movespeed.x < 0) return;
		if (!m_Movement[MathLib::Side::Right] && m_Movespeed.x > 0) return;
		go->GetTransform()->Translate(furPos.x, furPos.y, 0);
	}
}

void dae::InputComponent::Update()
{

	auto dt{ Timer::GetInstance().GetDeltaTime() };

	UpdatePos(dt);
}

void dae::InputComponent::FixedUpdate()
{
}

void dae::InputComponent::Render() const
{
}

void dae::InputComponent::SetMoveSpeed(const glm::vec3& movespeed)
{
	m_Movespeed = movespeed;
}