#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include "Scene.h"
#include "MathLib.h"
#include <map>
#include "Scene.h"


namespace dae {
    class EntityMovementComponent final : public Component
    {

    public:
        EntityMovementComponent(Scene* scene, glm::vec2 startPos, bool isAutonomous = false) : m_Scene{ scene }, m_StartPos{ startPos }, m_IsAutonomous{ isAutonomous } { };
        ~EntityMovementComponent() = default;
        EntityMovementComponent(const EntityMovementComponent&) = delete;
        EntityMovementComponent(EntityMovementComponent&&) noexcept = delete;
        EntityMovementComponent& operator=(const EntityMovementComponent&) = delete;
        EntityMovementComponent& operator=(EntityMovementComponent&&) noexcept = delete;
        virtual void Update() override;
        virtual void Render() const override;
        virtual void Init() override;

        void CheckMovement(const std::vector<SDL_Rect>& horizontal, const std::vector<SDL_Rect>& vertical);
        void SetIsController(bool isController) { m_IsController = isController; };

        void SetStartPos(glm::vec2 startPos) { m_StartPos = startPos; };
        void Reposition() { GetGameObject()->GetComponent<TextureComponent>()->SetPosition(m_StartPos.x, m_StartPos.y); };

        void SetMovement(MathLib::Movement movement);

        MathLib::ELifeState GetState() { return m_PlayerState; };
        SDL_Rect GetCollider() { return m_PathwayColliderRect; };
        SDL_Rect GetCharacterCollider() { return m_Rect; };
        bool GetShouldDig() { return m_ShouldDig; };
        void SetShouldDig(bool shouldDig) { m_ShouldDig = shouldDig; };

        void SetNextTileId(int id) { m_NextTileId = id; };
        void SetCurrentTileId(int id) { m_CurrentTileId = id; };        
        int GetNextTileId() { return m_NextTileId; };
        int GetCurrentTileId() { return m_CurrentTileId; };

        SDL_Rect GetPathCollider(MathLib::Movement movement);

        void SetMapBorders(SDL_Rect LeftMapBorder, SDL_Rect RightMapBorder, SDL_Rect TopMapBorder, SDL_Rect BottomMapBorder) {
            m_LeftMapBorder = LeftMapBorder;
            m_RightMapBorder = RightMapBorder;
            m_TopMapBorder = TopMapBorder;
            m_BottomMapBorder = BottomMapBorder;
        };

        void DisableMovement(bool isDisabled) { m_CanMove = !isDisabled; };

        MathLib::Movement GetDirection() { return m_Movement; };

    private:
        bool m_IsController{ false }, m_ShouldDig{ false }, m_IsAutonomous{ false }, m_CanMove{ true };
        int m_NextTileId{ 0 }, m_CurrentTileId{ 0 };
        Scene* m_Scene{ nullptr };

        SDL_Rect m_BottomRect{}, m_TopRect{}, m_Rect{}, m_LeftRect{}, m_RightRect{}, m_PathwayColliderRect{};
        SDL_Rect m_BottomMoveRect{}, m_TopMoveRect{}, m_LeftMoveRect{}, m_RightMoveRect{};
            
        MathLib::ELifeState m_PlayerState{ MathLib::ELifeState::ALIVE };
        MathLib::Movement m_Movement{ MathLib::Movement::DOWN };
        MathLib::Movement m_PrevMovement{};
        glm::vec2 m_StartPos{};
        glm::vec2 m_PrevLoc{};

        //Autonomous
        float m_Speed{ 50 }, m_MoveTimer{ 2 };
        bool m_CanChangeState{ false };
        SDL_Rect m_LeftMapBorder{}, m_RightMapBorder{}, m_TopMapBorder{}, m_BottomMapBorder{};
        SDL_Rect m_LastHorLeft{}, m_LastVertTop{}, m_CurrentHorLeft{}, m_CurrentVertTop{}, m_LastHorRight{}, m_LastVertBottom{}, m_CurrentHorRight{}, m_CurrentVertBottom{};
        MathLib::EMovingState m_State{ MathLib::EMovingState::MovingLeft };
        std::map<MathLib::EMovingState, bool> m_MovementDir{
              std::make_pair(MathLib::EMovingState::MovingUp, true),
              std::make_pair(MathLib::EMovingState::MovingLeft, true),
              std::make_pair(MathLib::EMovingState::MovingRight, true),
              std::make_pair(MathLib::EMovingState::MovingDown, true),
        };
    };
}