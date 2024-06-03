#include "PathwayCreatorComponent.h"
#include "GameObject.h"
#include "EntityMovementComponent.h"
#include "Renderer.h"
#include "RockComponent.h"
#include <memory>

dae::PathwayCreatorComponent::~PathwayCreatorComponent()
{
	m_Pathways.clear();
}

void dae::PathwayCreatorComponent::AddPathway(int id, glm::vec2 pos, std::string type, glm::vec2 pos2, glm::vec2 size)
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


	auto middle{ go->GetTransform()->GetFullPosition() };
	middle.x += component->GetRect().w / 2;
	middle.y -= (component->GetRect().h / 2)- component->GetRect().h;
	PathWay pathWay{
		id,
		component,
		EPathState::Undug,
		SDL_Rect{ static_cast<int>(pos2.x), static_cast<int>((pos2.y - size.y / 2) + (size.y * 0.85)), static_cast<int>(size.x), static_cast<int>(size.y / 8) },
		SDL_Rect{ static_cast<int>((pos2.x + size.x / 2) - 2), static_cast<int>(pos2.y), static_cast<int>(size.x / 8), static_cast<int>(size.y) },
		middle,
	};


	if(type == "dug"){
		pathWay.PathState = EPathState::Dug;
	}
	else if (type == "spawn") {
		pathWay.PathState = EPathState::Spawn;
		m_Spawns.push_back(pathWay);
	}
	else if(type == "enemySpawn"){
		pathWay.PathState = EPathState::EnemySpawn;
		m_EnemySpawns.push_back(pathWay);
	}

	m_Pathways.insert({ id, pathWay });

	if (type == "dug" || type == "spawn" || type == "enemySpawn") {
		ActivatePathway(id);
	}


}
void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	m_Pathways[id].TextureComponent->SetIsVisible(true);
	m_Pathways[id].PathState = EPathState::Dug;
}

void dae::PathwayCreatorComponent::Update()
{
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::HandleEntityTileOverlap()
{
	auto& children{ GetGameObject()->GetChildren() };
	if (m_pCharacters.size() > 0) {

		for (const auto& character : m_pCharacters) {
			if (character->IsMarkedForDestroy()) continue;
			const auto& moveComp{ character->GetComponent<EntityMovementComponent>() };
			for (const auto& gameObj : children)
			{
				auto texComp{ gameObj->GetComponent<TextureComponent>() };
				if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCollider())) {
					moveComp->SetNextTileId(std::stoi(gameObj->GetName()));
				}
				if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCharacterCollider())) {
					moveComp->SetCurrentTileId(std::stoi(gameObj->GetName()));
				}
			}

			moveComp->SetShouldDig(m_Pathways[moveComp->GetNextTileId()].PathState == EPathState::Undug);
			ActivatePathway(moveComp->GetCurrentTileId());
		}
	}

	if (m_pEnemies.size() > 0) {
		for (const auto& character : m_pEnemies) {
			if (character->IsMarkedForDestroy()) continue;
			const auto& moveComp{ character->GetComponent<EntityMovementComponent>() };
			for (const auto& gameObj : children)
			{
				auto texComp{ gameObj->GetComponent<TextureComponent>() };
				if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCollider())) {
					moveComp->SetNextTileId(std::stoi(gameObj->GetName()));
				}
				if (MathLib::IsOverlapping(texComp->GetRect(), moveComp->GetCharacterCollider())) {
					moveComp->SetCurrentTileId(std::stoi(gameObj->GetName()));
				}
			}
		}
	}
	else {
		m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
		m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);
	}
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacters = m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false);
	m_pEnemies = m_pScene->GetGameObjects(EnumStrings[EnemyGeneral], false);
	HandleEntityTileOverlap();
}

void dae::PathwayCreatorComponent::Render() const
{
	//for (auto path : m_Pathways) {
	//	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200);
	//	auto rrect = SDL_Rect{ int(path.second.Middle.x), int(path.second.Middle.y), 1,1 };
	//	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &path.second.HorizontalWay);
	//	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &path.second.VerticalWay);
	//	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rrect);
	//}
	
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 200, 0, 200); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftMapBorder);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_RightMapBorder);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_TopMapBorder);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomMapBorder);
}
