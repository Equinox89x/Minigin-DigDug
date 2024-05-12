#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include "Scene.h"

namespace dae {

    enum class EEnemyType {
        Pooka,
        Fygar
    };

    class EnemyState {
    public:
        virtual ~EnemyState() = default;
        virtual void Update() = 0;
        virtual void Init() = 0;

        void SetData(dae::Scene* scene, GameObject* go, EEnemyType enemyType) {
            m_Scene = scene;
            gameObject = go;
            EnemyType = enemyType;
        }

    protected:
        Scene* m_Scene{ nullptr };
        GameObject* gameObject{ nullptr };
        MathLib::Movement m_Movement{ MathLib::Movement::DOWN };
        EEnemyType EnemyType{ EEnemyType::Pooka };


    };

    class MovingState : public EnemyState {
    public:
        virtual void Init() override;
        virtual void Update() override;
    };   
    
    class InflatingState : public EnemyState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    };
        
    class GhostState : public EnemyState {
    public:
        virtual void Init() override;
        virtual void Update() override;
    private:
        glm::vec2 m_CachedLocation{};
    };       

    class BreatheFireState : public EnemyState {
    public:
        virtual void Init() override;
        virtual void Update() override;
    private:
        float m_FireTimer{ 0.3f }, m_PrepareTimer{ 0.6f };
        GameObject* fireObject{ nullptr };
        bool m_IsPrepareComplete{ false };
    };

    class DeathState : public EnemyState {
    public:
        virtual void Init() override {};
        virtual void Update() override;
    private:
        float m_DeathTimer{ 0.3f };
    };


    //MovingState* EnemyState::moving = new MovingState();
    //InflatingState* EnemyState::pumping = new InflatingState();
    //GhostState* EnemyState::ghosting = new GhostState();
    //DeathState* EnemyState::death = new DeathState();


    class EnemyComponent final : public Component
    {
    public:
        EnemyComponent(Scene* scene, EEnemyType enemyType = EEnemyType::Pooka) : m_Scene{ scene }, EnemyType{ enemyType } { SetState(new MovingState); };
        ~EnemyComponent();
        EnemyComponent(const EnemyComponent&) = delete;
        EnemyComponent(EnemyComponent&&) noexcept = delete;
        EnemyComponent& operator=(const EnemyComponent&) = delete;
        EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;
        MathLib::ELifeState GetState() { return m_PlayerState; };

        bool PumpUp();

        void SetState(EnemyState* state) {
            delete m_State;
            m_State = state;
            m_State->SetData(m_Scene, GetGameObject(), EnemyType);
            m_State->Init();
        }
        
    private:
        Scene* m_Scene{ nullptr };
        int currentPumpStage{ 0 }, maxPumpStage{ 4 };
        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };

        EnemyState* m_State{ nullptr };
        EEnemyType EnemyType{ EEnemyType::Pooka };
    };
}
