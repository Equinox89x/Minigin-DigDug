#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"
#include "Command.h"

using namespace dae;

#pragma region Move Keyboard
class MoveKeyboard final : public Command
{
public:
	MoveKeyboard(GameObject* object, PlayerComponent::Movement movement, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName) {}
	void Execute() override
	{
		auto player{ m_pObject->GetComponent<PlayerComponent>() };
		if (player->GetState() == PlayerComponent::PlayerState::DEAD) return;

		auto input{ m_pObject->GetComponent<InputComponent>() };
		auto tex{ m_pObject->GetComponent<TextureComponent>() };

		player->SetMovement(m_Movement);
		input->SetMoveSpeed(m_MoveSpeed);

		tex->SetTexture(m_TextureName);
		tex->SetNrOfFrames(1);
	}
private:
	GameObject* m_pObject;
	glm::vec3 m_MoveSpeed;
	std::string m_TextureName;
	PlayerComponent::Movement m_Movement;
};

class StopMoveKeyboard final : public Command
{
public:
	StopMoveKeyboard(GameObject* object) : m_pObject(object) {};
	void Execute() override
	{
		m_pObject->GetComponent<InputComponent>()->SetMoveSpeed(glm::vec3(0, 0, 0));
	}
private:
	GameObject* m_pObject;
};
#pragma endregion

#pragma region Move Controller	
class MoveController final : public Command
{
public:
	MoveController(GameObject* object, PlayerComponent::Movement movement, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName) {}
	void Execute() override
	{
		auto player{ m_pObject->GetComponent<PlayerComponent>() };
		if (player->GetState() == PlayerComponent::PlayerState::DEAD) return;
		if (m_MoveSpeed.x != 0) {
			auto input{ m_pObject->GetComponent<InputComponent>() };
			auto tex{ m_pObject->GetComponent<TextureComponent>() };

			player->SetMovement(m_Movement);
			input->SetMoveSpeed(m_MoveSpeed);
			tex->SetTexture(m_TextureName);
			tex->SetNrOfFrames(1);
		}
		if (m_MoveSpeed.y != 0) {
			auto input{ m_pObject->GetComponent<InputComponent>() };
			auto tex{ m_pObject->GetComponent<TextureComponent>() };

			player->SetMovement(m_Movement);
			input->SetMoveSpeed(m_MoveSpeed);
			tex->SetTexture(m_TextureName);
			tex->SetNrOfFrames(1);
		}
	}
private:
	GameObject* m_pObject;
	glm::vec3 m_MoveSpeed;
	std::string m_TextureName;
	PlayerComponent::Movement m_Movement;
};

class StopMoveController final : public Command
{
public:
	StopMoveController(GameObject* object) : m_pObject(object) {};
	void Execute() override
	{
		auto input{ m_pObject->GetComponent<InputComponent>() };
		input->SetMoveSpeed(glm::vec3(0, 0, 0));
	}
private:
	GameObject* m_pObject;
};
#pragma endregion
