#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "TextureComponent.h"
#include "../DigDug/EntityMovementComponent.h"
#include "MathLib.h"
#include "Command.h"
#include "ValuesComponent.h"
#include "AudioComponent.h"
#include "../DigDug/PumpComponent.h"
#include "../DigDug/MenuComponent.h"

namespace dae {

	#pragma region Movement
	class Move final : public Command
	{
	public:
		Move(Scene* scene, dae::GameObject* object, MathLib::Movement movement, std::string textureName, const glm::vec3& moveSpeed) : 
			m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName), m_Scene{scene} {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;
			auto player{ m_pObject->GetComponent<dae::EntityMovementComponent>() };
			if (player->GetState() == MathLib::ELifeState::DEAD) return;

			auto input{ m_pObject->GetComponent<dae::InputComponent>() };
			auto tex{ m_pObject->GetComponent<dae::TextureComponent>() };

			player->SetMovement(m_Movement);
			input->SetMoveSpeed(m_MoveSpeed, m_Movement);
			tex->SetTexture(m_Movement, m_TextureName + (player->GetShouldDig() ? "Dig.png" : ".png"), 0.1f, 2);
		}

	private:
		Scene* m_Scene{ nullptr };

		dae::GameObject* m_pObject;
		glm::vec3 m_MoveSpeed;
		std::string m_TextureName;
		MathLib::Movement m_Movement;
	};

	class StopMove final : public Command
	{
	public:
		StopMove(Scene* scene, dae::GameObject* object, MathLib::Movement direction) : m_pObject(object), m_Movement{ direction }, m_Scene{ scene } {};
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<dae::InputComponent>()->StopMovement(m_Movement);
			m_pObject->GetComponent<dae::TextureComponent>()->RemoveTexture(m_Movement);
		}
	private:
		Scene* m_Scene{ nullptr };
		dae::GameObject* m_pObject;
		MathLib::Movement m_Movement;
	};
	#pragma endregion

	#pragma region values
	class IncreaseScore final : public Command
	{
	public:
		IncreaseScore(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<ValuesComponent>()->IncreaseScore(100);
		}
	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
	};


	class Pump final : public Command
	{
	public:
		Pump(Scene* scene, GameObject* const object, GameObject* const pumpObject) : m_pObject(object), m_pPumpObject{ pumpObject }, m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<dae::AudioComponent>()->PlayShootSound();
			m_pObject->GetComponent<dae::EntityMovementComponent>()->DisableMovement(true);
			m_pObject->GetComponent<dae::InputComponent>()->DisableMovement(true);
			m_pPumpObject->GetComponent<dae::PumpComponent>()->Pump(m_pObject->GetTransform()->GetWorldPosition());
		}
	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
		GameObject* m_pPumpObject;
	};
	#pragma endregion

#pragma region Menu
	class CycleGameMode final : public Command
	{
	public:
		CycleGameMode(Scene* scene, GameObject* object, bool isMoveUp) : m_pObject(object), m_IsMoveUp{ isMoveUp }, m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<MenuComponent>()->CycleGameMode(m_IsMoveUp);
		}
	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
		bool m_IsMoveUp{};
	};

	class StartGame final : public Command
	{
	public:
		StartGame(Scene* scene, GameObject* object) : m_pObject(object), m_Scene{ scene } {}
		void Execute() override
		{
			if (!m_Scene->GetIsActive()) return;

			m_pObject->GetComponent<MenuComponent>()->StartGame(m_pObject->GetParent());
		}
	private:
		Scene* m_Scene{ nullptr };

		GameObject* m_pObject;
	};

#pragma endregion

}