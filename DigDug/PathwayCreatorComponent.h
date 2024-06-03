#pragma once 
#include "Component.h"
#include <map>
#include "TextureComponent.h"
#include "Scene.h"

namespace dae {

	enum class EPathState {
		Dug, Undug, Spawn, EnemySpawn, Rock
	};

	struct PathWay {
		int id{ 0 };
		dae::TextureComponent* TextureComponent{ nullptr };
		EPathState PathState{ EPathState::Undug };
		const SDL_Rect HorizontalWay{};
		const SDL_Rect VerticalWay{};
		const glm::vec2 Middle{};

		void Clean() {
			delete TextureComponent;
			TextureComponent = nullptr;
		}
	};

	class PathwayCreatorComponent : public Component
	{
	public:
		PathwayCreatorComponent() {};
		PathwayCreatorComponent(Scene* scene) : m_pScene{ scene } {
		};
		virtual ~PathwayCreatorComponent() override;
		PathwayCreatorComponent(const PathwayCreatorComponent& other) = delete;
		PathwayCreatorComponent(PathwayCreatorComponent&& other) = delete;
		PathwayCreatorComponent& operator=(const PathwayCreatorComponent& other) = delete;
		PathwayCreatorComponent& operator=(PathwayCreatorComponent&& other) = delete;

		void AddPathway(int id, glm::vec2 pos, std::string type, glm::vec2 pos2, glm::vec2 size);
		void ActivatePathway(int id);
		virtual void Update() override;
		virtual void Init() override;
		virtual void Render() const override;

		std::vector<PathWay>& GetSpawns() { return m_Spawns; };
		std::vector<PathWay>& GetEnemySpawns() { return m_EnemySpawns; };

		const std::map<int, PathWay>& GetPathways() { return m_Pathways; };
		void SetMapBorders(SDL_Rect LeftMapBorder, SDL_Rect RightMapBorder, SDL_Rect TopMapBorder, SDL_Rect BottomMapBorder) {
			m_LeftMapBorder = LeftMapBorder;
			m_RightMapBorder = RightMapBorder;
			m_TopMapBorder = TopMapBorder;
			m_BottomMapBorder = BottomMapBorder;
		};

		void GetMapBorders(SDL_Rect& LeftMapBorder, SDL_Rect& RightMapBorder, SDL_Rect& TopMapBorder, SDL_Rect& BottomMapBorder) {
			LeftMapBorder = m_LeftMapBorder;
			RightMapBorder = m_RightMapBorder;
			TopMapBorder = m_TopMapBorder;
			BottomMapBorder = m_BottomMapBorder;
		}

		private:
			Scene* m_pScene{ nullptr };
			std::vector<std::shared_ptr<GameObject>> m_pCharacters{ nullptr };
			std::vector<std::shared_ptr<GameObject>> m_pEnemies{ nullptr };
			std::map<int, PathWay> m_Pathways{};
			std::vector<PathWay> m_Spawns{};
			std::vector<PathWay> m_EnemySpawns{};

			SDL_Rect m_LeftMapBorder{}, m_RightMapBorder{}, m_TopMapBorder{}, m_BottomMapBorder{};

			void HandleEntityTileOverlap();

	};
}
