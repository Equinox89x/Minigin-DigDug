#include "PathwayCreatorComponent.h"
#include "GameObject.h"
#include "EntityMovementComponent.h"
#include "Renderer.h"

dae::PathwayCreatorComponent::~PathwayCreatorComponent()
{
	m_Pathways.clear();
}

void dae::PathwayCreatorComponent::AddPathway(int id, glm::vec2 pos)
{
	auto go = std::make_shared<GameObject>();
	m_pScene->Add(go);
	GetGameObject()->AddChild(go.get());
	go->SetName(std::to_string(id));
	go->GetTransform()->Translate(pos);

	auto comp{ std::make_unique<TextureComponent>() };
	comp->SetName(std::to_string(id));
	comp->SetTexture("Levels/hole.png");
	auto component{ go->AddComponent(std::move(comp)) };
	component->SetIsVisible(false);
	component->Scale(0.5f, 0.4f);

	PathWay pathWay{
		id,
		component,
		EPathState::Undug,
	};
	m_Pathways.insert({ id, pathWay });
}

void dae::PathwayCreatorComponent::AddPassageway(glm::vec2 pos, glm::vec2 size)
{
	m_HorizontalPathways.push_back(SDL_Rect{ static_cast<int>(pos.x), static_cast<int>((pos.y - size.y / 2) + (size.y*0.85)), static_cast<int>(size.x), static_cast<int>(size.y/8) });
	m_VerticalPathways.push_back(SDL_Rect{ static_cast<int>((pos.x + size.x / 2) - 2), static_cast<int>(pos.y), static_cast<int>(size.x/8), static_cast<int>(size.y) });
}

void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	GetGameObject()->GetChild(std::to_string(id))->GetComponent<TextureComponent>()->Scale(0.5f, 0.4f);
}

void dae::PathwayCreatorComponent::Update()
{
	if (m_pCharacter) {
		const auto& playercomp{ m_pCharacter->GetComponent<EntityMovementComponent>() };

		for (const auto& gameObj : GetGameObject()->GetChildren())
		{
			auto texComp{ gameObj->GetComponent<TextureComponent>() };
			if (MathLib::IsOverlapping(texComp->GetRect(), playercomp->GetCollider())) {
				playercomp->SetNextTileId(std::stoi(gameObj->GetName()));
			}
			if (MathLib::IsOverlapping(texComp->GetRect(), playercomp->GetCharacterCollider())) {
				playercomp->SetCurrentTileId(std::stoi(gameObj->GetName()));
			}
		}

		playercomp->SetShouldDig(m_Pathways[playercomp->GetNextTileId()].PathState == EPathState::Undug);
		m_Pathways[playercomp->GetCurrentTileId()].TextureComponent->SetIsVisible(true);
		m_Pathways[playercomp->GetCurrentTileId()].PathState = EPathState::Dug;
	}
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacter = m_pScene->GetGameObject(EnumStrings[Player0]).get();
}

void dae::PathwayCreatorComponent::Render() const
{
	//for (auto path : m_HorizontalPathways) {
	//	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200); // Set the color to red
	//	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &path);
	//}
	//for (auto path : m_VerticalPathways) {
	//	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &path);
	//}	
	// 
	// 
	
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftMapBorder);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightMapBorder);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopMapBorder);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomMapBorder);
}
