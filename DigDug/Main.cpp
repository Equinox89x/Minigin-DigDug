#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "../DigDug/Main.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObjectComponent.h"
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
#include <FileReader.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "HighscoreComponent.h"
#include "RockComponent.h"


using namespace dae;
using namespace rapidjson;

SDL_Rect leftMapBorder;
SDL_Rect topMapBorder;
SDL_Rect rightMapBorder;
SDL_Rect bottomMapBorder;
std::shared_ptr<dae::Font> font;
std::shared_ptr<dae::Font> font2;
std::shared_ptr<dae::Font> font3;

#pragma region Input
void MakePlayerInput(Scene* scene, std::shared_ptr<GameObject>& mainPlayer, std::shared_ptr<GameObject>& pump, Input::InputKey up, Input::InputKey down, Input::InputKey left, Input::InputKey right, Input::InputKey upCancel, Input::InputKey downCancel, Input::InputKey leftCancel, Input::InputKey rightCancel, Input::InputKey blow, bool isVersus) {
	Input::GetInstance().BindKey(up, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::UP, isVersus ? "Enemies/FygarRight" : "Character/moveUp", glm::vec3(0.f, -100.f, 0.0f)));
	Input::GetInstance().BindKey(down, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::DOWN, isVersus ? "Enemies/FygarLeft" : "Character/moveDown", glm::vec3(0.f, 100.f, 0.0f)));
	Input::GetInstance().BindKey(left, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::LEFT, isVersus ? "Enemies/FygarLeft" : "Character/moveLeft", glm::vec3(-100.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey(right, std::make_unique<Move>(scene, mainPlayer.get(), MathLib::Movement::RIGHT, isVersus ? "Enemies/FygarRight" : "Character/moveRight", glm::vec3(100.f, 0.0f, 0.0f)));


	if (isVersus) {
		Input::GetInstance().BindKey(blow, std::make_unique<FygarFire>(scene, mainPlayer.get()));
	}
	else {
		Input::GetInstance().BindKey(blow, std::make_unique<Pump>(scene, mainPlayer.get(), pump.get()));
	}

	Input::GetInstance().BindKey(upCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::UP));
	Input::GetInstance().BindKey(downCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::DOWN));
	Input::GetInstance().BindKey(leftCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::LEFT));
	Input::GetInstance().BindKey(rightCancel, std::make_unique<StopMove>(scene, mainPlayer.get(), MathLib::Movement::RIGHT));
}

void MapControllerInput(int id, Input::InputKey& up, Input::InputKey& down, Input::InputKey& left, Input::InputKey& right, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow){
	//up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::JoystickLeft, id);
	up = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadUp, id);
	down = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadDown, id);
	left = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadLeft, id);
	right = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::DpadRight, id);

	upCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadUp, id);
	downCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadDown, id);
	leftCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadLeft, id);
	rightCancel = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::DpadRight, id);

	blow = std::make_tuple(ButtonStates::BUTTON_UP, ControllerButton::ButtonA, id);
}
void MapKeyboardInput(int id, Input::InputKey& up, Input::InputKey& down, Input::InputKey& left, Input::InputKey& right, Input::InputKey& upCancel, Input::InputKey& downCancel, Input::InputKey& leftCancel, Input::InputKey& rightCancel, Input::InputKey& blow){
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
#pragma endregion

#pragma region Making objects
void MakePlayer(Scene* scene, std::string textureName, int id, glm::vec2 startPos, MathLib::GameMode gm, bool isController) {

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
	pump->AddComponent(std::make_unique<PumpComponent>(scene))->SetPlayer(mainPlayer.get());
	pump->AddComponent(std::make_unique<TextureComponent>());
	auto texPunp{ pump->GetComponent<TextureComponent>() };
	texPunp->SetTexture("Character/PumpRight.png", 0.1f, 1);
	texPunp->Scale(1.5f, 1.5f);
	texPunp->SetIsVisible(false);

	//lives
	auto values{ mainPlayer->AddComponent(std::make_unique<ValuesComponent>(scene)) };
	auto observer{ std::make_shared<ScoreObserver>(scene, id) };
	values->AddObserver(observer);
	values->SetLives(3);
	auto observer2{ std::make_shared<HealthObserver>(scene, id) };
	values->AddObserver(observer2);

	//Texture
	mainPlayer->AddComponent(std::make_unique<TextureComponent>());
	auto tex{ mainPlayer->GetComponent<TextureComponent>() };
	tex->SetName(playerName);
	tex->SetTexture(textureName, 0.1f, 2);
	tex->Scale(2.4f, 2.4f);

	mainPlayer->AddComponent(std::make_unique<AudioComponent>());
	startPos.y -= 2.5f;
	mainPlayer->AddComponent(std::make_unique<EntityMovementComponent>(scene, startPos))->SetMapBorders(leftMapBorder, topMapBorder, rightMapBorder, bottomMapBorder);
	mainPlayer->GetTransform()->Translate(startPos);
	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(scene, startPos));

	mainPlayer->GetComponent<EntityMovementComponent>()->SetIsController(isController);
	mainPlayer->AddComponent(std::make_unique<InputComponent>(scene, mainPlayer->GetTransform()->GetPosition(), false));

	Input::InputKey up, down, left, right;
	Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
	Input::InputKey blow;

	if (gm == MathLib::GameMode::SOLO) {
		MapControllerInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow);
		MakePlayerInput(scene, mainPlayer, pump, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow, false);

		MapKeyboardInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow);
	}
	else {
		if (isController) {
			MapControllerInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow);
		}
		else {

			MapKeyboardInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow);
		}
	}
	MakePlayerInput(scene, mainPlayer, pump, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow, false);
}

void MakeMainChar(dae::Scene* scene, glm::vec2 startPos, MathLib::GameMode gm, bool isController = false) {
	MakePlayer(scene, "Character/moveRight.png", 0, startPos, gm, isController);
}

void MakeSecondaryChar(Scene* scene, glm::vec2 startPos, MathLib::GameMode gm, bool isController = false) {
	MakePlayer(scene, "Character/moveRight.png", 1, startPos, gm, isController);
}

void MakeVersusChar(Scene* scene, glm::vec2 startPos, MathLib::GameMode gm, bool isController = false) {
	MakePlayer(scene, "Enemies/FygarRight.png", 1, startPos, gm, isController);
}

void MakeEnemy(Scene* scene, std::string textureName, int id, glm::vec2 startPos, dae::EEnemyType enemyType, bool isVersus = false) {
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

	auto comp{ enemy->AddComponent(std::make_unique<EntityMovementComponent>(scene, startPos, !isVersus)) };
	comp->SetMapBorders(leftMapBorder, topMapBorder, rightMapBorder, bottomMapBorder);
	comp->SetEnemyName(enemyType == EEnemyType::Pooka ? "Pooka" : "Fygar");
	auto enemyComp{ enemy->AddComponent(std::make_unique<EnemyComponent>(scene, enemyType, isVersus)) };
	auto observer{ std::make_shared<EnemyDeathObserver>(scene) };
	enemyComp->AddObserver(observer);
	enemy->GetTransform()->Translate(startPos);

	if (isVersus) {
		enemy->AddComponent(std::make_unique<InputComponent>(scene, enemy->GetTransform()->GetPosition(), true));
		Input::InputKey up, down, left, right;
		Input::InputKey upCancel, downCancel, leftCancel, rightCancel;
		Input::InputKey blow;
		MapControllerInput(id, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow);
		MakePlayerInput(scene, enemy, enemy, up, down, left, right, upCancel, downCancel, leftCancel, rightCancel, blow, isVersus);
	}
}

void MakePookaEnemy(Scene* scene, glm::vec2 startPos) {
	MakeEnemy(scene, "Enemies/PookaLeft.png", 0, startPos, dae::EEnemyType::Pooka);
}

void MakeDragonEnemy(Scene* scene, glm::vec2 startPos, bool isVersus = false) {
	MakeEnemy(scene, "Enemies/FygarLeft.png", 0, startPos, dae::EEnemyType::Fygar, isVersus);
}

void MakeLevel(Scene* scene, int levelIndex, std::string sceneName) {
	auto backgroundGameObject = std::make_shared<GameObject>();
	backgroundGameObject->SetName(EnumStrings[Background]);
	scene->Add(backgroundGameObject);

	auto background{ backgroundGameObject->AddComponent(std::make_unique<TextureComponent>()) };
	background->SetTexture("Levels/Level1.png");
	background->Scale(5, 5);

	backgroundGameObject->AddComponent(std::make_unique<AudioComponent>())->PlayGameMusicSound();

	auto pathObject = std::make_shared<GameObject>();
	scene->Add(pathObject);
	pathObject->SetName(EnumStrings[PathCreator]);
	auto pathCreator{ pathObject->AddComponent(std::make_unique<PathwayCreatorComponent>(scene))};

	FileReader* file{ new FileReader("../Data/game.json") };
	auto levels = file->GetDocument()["Levels"].GetObj();
	auto level = levels[sceneName.c_str()].GetArray();
	auto selectedLevel = level[levelIndex].GetObj()["tiles"].GetArray();

	int index{ 0 };
	int startX{ 5 }, startY{ 45 }, modifier{ 3 };
	int height{ file->GetDocument()["MapHeight"].GetInt() }, width{ file->GetDocument()["MapWidth"].GetInt() }, size{ file->GetDocument()["Cellsize"].GetInt() };
	glm::vec2 pos{ startX, startY };

	leftMapBorder = SDL_Rect{ static_cast<int>(pos.x), static_cast<int>(pos.y * modifier), size, WindowSizeY - startY };
	topMapBorder = SDL_Rect{ static_cast<int>(pos.x), static_cast<int>((pos.y * modifier) - size), WindowSizeX - startX, size };
	for (const auto& row : selectedLevel)
	{
		std::istringstream iss(row.GetString());
		std::string token;
		std::vector<std::string> tokens;

		while (std::getline(iss, token, '|')) {
			pathCreator->AddPathway(index, pos, token, { pos.x * modifier, pos.y * modifier }, { Cellsize * modifier , Cellsize * modifier });
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

	index = 0;
	pos = { startX, startY };
	for (const auto& row : selectedLevel)
	{
		std::istringstream iss(row.GetString());
		std::string token;
		std::vector<std::string> tokens;

		while (std::getline(iss, token, '|')) {
			if (token == "rock") {
				auto rock{ std::make_shared<GameObject>() };
				auto comp{ rock->AddComponent(std::make_unique<RockComponent>(scene)) };
				rock->GetTransform()->Translate({ pos.x * modifier, pos.y * modifier });
				rock->AddComponent(std::make_unique<TextureComponent>())->SetTexture("Levels/Rock.png", 0.1f, 1, true, false);
				auto observer{ std::make_shared<RockDeathObserver>(scene) };
				comp->AddObserver(observer);
				rock->AddComponent(std::make_unique<AudioComponent>());
				scene->Add(rock);
			}
			index++;
			pos.x += Cellsize;
		}
		pos.x = static_cast<float>(startX);
		pos.y += Cellsize - (Cellsize * 0.25f);
	}
	delete file;
}
#pragma endregion

#pragma region Making a level scene
void CreateLevelDetails(MathLib::GameMode gm, int levelIndex, std::string sceneName) {
	auto& scene = SceneManager::GetInstance().CreateScene(gm == MathLib::GameMode::SOLO ? sceneName + std::to_string(levelIndex) : sceneName);

	MakeLevel(&scene, levelIndex, sceneName);

	auto pathcreator{ scene.GetGameObject(EnumStrings[PathCreator]) };
	auto pathcreatorComp{ pathcreator->GetComponent<PathwayCreatorComponent>() };

	int i{ 0 };
	for (auto spawn : pathcreatorComp->GetSpawns())
	{
		auto location{ spawn.TextureComponent->GetGameObject()->GetTransform()->GetFullPosition() };
		if (i % 2 == 0) {
			MakeMainChar(&scene, location, gm);
		}
		else {
			gm == MathLib::GameMode::VERSUS ? MakeDragonEnemy(&scene, location, true) : MakeSecondaryChar(&scene, location, gm, true);
		}
		i++;
	}
	
	i = 0;
	for (auto spawn : pathcreatorComp->GetEnemySpawns())
	{
		auto location{ spawn.TextureComponent->GetGameObject()->GetTransform()->GetFullPosition() };
		if (i % 2 == 0) {
			MakePookaEnemy(&scene, location);
		}
		else {
			MakeDragonEnemy(&scene, location);
		}
		i++;
	}

	//MakeMainChar(&scene, glm::vec2{ WindowSizeX / 2 + (Cellsize / 2), WindowSizeY / 2 - (Cellsize / 2) });
	//MakeSecondaryChar(&scene, glm::vec2{ WindowSizeX / 2, WindowSizeY / 2 });
	/*MakePookaEnemy(&scene, glm::vec2{ WindowSizeX / 2 + (Cellsize / 2), WindowSizeY / 2 - (Cellsize / 2) });
	MakeDragonEnemy(&scene, glm::vec2{ WindowSizeX / 2 - (Cellsize * 4), WindowSizeY / 2 - (Cellsize / 2) });*/

	scene.SetActive(true);

	CreateGlobalGameObject(&scene, gm);

	pathcreatorComp->Init();

}
#pragma endregion

void CreateLevel(MathLib::GameMode gm, int levelIndex) {

	switch (gm)
	{
	case MathLib::GameMode::SOLO:
		CreateLevelDetails(gm, levelIndex, EnumStrings[SoloLevelNameGeneral]);
		break;
	case MathLib::GameMode::COOP:
		CreateLevelDetails(gm, levelIndex, EnumStrings[CoopLevelName]);
		break;
	case MathLib::GameMode::VERSUS:
		CreateLevelDetails(gm, levelIndex, EnumStrings[VersusLevelName]);
		break;
	default:
		break;
	}

}

#pragma region Score
void CreateGlobalGameObject(dae::Scene* scene, MathLib::GameMode gm) {

	auto global = std::make_shared<GameObject>();
	global->SetName(EnumStrings[Names::Global]);
	global->AddComponent(std::make_unique<MenuComponent>(scene, &CreateLevel, &MakeHighScoreScreen, std::vector<glm::vec2>{}, gm));
	scene->Add(global);

	Input::InputKey skip;
	skip = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_F1, 0);
	Input::GetInstance().BindKey(skip, std::make_unique<Skip>(scene, global.get()));

	auto scoreHolder = std::make_shared<GameObject>();
	scoreHolder->SetName(EnumStrings[Names::Score1]);
	scoreHolder->AddComponent(std::make_unique<TextObjectComponent>("Player1 Score: " + std::to_string(0), font3));
	scoreHolder->GetTransform()->AddTranslate(glm::vec3{ 10,10,0 });

	if (gm != MathLib::GameMode::SOLO) {
		auto scoreHolder2 = std::make_shared<GameObject>();
		scoreHolder2->SetName(EnumStrings[Score2]);
		scoreHolder2->AddComponent(std::make_unique<TextObjectComponent>("Player2 Score: " + std::to_string(0), font3));
		scoreHolder2->GetTransform()->AddTranslate(glm::vec3{ WindowSizeX-150,10,0 });
		scene->Add(scoreHolder2);
	}

	auto lifeHolder = std::make_shared<GameObject>();
	lifeHolder->SetName(EnumStrings[Life]);
	lifeHolder->AddComponent(std::make_unique<TextObjectComponent>("Lives: " + std::to_string(3), font3));
	lifeHolder->GetTransform()->AddTranslate(glm::vec3{ 10,30,0 });

	auto text2 = std::make_shared<GameObject>();
	text2->AddComponent(std::make_unique<TextObjectComponent>("Round" + std::to_string(1), font));
	text2->GetTransform()->AddTranslate(glm::vec3{ (WindowSizeX/2)-60,30,0 });

	scene->Add(scoreHolder);
	scene->Add(lifeHolder);
	scene->Add(text2);

	FileReader* file2{ new FileReader("../Data/save.json") };
	auto previousScore = file2->GetDocument()["Score"].GetString();
	scene->GetGameObject(EnumStrings[Names::Player0])->GetComponent<ValuesComponent>()->IncreaseScore(std::stoi(previousScore));
	delete file2;
}
#pragma endregion

#pragma region Menu
void CreateSelectorInput(dae::Scene* scene) {

	//game mode selection
	std::shared_ptr<GameObject> player1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player3 = std::make_shared<dae::GameObject>();
	scene->Add(player1);
	scene->Add(player2);
	scene->Add(player3);

	player1->AddComponent(std::make_unique<TextObjectComponent>("SOLO", font2));
	player2->AddComponent(std::make_unique<TextObjectComponent>("COOP", font2));
	player3->AddComponent(std::make_unique<TextObjectComponent>("VERSUS", font2));
	player1->GetTransform()->TranslateWorld(WindowSizeX / 2 - Margin * 3, 75);
	player2->GetTransform()->TranslateWorld(WindowSizeX / 2 - Margin * 3, 75 + Margin);
	player3->GetTransform()->TranslateWorld(WindowSizeX / 2 - Margin * 3, 75 + SubMargin);

	//game mode selector
	std::shared_ptr<GameObject> selector = std::make_shared<dae::GameObject>();
	selector->SetName(EnumStrings[Selector]);
	scene->Add(selector);

	std::vector<glm::vec2> positions{
		player1->GetTransform()->GetFullPosition(),
		player2->GetTransform()->GetFullPosition(),
		player3->GetTransform()->GetFullPosition(),
	};
	positions[0].x -= 20;
	positions[1].x -= 20;
	positions[2].x -= 20;
	selector->AddComponent(std::make_unique<MenuComponent>(scene, &CreateLevel, &MakeHighScoreScreen, positions));
	selector->AddComponent(std::make_unique<TextObjectComponent>(">", font2));
	selector->GetTransform()->TranslateWorld(positions[0]);

	Input::InputKey up = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_w, 0);
	Input::InputKey down = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_s, 0);
	Input::InputKey space = std::make_tuple(ButtonStates::BUTTON_DOWN, SDLK_SPACE, 0);

	Input::GetInstance().BindKey(up, std::make_unique<CycleGameMode>(scene, selector.get(), true));
	Input::GetInstance().BindKey(down, std::make_unique<CycleGameMode>(scene, selector.get(), false));
	Input::GetInstance().BindKey(space, std::make_unique<StartGame>(scene, selector.get()));

	up = std::make_tuple(ButtonStates::BUTTON_DOWN, ControllerButton::DpadUp, 0);
	down = std::make_tuple(ButtonStates::BUTTON_DOWN, ControllerButton::DpadDown, 0);
	space = std::make_tuple(ButtonStates::BUTTON_DOWN, ControllerButton::ButtonA, 1);

	Input::GetInstance().BindKey(up, std::make_unique<CycleGameMode>(scene, selector.get(), true));
	Input::GetInstance().BindKey(down, std::make_unique<CycleGameMode>(scene, selector.get(), false));
	Input::GetInstance().BindKey(space, std::make_unique<StartGame>(scene, selector.get()));

	
}

void MakeMenu() {
	Input::GetInstance().ClearKeys();
	auto& scene = SceneManager::GetInstance().CreateScene(EnumStrings[MainMenu]);
	scene.SetActive(true);

	FileReader* file2{ new FileReader("../Data/save.json") };
	file2->WriteData({ {"Score", "0"} });

	//Title
	std::shared_ptr<GameObject> upText1 = std::make_shared<dae::GameObject>();
	scene.Add(upText1);
	upText1->AddComponent(std::make_unique<TextObjectComponent>("DIG DUG", font));
	upText1->GetTransform()->TranslateWorld(WindowSizeX / 4, Margin);
	upText1->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	CreateSelectorInput(&scene);

	FileReader* file{ new FileReader("../Data/highscore.json") };
	auto scores{ file->GetDocument()["data"].GetArray() };

	float yPos{ WindowSizeY / 2 };
	int i{ 1 };
	for (const auto& field : scores)
	{
		std::shared_ptr<GameObject> scoreObj = std::make_shared<dae::GameObject>();
		std::shared_ptr<GameObject> scoreObj2 = std::make_shared<dae::GameObject>();
		std::string score{ std::to_string(i) + "  " + field.GetObj()["name"].GetString()};
		std::string score2{ std::string(field.GetObj()["score"].GetString()) + " PTS" };
		scoreObj->AddComponent(std::make_unique<TextObjectComponent>(score, font));
		scoreObj2->AddComponent(std::make_unique<TextObjectComponent>(score2, font));
		scoreObj->GetComponent<TextObjectComponent>()->SetPosition(Margin * 2, yPos);
		scoreObj2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 + Margin, yPos);
		scene.Add(scoreObj);
		scene.Add(scoreObj2);

		i++;
		yPos += 50;
	}
	delete file;
	delete file2;

	//scene.Add(go);
}
#pragma endregion

void MakeHighScoreScreen() {
	Input::GetInstance().ClearKeys();
	auto& scene = SceneManager::GetInstance().CreateScene(EnumStrings[HighScoreName]);
	scene.SetActive(true);

	FileReader* file2{ new FileReader("../Data/save.json") };
	int scoreNr = std::stoi(file2->GetDocument()["Score"].GetString());

	FileReader* file{ new FileReader("../Data/highscore.json") };
	auto scores{ file->GetDocument()["data"].GetArray() };
	std::string highScore{ scores[0].GetObj()["score"].GetString() };

	bool isNewHighScore{ scoreNr > std::stoi(highScore) };
	//Title
	std::shared_ptr<GameObject> upText1 = std::make_shared<dae::GameObject>();
	scene.Add(upText1);
	upText1->AddComponent(std::make_unique<TextObjectComponent>(isNewHighScore ? "New Highscore!" : "Your Score", font));
	upText1->GetTransform()->TranslateWorld(WindowSizeX / 4, Margin);
	upText1->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	std::shared_ptr<GameObject> upText2 = std::make_shared<dae::GameObject>();
	scene.Add(upText2);
	upText2->AddComponent(std::make_unique<TextObjectComponent>(std::to_string(scoreNr), font));
	upText2->GetTransform()->TranslateWorld(WindowSizeX / 3, Margin * 2);
	upText2->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 225,225,225 });


	if (isNewHighScore) {
		std::shared_ptr<GameObject> upText3 = std::make_shared<dae::GameObject>();
		scene.Add(upText3);
		upText3->SetName("EditableName");
		upText3->AddComponent(std::make_unique<TextObjectComponent>("Enter Name", font));
		upText3->GetTransform()->TranslateWorld(WindowSizeX / 3, Margin * 4);
		upText3->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 200,200,200 });

		std::shared_ptr<GameObject> scoreObj = std::make_shared<dae::GameObject>();
		scoreObj->AddComponent(std::make_unique<HighscoreComponent>(&scene, scoreNr, &MakeHighScoreScreen));
		scene.Add(scoreObj);
	}
	else {
		float yPos{ WindowSizeY / 2 };
		int i{ 1 };
		for (const auto& field : scores)
		{
			std::shared_ptr<GameObject> scoreObj = std::make_shared<dae::GameObject>();
			std::shared_ptr<GameObject> scoreObj2 = std::make_shared<dae::GameObject>();
			std::string score{ std::to_string(i) + "  " + field.GetObj()["name"].GetString() };
			std::string score2{ std::string(field.GetObj()["score"].GetString()) + " PTS" };
			scoreObj->AddComponent(std::make_unique<TextObjectComponent>(score, font));
			scoreObj2->AddComponent(std::make_unique<TextObjectComponent>(score2, font));
			scoreObj->GetComponent<TextObjectComponent>()->SetPosition(Margin * 2, yPos);
			scoreObj2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 + Margin, yPos);
			scene.Add(scoreObj);
			scene.Add(scoreObj2);

			i++;
			yPos += 50;
		}

		Input::InputKey space;
		space = std::make_tuple(ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonA, 0);
		Input::GetInstance().BindKey(space, std::make_unique<BackToMenu>(&scene, &MakeMenu));
		space = std::make_tuple(ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0);
		Input::GetInstance().BindKey(space, std::make_unique<BackToMenu>(&scene, &MakeMenu));

	}
	delete file;
	delete file2;

}

void load()
{
	font = ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 20);
	font2 = ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 14);
	font3 = ResourceManager::GetInstance().LoadFont("Emulogic.ttf", 10);

	MakeMenu();
	//SceneManager::GetInstance().GetScene(EnumStrings[MainMenu])->SetActive(true);

	//MakeHighScoreScreen(1000, true);
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