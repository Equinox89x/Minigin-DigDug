#pragma once
#include "Observer.h"
#include "vector"
#include "Event.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <functional>
//#include "Steam.h"
//#include "SteamHolder.h"

namespace dae
{
	class HealthObserver final : public Observer
	{
	public:
		HealthObserver(Scene* scene) : m_Scene{ scene } {};
		void Notify(GameObject* go, Event& event) override;

	private:
		Scene* m_Scene;
	};

	class ScoreObserver final : public Observer
	{
	public:

		ScoreObserver(Scene* const scene);
		~ScoreObserver();
		void Notify(GameObject* go, Event& event) override;

	private:

		Scene* m_pGO;

	};
	
	//class PumpEnemyOverlapObserver final : public Observer
	//{
	//public:

	//	PumpEnemyOverlapObserver(Scene* const scene) : m_Scene { scene } {};
	//	void Notify(GameObject* go, Event& event) override;

	//private:

	//	Scene* m_Scene;

	//};
}