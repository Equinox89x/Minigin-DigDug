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
#include "PathwayCreatorComponent.h"
#include "ValuesComponent.h"
#include "Observers.h"
#include <steam_api.h>

using namespace dae;

void MakePlayer(Scene* scene, std::string textureName, int id, glm::vec2 startPos, bool isController) {
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };

	//Main Player
	auto mainPlayer = std::make_shared<GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//lives
	auto values{ mainPlayer->AddComponent(std::make_unique<ValuesComponent>(scene)) };
	auto observer{ std::make_shared<ScoreObserver>(scene) };
	values->AddObserver(observer);
	values->SetLives(3);
	auto observer2{ std::make_shared<HealthObserver>(scene) };
	values->AddObserver(observer2);

	//Texture
	mainPlayer->AddComponent(std::make_unique<TextureComponent>());
	auto tex{ mainPlayer->GetComponent<TextureComponent>() };
	tex->SetName(playerName);
	tex->SetTexture(textureName, 0.1f, 2);
	tex->Scale(2.4f, 2.4f);

	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(startPos));
	mainPlayer->GetTransform()->Translate(startPos);

	mainPlayer->GetComponent<PlayerComponent>()->SetIsController(isController);
	mainPlayer->AddComponent(std::make_unique<InputComponent>(mainPlayer->GetTransform()->GetPosition()));

	Input::InputKey up, down, left, right;
	Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
	Input::InputKey upScore, downLives;


	if (isController) {
		up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp,    id);
		down = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown, id);
		left = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadLeft, id);
		right = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadRight, id);		
		
		upCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadUp, id);
		downCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadDown, id);
		leftCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadLeft, id);
		rightCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadRight, id);

		upScore = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::ButtonA, id);
		downLives = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::ButtonB, id);
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

		upScore = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_i, id);
		downLives = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_o, id);
	}

	Input::GetInstance().BindKey(up, std::make_unique<Move>(mainPlayer.get(), MathLib::Movement::UP, "moveUp.png", glm::vec3(0.f, -200.f, 0.0f)));
	Input::GetInstance().BindKey(down, std::make_unique<Move>(mainPlayer.get(), MathLib::Movement::DOWN, "moveDown.png", glm::vec3(0.f, 200.f, 0.0f)));
	Input::GetInstance().BindKey(left, std::make_unique<Move>(mainPlayer.get(), MathLib::Movement::LEFT, "moveLeft.png", glm::vec3(-200.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey(right, std::make_unique<Move>(mainPlayer.get(), MathLib::Movement::RIGHT, "moveRight.png", glm::vec3(200.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey(upScore, std::make_unique<IncreaseScore>(mainPlayer.get()));
	Input::GetInstance().BindKey(downLives, std::make_unique<DownLives>(mainPlayer.get()));

	Input::GetInstance().BindKey(upCancel, std::make_unique<StopMove>(mainPlayer.get(), MathLib::Movement::UP));
	Input::GetInstance().BindKey(downCancel, std::make_unique<StopMove>(mainPlayer.get(), MathLib::Movement::DOWN));
	Input::GetInstance().BindKey(leftCancel, std::make_unique<StopMove>(mainPlayer.get(), MathLib::Movement::LEFT));
	Input::GetInstance().BindKey(rightCancel, std::make_unique<StopMove>(mainPlayer.get(), MathLib::Movement::RIGHT));
}

void MakeMainChar(dae::Scene* scene, glm::vec2 startPos, bool isController = false) {
	MakePlayer(scene, "moveRight.png", 0, startPos, isController);
}

void MakeSecondaryChar(Scene* scene, glm::vec2 startPos, bool isController = false) {
	MakePlayer(scene, "moveRight.png", 1, startPos, isController);
}

void MakeLevel(Scene* scene) {
	auto go1 = std::make_shared<GameObject>();
	go1->SetName(EnumStrings[Global]);
	scene->Add(go1);

	auto background{ go1->AddComponent(std::make_unique<TextureComponent>()) };
	background->SetTexture("Level1.png");
	background->Scale(5, 5);

	auto go = std::make_shared<GameObject>();
	scene->Add(go);
	go->SetName(EnumStrings[PathCreator]);
	auto pathCreator{ go->AddComponent(std::make_unique<PathwayCreatorComponent>(scene))};

	int index{ 0 };
	glm::vec2 pos{ 5, 45 };
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t i2 = 0; i2 < 14; i2++)
		{
			pathCreator->AddPathway(index, pos);
			//pathCreator->ActivatePathway(index);
			index++;
			pos.x += Cellsize;
		}
		pos.x = 5;
		pos.y += Cellsize - (Cellsize * 0.25f);
	}

}

void load()
{
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	// make bg
	auto go = std::make_shared<GameObject>();
	go->AddComponent(std::make_unique<FPSCounterComponent>("2000", font));
	scene.Add(go);

	MakeLevel(&scene);

	MakeMainChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 }, true);
	MakeSecondaryChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });

	auto scoreHolder = std::make_shared<GameObject>();
	scoreHolder->SetName(EnumStrings[Names::Score]);
	scoreHolder->AddComponent(std::make_unique<TextObjectComponent>("Score: " + std::to_string(0), font));
	scoreHolder->GetTransform()->AddTranslate(glm::vec3{ 0,60,0 });
	
	auto lifeHolder = std::make_shared<GameObject>();
	lifeHolder->SetName(EnumStrings[Life]);	
	lifeHolder->AddComponent(std::make_unique<TextObjectComponent>("Lives: " + std::to_string(3), font));
	lifeHolder->GetTransform()->AddTranslate(glm::vec3{ 0,90,0 });
	
	auto text1 = std::make_shared<GameObject>();
	text1->AddComponent(std::make_unique<TextObjectComponent>("Gamepad A (Player1) or Keyboard I  (Player2) for score increase", font2));

	auto text2 = std::make_shared<GameObject>();
	text2->AddComponent(std::make_unique<TextObjectComponent>("Gamepad B (Player1) or Keyboard O (Player2) for score increase", font2));
	text2->GetTransform()->AddTranslate(glm::vec3{ 0,30,0 });

	scene.Add(scoreHolder);
	scene.Add(lifeHolder);
	scene.Add(text1);
	scene.Add(text2);

}

int main(int, char*[]) {
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else {
		std::cout << "Successfully initialized steam." << std::endl;
	}

	dae::Minigin engine("../Data/");
	engine.Run(load);

	SteamAPI_Shutdown();
    return 0;
}