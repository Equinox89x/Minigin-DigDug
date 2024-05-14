#include "MenuComponent.h"

void dae::MenuComponent::CycleGameMode(bool isMoveUp)
{
	if (!isMoveUp) {
		if (m_SelectedGameMode != MathLib::GameMode::VERSUS) {
			m_SelectedGameMode = MathLib::GameMode(static_cast<int>(m_SelectedGameMode) + 1);
		}
	}
	else {
		if (m_SelectedGameMode != MathLib::GameMode::SOLO) {
			m_SelectedGameMode = MathLib::GameMode(static_cast<int>(m_SelectedGameMode) - 1);
		}
	}

	GetGameObject()->GetTransform()->TranslateWorld(m_CursorLocations[static_cast<int>(m_SelectedGameMode)]);

}

void dae::MenuComponent::StartGame(GameObject* /*pMenu*/)
{
	//pMenu->SetIsHidden(true);
	//switch (m_SelectedGameMode)
	//{
	//case MathLib::GameMode::SOLO:
	SceneManager::GetInstance().GetScene(EnumStrings[MainMenu])->SetActive(false);
	CreateLevel(m_SelectedGameMode, 0);
	//	break;
	//case MathLib::GameMode::COOP:
	//	CreateLevel(m_pScene);
	//	break;
	//case MathLib::GameMode::VERSUS:
	//	CreateLevel(m_pScene);
	//	break;
	//default:
	//	break;
	//}
}
