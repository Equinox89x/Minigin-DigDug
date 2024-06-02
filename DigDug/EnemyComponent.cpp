#include "EnemyComponent.h"
#include "Timer.h"
#include "EntityMovementComponent.h"
#include "FireComponent.h"
#include <ValuesComponent.h>
#include "Observers.h"

dae::EnemyComponent::~EnemyComponent()
{
	delete m_State;
	m_State = nullptr;

	m_Player = nullptr;
}

void dae::EnemyComponent::Update()
{
	m_State->Update();
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
		Event scoreEvent{ EventType::EnemyDeath };
		Notify(GetGameObject(), scoreEvent);
	}
	return currentPumpStage >= maxPumpStage;
}

void dae::DeathState::Init()
{
	if (EnemyType == EEnemyType::Pooka) {
		//add score based on layer
	}
	else {
		//add score based on layer
	}
}

void dae::DeathState::Update()
{
	m_DeathTimer -= Timer::GetInstance().GetDeltaTime();
	if (m_DeathTimer <= 0) {
		gameObject->GetComponent<EnemyComponent>()->GetPlayer()->GetComponent<ValuesComponent>()->IncreaseScore(m_Score);
		gameObject->MarkForDestroy();
	}
}

void dae::MovingState::Init()
{
	m_Timer = static_cast<float>(MathLib::CalculateChance(10));
	if (gameObject) {
		std::string enemy{ EnemyType == EEnemyType::Fygar ? "Fygar" : "Pooka" };
		gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/" + enemy + "Left.png", 0.1f, 2);
		gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(false);
		gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(false);
		if (auto comp{ gameObject->GetComponent<EnemyComponent>() }) {
			comp->SetLifeState(MathLib::ELifeState::ALIVE);
		}
	}
}

void dae::MovingState::Update()
{
	m_Timer -= Timer::GetInstance().GetDeltaTime();
	if (m_Timer > 0) return;
	if (MathLib::CalculateChance() >= 80) {
		if (EnemyType == EEnemyType::Fygar) {
			if (MathLib::CalculateChance() >= 50) {
				//gameObject->GetComponent<EnemyComponent>()->SetState(new BreatheFireState());
				gameObject->GetComponent<EnemyComponent>()->SetState(new BreatheFireState());
			}
			else {
				gameObject->GetComponent<EnemyComponent>()->SetState(new GhostState());
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
	auto players{ m_Scene->GetGameObjects(EnumStrings[Names::PlayerGeneral], false) };
	int randNr{ MathLib::CalculateChance(static_cast<int>(players.size()) - 1) };

	auto id{ m_Scene->GetGameObject(EnumStrings[Names::PlayerGeneral] + std::to_string(randNr))->GetComponent<EntityMovementComponent>()->GetCurrentTileId() };
	m_CachedLocation  = m_Scene->GetGameObject(std::to_string(id))->GetCenter();
	gameObject->GetComponent<EntityMovementComponent>()->SetGhostModeEnabled(true);
	gameObject->GetComponent<EntityMovementComponent>()->SetGhostLocation(m_CachedLocation);
	gameObject->GetComponent<EnemyComponent>()->SetLifeState(MathLib::ELifeState::INVINCIBLE);
}

void dae::GhostState::Update()
{
	//if reached a dug tile near a player
	float dx = m_CachedLocation.x - gameObject->GetCenter().x;
	float dy = m_CachedLocation.y - gameObject->GetCenter().y;
	float distanceToTarget = std::sqrt(dx * dx + dy * dy);
	if (distanceToTarget <= 1) {
		gameObject->GetComponent<EnemyComponent>()->SetState(new MovingState());
	}
}

void dae::BreatheFireState::Init()
{
	gameObject->GetComponent<EntityMovementComponent>()->DisableMovement(true);
	gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/FygarPrepare.png", 0.2f, 3);	
	gameObject->GetComponent<EnemyComponent>()->SetLifeState(MathLib::ELifeState::ALIVE);

}

void dae::BreatheFireState::Update()
{
	m_PrepareTimer -= Timer::GetInstance().GetDeltaTime();
	if (!m_IsPrepareComplete) {
		if (m_PrepareTimer <= 0) {
			m_IsPrepareComplete = true;
			auto dir{ gameObject->GetComponent<EntityMovementComponent>()->GetLastDirection() };
			//if (gameObject->GetComponent<EntityMovementComponent>()->GetDirection() == MathLib::Movement::LEFT) {
				gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/Fygar"+ dir +".png", 0.2f, 2, true, false);
			//}
			//else {
			//	gameObject->GetComponent<TextureComponent>()->SetTexture("Enemies/FygarRight.png", 0.2f, 2, true, false);
			//}

			auto go{ std::make_shared<GameObject>() };
			m_Scene->Add(go);
			fireObject = go.get();
			go->AddComponent(std::make_unique<FireComponent>(m_Scene, gameObject));
		}
	}

	if (fireObject) {
		if (fireObject->GetComponent<FireComponent>()->GetIsFireFinished()) {
			fireObject->MarkForDestroy();
			gameObject->GetComponent<EnemyComponent>()->SetState(new MovingState());
		}
	}
}
