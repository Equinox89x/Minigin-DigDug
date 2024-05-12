#include "FireComponent.h"
#include "Timer.h"
#include "EnemyComponent.h"

void dae::FireComponent::Update()
{
	m_Timer -= Timer::GetInstance().GetDeltaTime();
	if (m_Timer <= 0.f) {
		m_FireFinished = true;
	}
	else if (m_Timer <= 0.2f) {
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame3.png", 0.1f, 1);
	}
	else if(m_Timer <= 0.4f){
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame2.png", 0.1f, 1);
	}
	else if(m_Timer <= 0.6f){
		GetGameObject()->GetComponent<TextureComponent>()->SetTexture("Enemies/Flame1.png", 0.1f, 1);
	}
}

void dae::FireComponent::Init()
{
	auto comp{ GetGameObject()->AddComponent(std::make_unique<TextureComponent>()) };
	if (comp) {
		GetGameObject()->GetTransform()->SetScale(glm::vec2{ 2.4f, 2.4f });
		GetGameObject()->GetTransform()->TranslateWorld(m_OriginLocation);
		comp->SetTexture("Enemies/Flame0.png", 0.1f, 1);
	}

}
