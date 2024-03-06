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
	scene.Add(go);
	go->AddComponent(std::make_unique<dae::TextureComponent>());
	go->GetComponent<dae::TextureComponent>()->SetTexture("background.tga");

	// make text
	auto go3 = std::make_shared<dae::GameObject>();
	scene.Add(go3);
	go3->AddComponent(std::make_unique<dae::TextObjectComponent>("Programming 4 Assignment", font));
	go3->GetComponent<dae::TextObjectComponent>()->SetPosition(80, 20);

	//make logo
	auto go2 = std::make_shared<dae::GameObject>();
	scene.Add(go2);
	go2->GetTransform()->TranslateWorld(216, 180);
	go2->AddComponent(std::make_unique<dae::TextureComponent>());
	go2->GetComponent<dae::TextureComponent>()->SetTexture("logo.tga");


	//make fps counter
	go->AddComponent(std::make_unique<dae::FPSCounterComponent>("2000", font));

	//rotator component
	auto go4 = std::make_shared<dae::GameObject>();
	scene.Add(go4);

	auto rot = go4->AddComponent(std::make_unique<dae::RotatorComponent>());
	rot->SetRotationSpeed(100);
	go4->GetTransform()->TranslateWorld(300, 300);
	go4->GetTransform()->SetRotationOffset(glm::vec3{ 50, 50, 0 });
	auto tex = go4->AddComponent(std::make_unique<dae::TextureComponent>());
	tex->SetTexture("ESNstar.png");
	tex->Scale(0.2f, 0.2f);	
	tex->SetCanRotate(true);
	tex->SetRotationSpeed(-50);

	//go4->AddChild(new dae::GameObject());
	auto child = std::make_shared<dae::GameObject>();
	scene.Add(child);
	go4->AddChild(child.get());

	auto rot2 = child->AddComponent(std::make_unique<dae::RotatorComponent>());
	rot2->SetRotationSpeed(200);
	child->GetTransform()->SetRotationOffset(glm::vec3{ 50, 50, 0 });
	auto tex2 = child->AddComponent(std::make_unique<dae::TextureComponent>());
	tex2->SetTexture("ESNstar.png");
	tex2->Scale(0.2f, 0.2f);
	tex2->SetCanRotate(true);
	tex2->SetRotationSpeed(-100);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}