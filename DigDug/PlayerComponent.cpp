#include "PlayerComponent.h"
#include <ValuesComponent.h>
#include "MenuComponent.h"
#include "EntityMovementComponent.h"
#include <InputComponent.h>
#include "EnemyComponent.h"

void dae::PlayerComponent::Update()
{
	float dt{ Timer::GetInstance().GetDeltaTime() };
	if (m_PlayerState == MathLib::RESPAWN) {
		m_InvincibilityTimer -= dt;
		if (m_InvincibilityTimer < 0) {
			m_PlayerState = MathLib::ALIVE;
			GetGameObject()->GetComponent<TextureComponent>()->SetIsVisible(true);
			GetGameObject()->GetComponent<InputComponent>()->DisableMovement(false);
			GetGameObject()->GetComponent<EntityMovementComponent>()->DisableMovement(false);
			m_InvincibilityTimer = 3;
		}
	}
	if (m_PlayerState == MathLib::INVINCIBLE) {
		m_InvincibilityTimer -= dt;
		if (m_InvincibilityTimer < 0) {
			m_PlayerState = MathLib::ALIVE;
			m_InvincibilityTimer = 3;
		}
	}

	if (m_PlayerState == MathLib::ALIVE) {
		if (m_Scene) {
			auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
			for (const auto& enemy : enemies) {
				auto lifestate{ enemy->GetComponent<EnemyComponent>()->GetLifeState() };
				if (lifestate != MathLib::DEAD && lifestate != MathLib::INFLATING) {
					if (MathLib::IsOverlapping(GetGameObject()->GetComponent<TextureComponent>()->GetRect(), enemy->GetComponent<TextureComponent>()->GetRect())) {
						Respawn();
					}
				}
			}
		}
	}
}

void dae::PlayerComponent::Render() const
{
}

void dae::PlayerComponent::Init()
{
}

void dae::PlayerComponent::Respawn()
{
	if (!GetGameObject()) return;
	if (GetGameObject()->IsMarkedForDestroy()) return;
	if (GetGameObject()->GetComponent<ValuesComponent>()->GetLives() > 0) {
		if (m_PlayerState == MathLib::ALIVE && m_PlayerState != MathLib::INVINCIBLE) {
			m_PlayerState = MathLib::RESPAWN;
			GetGameObject()->GetComponent<ValuesComponent>()->Damage();
			GetGameObject()->GetTransform()->Translate(m_OriginalPosition);
			GetGameObject()->GetComponent<TextureComponent>()->SetIsVisible(false);
			GetGameObject()->GetComponent<InputComponent>()->DisableMovement(true);
			GetGameObject()->GetComponent<EntityMovementComponent>()->DisableMovement(true);
		}
	}
	else {
		GetGameObject()->GetComponent<ValuesComponent>()->Damage();
	}
}