#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include <map>
#include "Scene.h"


namespace dae {
    class PlayerComponent final : public Component
    {

    public:
        enum PlayerState {
            ALIVE, DEAD, RESPAWN
        };

        PlayerComponent(glm::vec2 startPos) : m_StartPos{ startPos } { };
        ~PlayerComponent() = default;
        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;

        void CheckMovement(/*const std::vector<std::pair<SDL_Rect, GameObject*>>& rects*/);
        void SetIsController(bool isController) { m_IsController = isController; };

        void SetStartPos(glm::vec2 startPos) { m_StartPos = startPos; };
        void Reposition() { GetGameObject()->GetComponent<TextureComponent>()->SetPosition(m_StartPos.x, m_StartPos.y); };

        void SetMovement(MathLib::Movement movement) { m_Movement = movement; };

        PlayerState GetState() { return m_PlayerState; };

    private:
        bool m_IsController{ false };

        SDL_Rect m_BottomRect{}, m_LeftRect{}, m_Rect{}, m_BottomLeft{}, m_BottomRight{};
        PlayerState m_PlayerState{ PlayerState::ALIVE };
        MathLib::Movement m_Movement{ MathLib::Movement::DOWN };
        MathLib::Movement m_PrevMovement{};
        glm::vec2 m_StartPos{};
        glm::vec2 m_PrevLoc{};
    };
}