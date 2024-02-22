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

void load()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// make bg
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextureComponent>());
	go->GetComponent<dae::TextureComponent>()->SetTexture("background.tga");

	// make text
	auto go3 = std::make_shared<dae::GameObject>();
	go3->AddComponent(std::make_unique<dae::TextObjectComponent>("Programming 4 Assignment", font));
	go3->GetComponent<dae::TextObjectComponent>()->SetPosition(80, 20);

	//make logo
	auto go2 = std::make_shared<dae::GameObject>();
	go2->AddComponent(std::make_unique<dae::TextureComponent>());
	go2->GetComponent<dae::TextureComponent>()->SetTexture("logo.tga");
	go2->GetComponent<dae::TextureComponent>()->SetPosition(216, 180);


	//make fps counter
	go->AddComponent(std::make_unique<dae::FPSCounterComponent>("2000", font));

	//removable component
	auto go4 = std::make_shared<dae::GameObject>();
	go4->AddComponent(std::make_unique<dae::TextureComponent>());
	go4->GetComponent<dae::TextureComponent>()->SetTexture("background.tga");
	go4->RemoveComponent(go4->GetComponent<dae::TextureComponent>());

	scene.Add(go);
	scene.Add(go2);
	scene.Add(go3);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}