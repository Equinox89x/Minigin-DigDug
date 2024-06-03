#include "HighscoreComponent.h"
#include "TextObjectComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Commands.h"


void dae::HighscoreComponent::Init()
{
	Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
	Input::InputKey blow;
	MapControllerInput(0, upCancel, downCancel, leftCancel, rightCancel, blow);
	MakePlayerInput(upCancel, downCancel, leftCancel, rightCancel, blow);
	MapKeyboardInput(0, upCancel, downCancel, leftCancel, rightCancel, blow);
	MakePlayerInput(upCancel, downCancel, leftCancel, rightCancel, blow);

	m_KeyboardKeyRects.clear();
	auto font{ ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 14) };
	glm::vec2 pos{ Margin * 2, WindowSizeY / 2 };
	int k{ 0 };
	for (size_t i = 0; i < 3; i++)
	{
		auto nr{ i == 2 ? 6 : 10 };
		for (size_t j = 0; j < nr; j++)
		{
			auto go{ std::make_shared<GameObject>() };
			auto& key{ m_KeyboardKeys[k] };
			auto comp{ go->AddComponent(std::make_unique<TextObjectComponent>(key, font)) };
			comp->SetName(key);
			comp->SetPosition(pos.x, pos.y);
			m_KeyboardKeyRects.push_back(SDL_Rect{ static_cast<int>(pos.x), static_cast<int>(pos.y), 40,40 });
			pos.x += 40;
			k++;
			m_Scene->Add(go);
		}
		pos.y += 40;
		pos.x = Margin * 2;
	}

	auto go{ std::make_shared<GameObject>() };
	auto comp{ go->AddComponent(std::make_unique<TextObjectComponent>("Submit", font))};
	comp->SetName("Submit");
	comp->SetPosition(static_cast<float>(m_KeyboardKeyRects[m_KeyboardKeyRects.size()-1].x + 40), static_cast<float>(pos.y - 40));
	m_KeyboardKeyRects.push_back(SDL_Rect{ m_KeyboardKeyRects[m_KeyboardKeyRects.size() - 1].x + 40, static_cast<int>(pos.y) - 40, 160,40 });
	m_Scene->Add(go);
}

void dae::HighscoreComponent::MoveCursor(glm::vec2 key)
{
	auto ykey{ static_cast<int>(key.y) };
	auto xkey{ static_cast<int>(key.x) };
	if (ykey == 0) {
		if (m_SelectedKey + key.x < m_KeyboardKeys.size()+1 && xkey == 1) {
			m_SelectedKey += static_cast<int>(key.x);
		}
		if (m_SelectedKey - key.x > 1 && xkey == -1) {
			m_SelectedKey += static_cast<int>(key.x);
		}
	}
	else {
		if (m_SelectedKey + 10 < m_KeyboardKeys.size() && ykey == -1 ) {
			m_SelectedKey += 10;
		}
		if (m_SelectedKey - 10 > -1 && ykey == 1) {
			m_SelectedKey -= 10;
		}
	}

}

void dae::HighscoreComponent::Select()
{
	if (m_Name == "Enter Name") m_Name = "";
	if (m_SelectedKey > m_KeyboardKeys.size()-1) {
		//TODO send data$
		auto scene{ SceneManager::GetInstance().GetActiveScene() };
		scene->SetActive(false);
		m_CreateHighscoreScreen(m_Score, false);
		SceneManager::GetInstance().DeleteScene(scene);
	}
	else {
		m_Name += m_KeyboardKeys[m_SelectedKey];
		m_Scene->GetGameObject("EditableName")->GetComponent<TextObjectComponent>()->SetText(m_Name);
	}
}

void dae::HighscoreComponent::Render() const
{
	for (size_t i = 0; i < m_KeyboardKeyRects.size(); i++) {
		if (i == m_SelectedKey) {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
		}
		else {
			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
		}
		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &m_KeyboardKeyRects[i]);
	}
}

void dae::HighscoreComponent::MakePlayerInput(Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow) {

	Input::GetInstance().BindKey(blow, std::make_unique<SelectKey>(m_Scene, GetGameObject()));

	Input::GetInstance().BindKey(upCancel, std::make_unique<MoveKeyboardCursor>(m_Scene, GetGameObject(), glm::vec2{0,1}));
	Input::GetInstance().BindKey(downCancel, std::make_unique<MoveKeyboardCursor>(m_Scene, GetGameObject(), glm::vec2{0,-1}));
	Input::GetInstance().BindKey(leftCancel, std::make_unique<MoveKeyboardCursor>(m_Scene, GetGameObject(), glm::vec2{-1,0}));
	Input::GetInstance().BindKey(rightCancel, std::make_unique<MoveKeyboardCursor>(m_Scene, GetGameObject(), glm::vec2{1,0}));
}

void dae::HighscoreComponent::MapControllerInput(int id, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow){
	upCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp, id);
	downCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown, id);
	leftCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadLeft, id);
	rightCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadRight, id);

	blow = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonA, id);
}
void dae::HighscoreComponent::MapKeyboardInput(int id, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow) {
	upCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_UP, id);
	downCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_DOWN, id);
	leftCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_LEFT, id);
	rightCancel = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_RIGHT, id);

	blow = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_SPACE, id);
}