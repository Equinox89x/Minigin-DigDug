#include "Observers.h"
#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"
#include "PlayerComponent.h"
#include "Commands.h"
#include "TextObjectComponent.h"
#include "SteamHolder.h"

void dae::HealthObserver::Notify(GameObject* go, Event& event)
{
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	switch (event.GetEvent())
	{
	case EventType::Live:
		if (auto child{ m_Scene->GetGameObject(EnumStrings[Names::Life]) })
			child->GetComponent<TextObjectComponent>()->SetText("Lives: " + std::to_string(i));

		break;
	case EventType::Reset:
		break;
	}
}

dae::ScoreObserver::ScoreObserver(Scene* const scene) : m_pGO(scene)
{
	if (!g_SteamAchievements) { g_SteamAchievements = new CSteamAchievements(g_Achievements, 4); }
}

dae::ScoreObserver::~ScoreObserver()
{
	if (g_SteamAchievements) {
		delete g_SteamAchievements;
		g_SteamAchievements = nullptr;
	}
}

void dae::ScoreObserver::Notify(GameObject* go, Event& event)
{
	ValuesComponent* comp{ go->GetComponent<ValuesComponent>() };
	auto score{ comp->GetScores() };
	switch (event.GetEvent())
	{
	case EventType::Score:
		if (auto child{ m_pGO->GetGameObject(EnumStrings[Names::Score]) }) {
			child->GetComponent<TextObjectComponent>()->SetText("Score: " + std::to_string(score));
			if (score == 500) {
				g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
			}
		}

		break;
	case EventType::Reset:
		if (auto child{ m_pGO->GetGameObject(EnumStrings[Names::Score]) })
			child->GetComponent<TextObjectComponent>()->SetText("Score: " + std::to_string(score));
		break;

	}
}