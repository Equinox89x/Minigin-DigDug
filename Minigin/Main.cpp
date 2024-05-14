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
#include "../DigDug/Commands.h"
#include "InputManager.h"
#include "../DigDug/EntityMovementComponent.h"
#include "../DigDug/PathwayCreatorComponent.h"
#include "ValuesComponent.h"
#include "../DigDug/Observers.h"
//#include <steam_api.h>
#include "../DigDug/EnemyComponent.h"
#include "../DigDug/PlayerComponent.h"
#include "../DigDug/PumpComponent.h"
#include <AudioComponent.h>
#include "../DigDug/MenuComponent.h"

using namespace dae;

SDL_Rect leftMapBorder;
SDL_Rect topMapBorder;
SDL_Rect rightMapBorder;
SDL_Rect bottomMapBorder;

#pragma region Making objects
void MakePlayer(Scene* scene, std::string textureName, int id, glm::vec2 startPos, bool isController) {

	//Main Player
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };
	auto mainPlayer = std::make_shared<GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//pump
	auto pumpName{ EnumStrings[PumpGeneral] + std::to_string(id) };
	auto pump = std::make_shared<GameObject>();
	scene->Add(pump);
	pump->SetName(pumpName);
	pump->AddComponent(std::make_unique<PumpComponent>(scene));
	pump->AddComponent(std::make_unique<TextureComponent>());
	auto texPunp{ pump->GetComponent<TextureComponent>() };
	texPunp->SetTexture("Character/Pump.png", 0.1f, 1);
	texPunp->Scale(1.5f, 1.5f);
	texPunp->SetIsVisible(false);

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

	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(scene));
	mainPlayer->AddComponent(std::make_unique<AudioComponent>());
	mainPlayer->AddComponent(std::make_unique<EntityMovementComponent>(scene, startPos))->SetMapBorders(leftMapBorder, topMapBorder, rightMapBorder, bottomMapBorder);
	mainPlayer->GetTransform()->Translate(startPos);

	mainPlayer->GetComponent<EntityMovementComponent>()->SetIsController(isController);
	mainPlayer->AddComponent(std::make_unique<InputComponent>(scene, mainPlayer->GetTransform()->GetPosition()));

	Input::InputKey up, down, left, right;
	Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
	Input::InputKey blow;


	if (isController) {
		up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp,    id);
		down = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown, id);
		left = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadLeft, id);
		right = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadRight, id);		
		
		upCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadUp, id);
		downCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadDown, id);
		leftCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadLeft, id);
		rightCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadRight, id);

		blow = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::ButtonA, id);
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

		blow = std::make_tuple(ButtonStates::BUTTON_UP, SDLK_SPACE, id);
	}

	Input::GetInstance().BindKey(up, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::UP, "Character/moveUp", glm::vec3(0.f, -200.f, 0.0f)));
	Input::GetInstance().BindKey(down, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::DOWN, "Character/moveDown", glm::vec3(0.f, 200.f, 0.0f)));
	Input::GetInstance().BindKey(left, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::LEFT, "Character/moveLeft", glm::vec3(-200.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey(right, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::RIGHT, "Character/moveRight", glm::vec3(200.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey(blow, std::make_unique<Pump>(scene, mainPlayer.get(), pump.get()));

	Input::GetInstance().BindKey(upCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::UP));
	Input::GetInstance().BindKey(downCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::DOWN));
	Input::GetInstance().BindKey(leftCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::LEFT));
	Input::GetInstance().BindKey(rightCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::RIGHT));
}

void MakeMainChar(dae::Scene* scene, glm::vec2 startPos, bool isController = false) {
	MakePlayer(scene, "Character/moveRight.png", 0, startPos, isController);
}

void MakeSecondaryChar(Scene* scene, glm::vec2 startPos, bool isController = false) {
	MakePlayer(scene, "moveRight.png", 1, startPos, isController);
}

void MakeEnemy(Scene* scene, std::string textureName, int id, glm::vec2 startPos, dae::EEnemyType enemyType) {
	auto enemyHolder = scene->GetGameObject(EnumStrings[EnemyHolder]);
	if (!enemyHolder) {
		enemyHolder = std::make_shared<GameObject>();
		scene->Add(enemyHolder);
	}
	auto m_EnemyName{ EnumStrings[EnemyGeneral] + std::to_string(id) };

	//Enemy
	auto enemy = std::make_shared<GameObject>();
	scene->Add(enemy);
	enemy->SetName(m_EnemyName);
	//enemyHolder->AddChild(enemy.get());

	////lives
	//auto values{ enemy->AddComponent(std::make_unique<ValuesComponent>(scene)) };
	//auto observer{ std::make_shared<ScoreObserver>(scene) };
	//values->AddObserver(observer);
	//values->SetLives(3);
	//auto observer2{ std::make_shared<HealthObserver>(scene) };
	//values->AddObserver(observer2);

	//Texture
	enemy->AddComponent(std::make_unique<TextureComponent>());
	enemy->AddComponent(std::make_unique<AudioComponent>());

	auto tex{ enemy->GetComponent<TextureComponent>() };
	tex->SetName(m_EnemyName);
	tex->SetTexture(textureName, 0.1f, 2);
	tex->Scale(2.4f, 2.4f);

	auto comp{ enemy->AddComponent(std::make_unique<EntityMovementComponent>(scene, startPos, true)) };
	comp->SetMapBorders(leftMapBorder, topMapBorder, rightMapBorder, bottomMapBorder);
	comp->SetEnemyName(enemyType == EEnemyType::Pooka ? "Pooka" : "Fygar");
	enemy->AddComponent(std::make_unique<EnemyComponent>(scene, enemyType));
	enemy->GetTransform()->Translate(startPos);
}

void MakePookaEnemy(Scene* scene, glm::vec2 startPos) {
	MakeEnemy(scene, "Enemies/PookaLeft.png", 0, startPos, dae::EEnemyType::Pooka);
}

void MakeDragonEnemy(Scene* scene, glm::vec2 startPos) {
	MakeEnemy(scene, "Enemies/FygarLeft.png", 0, startPos, dae::EEnemyType::Fygar);
}

void MakeLevel(Scene* scene) {
	auto backgroundGameObject = std::make_shared<GameObject>();
	backgroundGameObject->SetName(EnumStrings[Global]);
	scene->Add(backgroundGameObject);

	auto background{ backgroundGameObject->AddComponent(std::make_unique<TextureComponent>()) };
	background->SetTexture("Levels/Level1.png");
	background->Scale(5, 5);

	auto pathObject = std::make_shared<GameObject>();
	scene->Add(pathObject);
	pathObject->SetName(EnumStrings[PathCreator]);
	auto pathCreator{ pathObject->AddComponent(std::make_unique<PathwayCreatorComponent>(scene))};

	int index{ 0 };
	int startX{ 5 }, startY{ 45 }, modifier{ 3 }, height{ 10 }, width{ 14 }, size{10};
	glm::vec2 pos{ startX, startY };

	leftMapBorder = SDL_Rect{ static_cast<int>(pos.x), static_cast<int>(pos.y * modifier), size, WindowSizeY - startY };
	topMapBorder = SDL_Rect{ static_cast<int>(pos.x), static_cast<int>((pos.y * modifier) - size), WindowSizeX - startX, size };
	for (int i = 0; i < height; i++)
	{
		for (int i2 = 0; i2 < width; i2++)
		{
			pathCreator->AddPathway(index, pos);
			pathCreator->AddPassageway({ pos.x * modifier, pos.y * modifier }, { Cellsize * modifier , Cellsize * modifier });
			index++;
			pos.x += Cellsize;
		}
		pos.x = static_cast<float>(startX);
		pos.y += Cellsize - (Cellsize * 0.25f);
	}
	int posX{ static_cast<int>(startX + ((Cellsize * width) * modifier)) + size };
	int posY{ static_cast<int>(startY + (((Cellsize * height) * modifier)) - size) };
	rightMapBorder = SDL_Rect{ posX, startY, size, WindowSizeY - startY };
	bottomMapBorder = SDL_Rect{ static_cast<int>(pos.x), posY, WindowSizeX - startX, size };
	pathCreator->SetMapBorders(leftMapBorder, rightMapBorder, topMapBorder, bottomMapBorder);
}
#pragma endregion

#pragma region Making a level scene
void CreateSoloLevel(MathLib::GameMode gm, int levelIndex, std::string sceneName) {
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& scene = SceneManager::GetInstance().CreateScene(gm == MathLib::GameMode::SOLO ? sceneName + std::to_string(levelIndex) : sceneName);

	//// make bg
	//auto go = std::make_shared<GameObject>();
	//go->AddComponent(std::make_unique<FPSCounterComponent>("2000", font));
	//scene.Add(go);

	MakeLevel(&scene);

	MakeMainChar(&scene, glm::vec2{ WindowSizeX / 2 + (Cellsize / 2), WindowSizeY / 2 - (Cellsize / 2) });
	//MakeSecondaryChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });

	MakePookaEnemy(&scene, glm::vec2{ WindowSizeX / 2 + (Cellsize / 2), WindowSizeY / 2 - (Cellsize / 2) });

	MakeDragonEnemy(&scene, glm::vec2{ WindowSizeX / 2 - (Cellsize * 4), WindowSizeY / 2 - (Cellsize / 2) });

	auto scoreHolder = std::make_shared<GameObject>();
	scoreHolder->SetName(EnumStrings[Names::Score]);
	scoreHolder->AddComponent(std::make_unique<TextObjectComponent>("Score: " + std::to_string(0), font));
	scoreHolder->GetTransform()->AddTranslate(glm::vec3{ 0,60,0 });

	auto lifeHolder = std::make_shared<GameObject>();
	lifeHolder->SetName(EnumStrings[Life]);
	lifeHolder->AddComponent(std::make_unique<TextObjectComponent>("Lives: " + std::to_string(3), font));
	lifeHolder->GetTransform()->AddTranslate(glm::vec3{ 0,90,0 });

	auto text1 = std::make_shared<GameObject>();
	text1->AddComponent(std::make_unique<TextObjectComponent>("Spacebar for sound", font2));

	auto text2 = std::make_shared<GameObject>();
	text2->AddComponent(std::make_unique<TextObjectComponent>("Spacebar for sound", font2));
	text2->GetTransform()->AddTranslate(glm::vec3{ 0,30,0 });

	scene.Add(scoreHolder);
	scene.Add(lifeHolder);
	scene.Add(text1);
	scene.Add(text2);

	scene.SetActive(true);
}
#pragma endregion

void CreateLevel(MathLib::GameMode gm, int levelIndex) {

	switch (gm)
	{
	case MathLib::GameMode::SOLO:
		CreateSoloLevel(gm, levelIndex, EnumStrings[SoloLevelNameGeneral]);
		break;
	case MathLib::GameMode::COOP:
		CreateSoloLevel(gm, levelIndex, EnumStrings[CoopLevelName]);
		break;
	case MathLib::GameMode::VERSUS:
		CreateSoloLevel(gm, levelIndex, EnumStrings[VersusLevelName]);
		break;
	default:
		break;
	}
}

void CreateSelectorInput(dae::Scene* scene) {
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);

	//game mode selection
	std::shared_ptr<GameObject> player1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player3 = std::make_shared<dae::GameObject>();
	scene->Add(player1);
	scene->Add(player2);
	scene->Add(player3);
	//container->AddChild(player1.get());
	//container->AddChild(player2.get());
	//container->AddChild(player3.get());
	player1->AddComponent(std::make_unique<TextObjectComponent>("SOLO", font));
	player2->AddComponent(std::make_unique<TextObjectComponent>("COOP", font));
	player3->AddComponent(std::make_unique<TextObjectComponent>("VERSUS", font));
	player1->GetTransform()->TranslateWorld(WindowSizeX / 2 - Margin * 3, 150);
	player2->GetTransform()->TranslateWorld(WindowSizeX / 2 - Margin * 3, 150 + Margin);
	player3->GetTransform()->TranslateWorld(WindowSizeX / 2 - Margin * 3, 150 + SubMargin);

	//game mode selector
	std::shared_ptr<GameObject> selector = std::make_shared<dae::GameObject>();
	selector->SetName(EnumStrings[Selector]);
	scene->Add(selector);
	//container->AddChild(selector.get());

	std::vector<glm::vec2> positions{
		player1->GetTransform()->GetFullPosition(),
		player2->GetTransform()->GetFullPosition(),
		player3->GetTransform()->GetFullPosition(),
	};
	positions[0].x -= 20;
	positions[1].x -= 20;
	positions[2].x -= 20;
	selector->AddComponent(std::make_unique<MenuComponent>(scene, &CreateLevel, positions));
	selector->AddComponent(std::make_unique<TextObjectComponent>(">", font));
	selector->GetTransform()->TranslateWorld(positions[0]);

	Input::InputKey up, down, space;

	//if (false) {
	//	up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp, 0);
	//	down = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown, 0);

	//	space = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::ButtonA, 0);
	//}
	//else {
		up = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_w, 0);
		down = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_s, 0);

		space = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_SPACE, 0);
	//}

	Input::GetInstance().BindKey(up, std::make_unique<CycleGameMode>(scene, selector.get(), true));
	Input::GetInstance().BindKey(down, std::make_unique<CycleGameMode>(scene, selector.get(), false));
	Input::GetInstance().BindKey(space, std::make_unique<StartGame>(scene, selector.get()));
}

void MakeMenu() {
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
	auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	auto& scene = SceneManager::GetInstance().CreateScene(EnumStrings[MainMenu]);
	//std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	//container->SetName(EnumStrings[MainMenu]);
	//scene.Add(container);


	//Title
	std::shared_ptr<GameObject> upText1 = std::make_shared<dae::GameObject>();
	scene.Add(upText1);
	//container->AddChild(upText1.get());
	upText1->AddComponent(std::make_unique<TextObjectComponent>("DIG DUG", font));
	upText1->GetTransform()->TranslateWorld(WindowSizeX / 4, Margin);
	upText1->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	CreateSelectorInput(&scene);

	//auto go{ std::make_shared<GameObject>() };
	//go->SetName(EnumStrings[Global]);
	//go->AddComponent(std::make_unique<AudioComponent>());
	//go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	//go->AddComponent(std::make_unique<MoveKeyboardComponent>(go->GetTransform()->GetPosition()));
	//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_F1, 0 }, std::make_unique<Skip>(&MakeStageOfNr, &MakeMrPepper, scene));

	/*FileReader* file{ new FileReader("../Data/highscore.txt") };
	auto str{ file->ReadGameDataFile() };
	auto data{ file->ParseDataSimple(str, '+') };

	float yPos{ WindowSizeY / 2 };
	int i{ 1 };
	for (auto field : data)
	{
		std::shared_ptr<GameObject> scoreObj = std::make_shared<dae::GameObject>();
		std::shared_ptr<GameObject> scoreObj2 = std::make_shared<dae::GameObject>();
		std::string score{ std::to_string(i) + "  " + field.first };
		std::string score2{ std::any_cast<std::string>(field.second) + " PTS" };
		scoreObj->AddComponent(std::make_unique<TextObjectComponent>(score, font));
		scoreObj2->AddComponent(std::make_unique<TextObjectComponent>(score2, font));
		scoreObj->GetComponent<TextObjectComponent>()->SetPosition(Margin * 2, yPos);
		scoreObj2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 + Margin, yPos);
		scene->Add(scoreObj);
		scene->Add(scoreObj2);

		container->AddChild(scoreObj.get());
		container->AddChild(scoreObj2.get());

		i++;
		yPos += 50;
	}*/

	//scene.Add(go);
}

void load()
{
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto font2 = ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);
	//auto& scene = SceneManager::GetInstance().CreateScene("DigDug");

	MakeMenu();
	//CreateLevel(MathLib::GameMode::SOLO, 0);
	SceneManager::GetInstance().GetScene(EnumStrings[MainMenu])->SetActive(true);
	//CreateLevel(MathLib::GameMode::SOLO, 1, "DigDugSolo");
	//CreateLevel(MathLib::GameMode::SOLO, 2, "DigDugSolo");
	//CreateLevel(MathLib::GameMode::COOP, 0, "DigDugCoop");
	//CreateLevel(MathLib::GameMode::VERSUS, 0, "DigDugVersus");


	//// make bg
	//auto go = std::make_shared<GameObject>();
	//go->AddComponent(std::make_unique<FPSCounterComponent>("2000", font));
	//scene.Add(go);

	//MakeLevel(&scene);

	//MakeMainChar(&scene, glm::vec2{ WindowSizeX / 2+(Cellsize/2), WindowSizeY / 2 - (Cellsize / 2)});
	////MakeSecondaryChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });

	//MakePookaEnemy(&scene, glm::vec2{ WindowSizeX / 2 + (Cellsize / 2), WindowSizeY / 2 - (Cellsize / 2) });

	//MakeDragonEnemy(&scene, glm::vec2{ WindowSizeX / 2 - (Cellsize*4), WindowSizeY / 2 - (Cellsize / 2) });

	//auto scoreHolder = std::make_shared<GameObject>();
	//scoreHolder->SetName(EnumStrings[Names::Score]);
	//scoreHolder->AddComponent(std::make_unique<TextObjectComponent>("Score: " + std::to_string(0), font));
	//scoreHolder->GetTransform()->AddTranslate(glm::vec3{ 0,60,0 });
	//
	//auto lifeHolder = std::make_shared<GameObject>();
	//lifeHolder->SetName(EnumStrings[Life]);	
	//lifeHolder->AddComponent(std::make_unique<TextObjectComponent>("Lives: " + std::to_string(3), font));
	//lifeHolder->GetTransform()->AddTranslate(glm::vec3{ 0,90,0 });
	//
	//auto text1 = std::make_shared<GameObject>();
	//text1->AddComponent(std::make_unique<TextObjectComponent>("Spacebar for sound", font2));

	//auto text2 = std::make_shared<GameObject>();
	//text2->AddComponent(std::make_unique<TextObjectComponent>("Spacebar for sound", font2));
	//text2->GetTransform()->AddTranslate(glm::vec3{ 0,30,0 });

	//scene.Add(scoreHolder);
	//scene.Add(lifeHolder);
	//scene.Add(text1);
	//scene.Add(text2);

}

int main(int, char*[]) {
	/*if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else {
		std::cout << "Successfully initialized steam." << std::endl;
	}*/

	dae::Minigin engine("../Data/");
	engine.Run(load);

	//SteamAPI_Shutdown();
    return 0;
}