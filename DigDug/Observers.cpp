#include "Observers.h"
#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"
#include "EntityMovementComponent.h"
#include "Commands.h"
#include "TextObjectComponent.h"
#include "SteamHolder.h"
#include "RockComponent.h"

void dae::HealthObserver::Notify(GameObject* go, Event& event)
{
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	switch (event.GetEvent())
	{
	case EventType::Live:
		if (auto child{ m_Scene->GetGameObject(EnumStrings[Names::Life]) })
			child->GetComponent<TextObjectComponent>()->SetText(child->GetName() + " Lives: " + std::to_string(i));

		break;
	case EventType::Reset:
		break;
	}
}

dae::ScoreObserver::~ScoreObserver()
{
	//if (g_SteamAchievements) {
	//	delete g_SteamAchievements;
	//	g_SteamAchievements = nullptr;
	//}
}

void dae::ScoreObserver::Notify(GameObject* player, Event& event)
{
	ValuesComponent* comp{ player->GetComponent<ValuesComponent>() };
	auto score{ comp->GetScores() };
	switch (event.GetEvent())
	{
	case EventType::Score:
		if (auto scoreHolder{m_Scene->GetGameObject(EnumStrings[Names::ScoreGeneral]+std::to_string(m_Id))}) {
			scoreHolder->GetComponent<TextObjectComponent>()->SetText(player->GetName() + " Score: " + std::to_string(score));
			scoreHolder->GetComponent<TextObjectComponent>()->SetName(std::to_string(score));
			//if (score == 500) {
			//	g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			//}
		}
		break;
	case EventType::Reset:
		if (auto scoreHolder{ m_Scene->GetGameObject(EnumStrings[Names::ScoreGeneral] + std::to_string(m_Id)) }) {
			scoreHolder->GetComponent<TextObjectComponent>()->SetText(player->GetName() + " Score: " + std::to_string(score));
			scoreHolder->GetComponent<TextObjectComponent>()->SetName(std::to_string(score));

		}
		break;

	}
}

//void dae::PumpEnemyOverlapObserver::Notify(GameObject* go, Event& event)
//{
//	auto enemies{ m_Scene->GetGameObject(EnumStrings[Names::EnemyHolder])->GetChildren() };
//
//}

void dae::EnemyDeathObserver::Notify(GameObject* /*go*/, Event& event)
{
	switch (event.GetEvent())
	{
	case EventType::EnemyDeath:
		auto enemies{ m_Scene->GetGameObjects(EnumStrings[Names::EnemyGeneral], false) };
		if (enemies.size() <= 1) {
			m_Scene->GetGameObject(EnumStrings[Names::Global])->GetComponent<MenuComponent>()->SkipLevel();
		}
		break;
	}
}

void dae::RockDeathObserver::Notify(GameObject* go, Event& event)
{
	auto comp{ go->GetComponent<RockComponent>() };
	switch (event.GetEvent())
	{
	case EventType::RockDeath:
		auto player{ m_Scene->GetGameObject(EnumStrings[Names::PlayerGeneral] + std::to_string(comp->GetPlayerId())) };
		ValuesComponent* valueComp{ player->GetComponent<ValuesComponent>() };
		valueComp->IncreaseScore(comp->GetScore());
		break;
	}
}
