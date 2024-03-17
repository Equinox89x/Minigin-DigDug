#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObjectComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "FPSCounterComponent.h"
#include "RotatorComponent.h"
#include "InputComponent.h"
#include <iostream>
#include "Commands.h"
#include "InputManager.h"
#include "PlayerComponent.h"

using namespace dae;

void MakePlayer(Scene* scene, std::string textureName, int id, glm::vec2 startPos, bool isController) {
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };

	//Main Player
	auto mainPlayer = std::make_shared<GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//Texture
	mainPlayer->AddComponent(std::make_unique<TextureComponent>());
	auto tex{ mainPlayer->GetComponent<TextureComponent>() };
	tex->SetName(playerName);
	tex->SetTexture(textureName);
	tex->Scale(0.3f, 0.3f);
	//tex->GetRect();

	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(startPos));
	mainPlayer->GetTransform()->Translate(startPos);

	mainPlayer->GetComponent<PlayerComponent>()->SetIsController(isController);
	mainPlayer->AddComponent(std::make_unique<InputComponent>(mainPlayer->GetTransform()->GetPosition()));

	Input::InputKey up, down, left, right;
	Input::InputKey upCancel, downCancel, leftCancel, rightCancel;

	if (isController) {
		up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp,    id);
		down = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown, id);
		left = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadLeft, id);
		right = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadRight, id);		
		
		upCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadUp, id);
		downCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadDown, id);
		leftCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadLeft, id);
		rightCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadRight, id);
	}
	else {
		up = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_w, id);
		down = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_s, id);
		left = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_a, id);
		right = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_d, id);		
		
		upCancel = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_w, id);
		downCancel = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_s, id);
		leftCancel = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_a, id);
		rightCancel = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_d, id);
	}

	Input::GetInstance().BindKey(up, std::make_unique<Move>(mainPlayer.get(), PlayerComponent::Movement::UP, "ESNstar.png", glm::vec3(0.f, -200.f, 0.0f)));
	Input::GetInstance().BindKey(down, std::make_unique<Move>(mainPlayer.get(), PlayerComponent::Movement::DOWN, "ESNstar.png", glm::vec3(0.f, 200.f, 0.0f)));
	Input::GetInstance().BindKey(left, std::make_unique<Move>(mainPlayer.get(), PlayerComponent::Movement::LEFT, "ESNstar.png", glm::vec3(-200.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey(right, std::make_unique<Move>(mainPlayer.get(), PlayerComponent::Movement::RIGHT, "ESNstar.png", glm::vec3(200.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey(upCancel, std::make_unique<StopMove>(mainPlayer.get()));
	Input::GetInstance().BindKey(downCancel, std::make_unique<StopMove>(mainPlayer.get()));
	Input::GetInstance().BindKey(leftCancel, std::make_unique<StopMove>(mainPlayer.get()));
	Input::GetInstance().BindKey(rightCancel, std::make_unique<StopMove>(mainPlayer.get()));
}

void MakeMainChar(dae::Scene* scene, glm::vec2 startPos, bool isController = false) {
	MakePlayer(scene, "ESNstar.png", 0, startPos, isController);
}

void MakeSecondaryChar(Scene* scene, glm::vec2 startPos, bool isController = false) {
	MakePlayer(scene, "ESNstar.png", 1, startPos, isController);
}


void load()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// make bg
	auto go = std::make_shared<GameObject>();
	scene.Add(go);
	go->AddComponent(std::make_unique<TextureComponent>())->SetTexture("background.tga");
	//go->GetComponent<TextureComponent>()

	// make text
	auto go3 = std::make_shared<GameObject>();
	scene.Add(go3);
	go3->AddComponent(std::make_unique<TextObjectComponent>("Programming 4 Assignment", font))->SetPosition(80, 20);
	//go3->GetComponent<TextObjectComponent>()

	//make logo
	auto go2 = std::make_shared<GameObject>();
	scene.Add(go2);
	go2->GetTransform()->TranslateWorld(216, 180);
	go2->AddComponent(std::make_unique<TextureComponent>())->SetTexture("logo.tga");
	//go2->GetComponent<TextureComponent>()->SetTexture("logo.tga");


	//make fps counter
	go->AddComponent(std::make_unique<FPSCounterComponent>("2000", font));

	MakeMainChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 }, true);
	MakeSecondaryChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}