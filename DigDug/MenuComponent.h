#pragma once
#include "Component.h"
#include "TextureComponent.h"
#include <Scene.h>
#include <MathLib.h>
#include <functional>

namespace dae {
    class MenuComponent final : public Component
    {
    public:
        MenuComponent(Scene* scene, std::function<void(MathLib::GameMode gm, int levelIndex)> createLevel, std::vector<glm::vec2> cursorLocations) : 
            m_Scene{ scene }, CreateLevel{ createLevel }, m_CursorLocations{ cursorLocations } {  };
        ~MenuComponent() = default;

        MenuComponent(const MenuComponent&) = delete;
        MenuComponent(MenuComponent&&) noexcept = delete;
        MenuComponent& operator=(const MenuComponent&) = delete;
        MenuComponent& operator=(MenuComponent&&) noexcept = delete;

        void CycleGameMode(bool isMoveUp);

        void StartGame(GameObject* pMenu);

    private:
        Scene* m_Scene{ nullptr };


        MathLib::GameMode m_SelectedGameMode{ MathLib::GameMode::SOLO };
        bool m_IsMainMenu{ true };
        bool m_IsMenu{ false };
        std::vector<glm::vec2> m_CursorLocations{};

        std::function<void(MathLib::GameMode gm, int levelIndex)> CreateLevel;
    };
}
