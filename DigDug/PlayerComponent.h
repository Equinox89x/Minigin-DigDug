#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class PlayerComponent final : public Component
    {
    public:
        PlayerComponent(Scene* scene, glm::vec2 startPos) : m_Scene{ scene }, m_OriginalPosition{ startPos } { };
        ~PlayerComponent() = default;
        PlayerComponent(const PlayerComponent&) = delete;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent& operator=(const PlayerComponent&) = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;
        MathLib::ELifeState GetState() { return m_PlayerState; };
        void Respawn();

    private:
        Scene* m_Scene{ nullptr };

        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
        glm::vec2 m_OriginalPosition;
        float m_InvincibilityTimer{ 3 };
    };
}

