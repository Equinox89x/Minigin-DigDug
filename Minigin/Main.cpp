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

void load()
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// make bg
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextureComponent>());
	go->GetComponent<dae::TextureComponent>()->SetTexture("background.tga");
	scene.Add(go);

	// make text
	auto go3 = std::make_shared<dae::GameObject>();
	go3->AddComponent(std::make_unique<dae::TextObjectComponent>("Programming 4 Assignment", font));
	go3->GetComponent<dae::TextObjectComponent>()->SetPosition(80, 20);
	scene.Add(go3);

	//make logo
	auto go2 = std::make_shared<dae::GameObject>();
	go2->GetTransform()->TranslateWorld(216, 180);
	go2->AddComponent(std::make_unique<dae::TextureComponent>());
	go2->GetComponent<dae::TextureComponent>()->SetTexture("logo.tga");
	scene.Add(go2);


	//make fps counter
	go->AddComponent(std::make_unique<dae::FPSCounterComponent>("2000", font));

	//rotator component
	auto go4 = std::make_shared<dae::GameObject>();
	go4->AddComponent(std::make_unique<dae::RotatorComponent>());
	go4->GetComponent<dae::RotatorComponent>()->SetRotationSpeed(100);
	go4->GetTransform()->TranslateWorld(300, 300);
	go4->GetTransform()->SetRotationOffset(glm::vec3{ 50, 50, 0 });
	go4->AddComponent(std::make_unique<dae::TextureComponent>());
	go4->GetComponent<dae::TextureComponent>()->SetTexture("ESNstar.png");
	go4->GetComponent<dae::TextureComponent>()->Scale(0.2f, 0.2f);	
	go4->GetComponent<dae::TextureComponent>()->SetCanRotate(true);
	go4->GetComponent<dae::TextureComponent>()->SetRotationSpeed(-50);
	
	//auto go5 = std::make_shared<dae::GameObject>();
	//go5->GetTransform()->TranslateWorld(300, 300);
	//go5->AddComponent(std::make_unique<dae::TextureComponent>());
	//go5->GetComponent<dae::TextureComponent>()->SetTexture("ESNstar.png");
	//go5->GetComponent<dae::TextureComponent>()->Scale(0.2f, 0.2f);
	//scene.Add(go5);



	auto child{ go4->AddChild(new dae::GameObject()) };
	child->AddComponent(std::make_unique<dae::RotatorComponent>());
	child->GetComponent<dae::RotatorComponent>()->SetRotationSpeed(200);
	child->GetTransform()->SetRotationOffset(glm::vec3{ 50, 50, 0 });
	child->AddComponent(std::make_unique<dae::TextureComponent>());
	child->GetComponent<dae::TextureComponent>()->SetTexture("ESNstar.png");
	child->GetComponent<dae::TextureComponent>()->Scale(0.2f, 0.2f);
	child->GetComponent<dae::TextureComponent>()->SetCanRotate(true);
	child->GetComponent<dae::TextureComponent>()->SetRotationSpeed(-100);


	scene.Add(go4);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}