#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"
#include "MathLib.h"
#include "Command.h"

namespace dae {

	#pragma region Movement
	class Move final : public Command
	{
	public:
		Move(dae::GameObject* object, MathLib::Movement movement, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName) {}
		void Execute() override
		{
			auto player{ m_pObject->GetComponent<dae::PlayerComponent>() };
			if (player->GetState() == dae::PlayerComponent::PlayerState::DEAD) return;

			auto input{ m_pObject->GetComponent<dae::InputComponent>() };
			auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

			player->SetMovement(m_Movement);
			input->SetMoveSpeed(m_MoveSpeed, m_Movement);

			tex->SetTexture(m_TextureName);
		}

	private:
		dae::GameObject* m_pObject;
		glm::vec3 m_MoveSpeed;
		std::string m_TextureName;
		MathLib::Movement m_Movement;
	};

	class StopMove final : public Command
	{
	public:
		StopMove(dae::GameObject* object, MathLib::Movement direction) : m_pObject(object), m_Movement{ direction } {};
		void Execute() override
		{
			//m_pObject->GetComponent<dae::InputComponent>()->SetMoveSpeed(glm::vec3(0, 0, 0));
			m_pObject->GetComponent<dae::InputComponent>()->StopMovement(m_Movement);
		}
	private:
		dae::GameObject* m_pObject;
		MathLib::Movement m_Movement;
	};
	#pragma endregion

}