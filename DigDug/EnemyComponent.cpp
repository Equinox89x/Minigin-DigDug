#include "EnemyComponent.h"
#include "Timer.h"
#include "EntityMovementComponent.h"

dae::EnemyComponent::~EnemyComponent()
{
	delete m_State;
	m_State = nullptr;
}

void dae::EnemyComponent::Update()
{
	m_State->Update();
}

void dae::EnemyComponent::Render() const
{
}

void dae::EnemyComponent::Init()
{
	SetState(new MovingState());
}

bool dae::EnemyComponent::PumpUp()
{
	currentPumpStage++;
	GetGameObject()->GetComponent<TextureComponent>()->SetFrame(currentPumpStage);
	if (currentPumpStage >= maxPumpStage) {
		SetState(new DeathState());
	}
	return currentPumpStage >= maxPumpStage;
}

void dae::DeathState::Update()
{
	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_DeathTimer <= 0) {
		gameObject->MarkForDestroy();
	}
}

void dae::MovingState::Init()
{
	if (gameObject) {
		gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(false);
		gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
	}
}

void dae::MovingState::Update()
{
	if (MathLib::CalculateChance() >= 80) {
		if (EnemyType == EEnemyType::Fygar) {
			if (MathLib::CalculateChance() >= 50) {
				gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
			}
			else {
				gameObject->GetComponent<EnemyComponent>()->SetState(new BreatheFireState());
			}
		}
		else {
			gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
		}
	}
}

void dae::InflatingState::Update()
{
}

void dae::GhostState::Init()
{
	m_CachedLocation  = m_Scene->GetGameObject(EnumStrings[Names::Player0])->GetTransform()->GetWorldPosition();
	gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(true);
	gameObject->GetComponent<EntityMovementComponent>()->SetGhostLocation(m_CachedLocation);

}

void dae::GhostState::Update()
{
	//TODO if reached a dug tile near a player
	float dx = m_CachedLocation.x - gameObject->GetTransform()->GetWorldPosition().x;
	float dy = m_CachedLocation.y - gameObject->GetTransform()->GetWorldPosition().y;
	float distanceToTarget = std::sqrt(dx * dx + dy * dy);
	if (distanceToTarget <= 1) {
		gameObject->GetComponent<EnemyComponent>()->SetState(new MovingState());
	}


}

void dae::BreatheFireState::Init()
{
	gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);
}

void dae::BreatheFireState::Update()
{
	m_FireTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_FireTimer <= 0) {
		gameObject->GetComponent<EnemyComponent>()->SetState(new MovingState());
	}
}
