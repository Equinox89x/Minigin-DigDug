#pragma once
#include <memory>
#include "TransformComponent.h"
#include <typeindex>
#include <unordered_map>
#include "Component.h"

namespace dae
{
	class Component;
	class TransformComponent;

	class GameObject final
	{
	public:

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Init();
		virtual void LateUpdate();
		virtual void Update();
		virtual void Render() const;

		void MarkForDestroy() { MarkedForDelete = true; };
		bool IsMarkedForDestroy() { return MarkedForDelete; };

		template<typename Comp>
		Comp* AddComponent(std::unique_ptr<Comp> comp) {
			if (comp) {
				comp->m_pGameObject = this;
				dynamic_cast<Component*>(comp.get())->Init();
				m_pComponents.push_back(std::move(comp));
				return dynamic_cast<Comp*>(comp.get());
			}
			return nullptr;
		}

		template<typename Comp>
		Comp* GetComponent() {
			const type_info& ti = typeid(Comp);
			for (const auto& component : m_pComponents) {
				if (component && typeid(*component.get()) == ti)
					return static_cast<Comp*>(component.get());
			}
			return nullptr;
		}		

		template<typename Comp>
		Comp* GetComponent(std::string name) {
			const type_info& ti = typeid(Comp);
			for (const auto& component : m_pComponents)
			{
				if (component && typeid(*component.get()) == ti && component->GetName() == name)
					return static_cast<Comp*>(component.get());
			}
			return nullptr;
		}		

		template<typename Comp>
		void RemoveComponent(Comp* componentPtr) {
			const std::type_info& ti = typeid(Comp);
			for (auto it = m_pComponents.begin(); it != m_pComponents.end(); ++it) {
				if (*it && typeid(**it) == ti && it->get() == componentPtr) {
					it = m_pComponents.erase(it); // Remove the component from the vector
					return;
				}
			}
		}
		void RemoveComponent(const std::unique_ptr<Component>& comp);

		void SetParent(GameObject* const parent);
		GameObject* GetParent() const;
		void RemoveChild(GameObject* pObject);
		void AddChild(GameObject* const go);
		void SetName(std::string name) { m_ObjectName = name; };
		std::string GetName() { return m_ObjectName; };

		TransformComponent* GetTransform() const { return m_pTransform; };
		std::vector<GameObject*>& GetChildren() { return m_pChildren; };
		GameObject* GetChild(std::string name);
		std::vector<GameObject*> GetChildren(std::string name);

	private:
		std::vector<GameObject*> m_pChildren{ std::vector<GameObject*>() };
		std::vector<std::unique_ptr<Component>> m_pComponents{ };
		GameObject* m_pParent{ nullptr };
		TransformComponent* m_pTransform{};
		std::string m_ObjectName{};

		bool MarkedForDelete{ false }, NeedsUpdate{ true };
	};
}
