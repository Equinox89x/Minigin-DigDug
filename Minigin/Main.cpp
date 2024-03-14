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

void MakePlayer(Scene* scene, std::string textureName, int id, glm::vec2 startPos) {
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
	tex->GetRect();

	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(startPos));
	mainPlayer->GetTransform()->Translate(startPos);

	if (id == 0) {
		//Keyboard
		mainPlayer->GetComponent<PlayerComponent>()->SetIsController(false);
		mainPlayer->AddComponent(std::make_unique<InputComponent>(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_w,					 id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::UP, "ESNstar.png", glm::vec3(0.f, -200.f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_s,					 id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::DOWN, "ESNstar.png", glm::vec3(0.f, 200.f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a,					 id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::LEFT, "ESNstar.png", glm::vec3(-200.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d,					 id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::RIGHT, "ESNstar.png", glm::vec3(200.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 SDLK_w,					 id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 SDLK_s,					 id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 SDLK_a,					 id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 SDLK_d,					 id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));

		////Controller
		//mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));


		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	}
	else {
		//Controller
		mainPlayer->GetComponent<PlayerComponent>()->SetIsController(true);
		mainPlayer->AddComponent(std::make_unique<InputComponent>(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp,    id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::UP, "ESNstar.png", glm::vec3(0, -200.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown,  id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::DOWN, "ESNstar.png", glm::vec3(0, 200.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::DpadLeft,  id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::LEFT, "ESNstar.png", glm::vec3(-200.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::DpadRight, id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::RIGHT, "ESNstar.png", glm::vec3(200.f, 0.0f, 0.0f)));


		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 ControllerButton::DpadUp,    id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 ControllerButton::DpadDown,  id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 ControllerButton::DpadLeft,  id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,		 ControllerButton::DpadRight, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
	}

}

void MakeMainChar(dae::Scene* scene, glm::vec2 startPos) {
	MakePlayer(scene, "ESNstar.png", 0, startPos);
}

void MakeSecondaryChar(Scene* scene, glm::vec2 startPos) {
	MakePlayer(scene, "ESNstar.png", 1, startPos);
}


void load()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// make bg
	auto go = std::make_shared<GameObject>();
	scene.Add(go);
	go->AddComponent(std::make_unique<TextureComponent>());
	go->GetComponent<TextureComponent>()->SetTexture("background.tga");

	// make text
	auto go3 = std::make_shared<GameObject>();
	scene.Add(go3);
	go3->AddComponent(std::make_unique<TextObjectComponent>("Programming 4 Assignment", font));
	go3->GetComponent<TextObjectComponent>()->SetPosition(80, 20);

	//make logo
	auto go2 = std::make_shared<GameObject>();
	scene.Add(go2);
	go2->GetTransform()->TranslateWorld(216, 180);
	go2->AddComponent(std::make_unique<TextureComponent>());
	go2->GetComponent<TextureComponent>()->SetTexture("logo.tga");


	//make fps counter
	go->AddComponent(std::make_unique<FPSCounterComponent>("2000", font));

	MakeMainChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });
	MakeSecondaryChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });

}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}