#include "EnemyComponent.h"

void dae::EnemyComponent::Update()
{
}

void dae::EnemyComponent::Render() const
{
}

void dae::EnemyComponent::Init()
{
}

bool dae::EnemyComponent::PumpUp()
{
	m_Lives--;
	if (m_Lives <= 0) {
		GetGameObject()->MarkForDestroy();
	}
	return m_Lives <= 0;
}
