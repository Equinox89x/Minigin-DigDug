#pragma once
#include "GameObject.h"
#include "InputComponent.h"
#include "TextureComponent.h"
#include "Command.h"

#pragma region Move Keyboard
class MoveKeyboard final : public Command
{
public:
	MoveKeyboard(GameObject* object, PlayerComponent::Movement movement, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_Movement{ movement }, m_MoveSpeed(moveSpeed), m_TextureName(textureName) {}
	void Execute() override
	{
		auto player{ m_pObject->GetComponent<PlayerComponent>() };
		if (player->GetState() == PlayerComponent::PlayerState::THROW || player->GetState() == PlayerComponent::PlayerState::DEAD) return;
		m_pObject->GetComponent<InputComponent>()->SetMoveSpeed(m_MoveSpeed);
		m_pObject->GetComponent<TextureComponent>()->SetTexture(m_TextureName);
		m_pObject->GetComponent<TextureComponent>()->SetNrOfFrames(3);
		player->SetMovement(m_Movement);
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
		if (player->GetState() == PlayerComponent::PlayerState::THROW || player->GetState() == PlayerComponent::PlayerState::DEAD) return;
		if (m_MoveSpeed.x != 0) {
			if (player->GetCanMoveHorizontally()) {
				m_pObject->GetComponent<InputComponent>()->SetMoveSpeed(m_MoveSpeed);
				m_pObject->GetComponent<TextureComponent>()->SetTexture(m_TextureName);
				m_pObject->GetComponent<TextureComponent>()->SetNrOfFrames(3);
				player->SetMovement(m_Movement);

			}
		}
		if (m_MoveSpeed.y != 0) {
			if (player->GetCanMoveVertically()) {
				m_pObject->GetComponent<InputComponent>()->SetMoveSpeed(m_MoveSpeed);
				m_pObject->GetComponent<TextureComponent>()->SetTexture(m_TextureName);
				m_pObject->GetComponent<TextureComponent>()->SetNrOfFrames(3);
				player->SetMovement(m_Movement);

			}
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
		m_pObject->GetComponent<InputComponent>()->SetMoveSpeed(glm::vec3(0, 0, 0));
	}
private:
	GameObject* m_pObject;
};
#pragma endregion
