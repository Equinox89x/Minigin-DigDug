#include "RotatorComponent.h"

dae::RotatorComponent::RotatorComponent()
{
}

void dae::RotatorComponent::Update(float deltaTime)
{
	if (GetGameObject()) {
		if (GetGameObject()->GetTransform()) {
			GetGameObject()->GetTransform()->Rotate(m_Angle);
			m_Angle += (deltaTime * m_Speed);
			if (m_Angle > 360) {
				m_Angle = 0;
			}
		}
	}
}
