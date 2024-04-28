#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {
    class EnemyComponent final : public Component
    {
    public:
        EnemyComponent(Scene* scene) : m_Scene{ scene }{ };
        ~EnemyComponent() = default;
        EnemyComponent(const EnemyComponent&) = delete;
        EnemyComponent(EnemyComponent&&) noexcept = delete;
        EnemyComponent& operator=(const EnemyComponent&) = delete;
        EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;
        MathLib::ELifeState GetState() { return m_PlayerState; };

        bool PumpUp();
        
    private:
        Scene* m_Scene{ nullptr };
        int m_Lives{ 5 };
        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
    };
}
