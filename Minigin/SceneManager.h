#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		std::shared_ptr<dae::Scene> GetScene(int index);
		std::shared_ptr<dae::Scene> GetScene(std::string name);

		void Init();
		void Update( );
		void LateUpdate();
		void Render();
		void Cleanup();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
	};
}
