#include "PathwayCreatorComponent.h"
#include "GameObject.h"

dae::PathwayCreatorComponent::~PathwayCreatorComponent()
{
	for (auto key : m_Pathways)
	{
		delete key.second;
		key.second = nullptr;
	}

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
	comp->SetTexture("hole.png");
	auto component{ go->AddComponent(std::move(comp)) };
	component->Scale(0.f, 0.f);
}

void dae::PathwayCreatorComponent::ActivatePathway(int id)
{
	GetGameObject()->GetChild(std::to_string(id))->GetComponent<TextureComponent>()->Scale(0.5f, 0.4f);
}

void dae::PathwayCreatorComponent::Update()
{
	for (const auto obj : GetGameObject()->GetChildren())
	{
		if (m_pCharacter) {
			if (MathLib::IsOverlapping(obj->GetComponent<TextureComponent>()->GetRect(), m_pCharacter->GetComponent<TextureComponent>()->GetRect())) {
				obj->GetComponent<TextureComponent>()->Scale(0.5f, 0.4f);
			}
		}
	}
}

void dae::PathwayCreatorComponent::Init()
{
	m_pCharacter = m_pScene->GetGameObject(EnumStrings[Player1]).get();
}
