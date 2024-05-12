#include "PumpComponent.h"
#include "EntityMovementComponent.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include "PathwayCreatorComponent.h"
#include "InputComponent.h"

void dae::PumpComponent::Init()
{
	m_Player = m_Scene->GetGameObject(EnumStrings[Names::Player0]).get();
}

void dae::PumpComponent::Pump(glm::vec3 loc)
{
	//On space press, we shoot, or pump if we have an enemy attached
	if (m_SelectedEnemy) {
		//Stop pump movement
		//m_CanMove = false;
		//m_SelectedEnemy->GetComponent<dae::EntityMovementComponent>()->DisableMovement(true);
		//release if enemy dead
		if (m_SelectedEnemy->IsMarkedForDestroy()) {
			ReleasePump();
		}
		else if (m_SelectedEnemy->GetComponent<EnemyComponent>()->PumpUp()) {
			ReleasePump();
		}
	}
	else {
		//Start moving the pump
		m_CanMove = true;
		ResetMovement(loc);
		GetGameObject()->GetComponent<TextureComponent>()->SetIsVisible(true);
	}
}

void dae::PumpComponent::ResetMovement(glm::vec3 loc)
{
	m_StartPos = loc;
	GetGameObject()->GetTransform()->Translate(m_StartPos);
}

void dae::PumpComponent::ReleasePump()
{
	m_CanMove = false;
	m_Player->GetComponent<dae::EntityMovementComponent>()->DisableMovement(false);
	m_Player->GetComponent<dae::InputComponent>()->DisableMovement(false);
	m_SelectedEnemy = nullptr;
	ResetMovement({0,0,0});
	GetGameObject()->GetComponent<TextureComponent>()->SetIsVisible(false);
}

void dae::PumpComponent::AttachPump(GameObject* enemy)
{
	m_SelectedEnemy = enemy;
	auto comp{ m_SelectedEnemy->GetComponent<dae::EntityMovementComponent>() };
	m_SelectedEnemy->GetComponent<TextureComponent>()->SetTexture("Enemies/"+ comp->GetEnemyName() + "Inflate.png", 0.f, 4, true, false);
	comp->DisableMovement(true);
	m_CanMove = false;
}

void dae::PumpComponent::Update()
{
	auto texComp = GetGameObject()->GetComponent<TextureComponent>();

	UpdatePosition();

	if (!m_SelectedEnemy) {
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		if (enemies.size() > 0) {
			for (auto enemy : enemies) {
				if (MathLib::IsOverlapping(enemy->GetComponent<TextureComponent>()->GetRect(), texComp->GetRect())) {
					AttachPump(enemy.get());
				}
			}
		}
	}

	auto paths{ m_Scene->GetGameObject(EnumStrings[Names::PathCreator]) };
	auto pathcomp{ paths->GetComponent<PathwayCreatorComponent>() };
	for (auto path : pathcomp->GetPathways()) {
		//Release Pump if it hits a wall;
		if (MathLib::IsOverlapping(path.second.TextureComponent->GetRect(), texComp->GetRect())) {
			if (path.second.PathState == EPathState::Undug) {
				ReleasePump();
			}
		}
	}
}


void dae::PumpComponent::UpdatePosition()
{
	if (!m_CanMove) return;
	auto dt = Timer::GetInstance().GetDeltaTime();
	auto speed = dt * 200.f;
	auto dir = m_Player->GetComponent<EntityMovementComponent>()->GetDirection();
	switch (dir)
	{
	case MathLib::UP:
		GetGameObject()->GetTransform()->AddTranslate(0, -speed);
		break;
	case MathLib::DOWN:
		GetGameObject()->GetTransform()->AddTranslate(0, speed);
		break;
	case MathLib::LEFT:
		GetGameObject()->GetTransform()->AddTranslate(-speed, 0);
		break;
	case MathLib::RIGHT:
		GetGameObject()->GetTransform()->AddTranslate(speed, 0);
		break;
	default:
		break;
	}
}