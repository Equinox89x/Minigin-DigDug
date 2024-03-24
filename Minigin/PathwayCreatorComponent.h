#pragma once 
#include "Component.h"
#include <map>
#include "TextureComponent.h"
#include "Scene.h"

namespace dae {


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

		void AddPathway(int id, glm::vec2 pos);
		void ActivatePathway(int id);
		virtual void Update() override;
		virtual void Init() override;

		private:
			Scene* m_pScene{ nullptr };
			GameObject* m_pCharacter{ nullptr };
			std::map<int, dae::TextureComponent*> m_Pathways{};
	};
}
