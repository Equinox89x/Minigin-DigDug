#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject()
{
	auto comp{ std::make_unique<TransformComponent>() };
	m_pTransform = comp.get();
	AddComponent(std::move(comp));
}

dae::GameObject::~GameObject() {
	m_pComponents.clear();
};

void dae::GameObject::Init()
{
	for (const std::unique_ptr<Component>& comp : m_pComponents) {
		comp->Init();
	}

	for (GameObject* child : m_pChildren) {
		child->Init();
	}
}

void dae::GameObject::Update() {
		for (const std::unique_ptr<Component>& comp : m_pComponents) {
			if (comp->IsMarkedForDestroy()) {
				RemoveComponent(comp);
				break;
			}
		}

		for (size_t i = 0; i < m_pChildren.size(); i++)
		{
			if (m_pChildren[i]->IsMarkedForDestroy()) {
				RemoveChild(m_pChildren[i]);
				break;
			}
		}

		for (const std::unique_ptr<Component>& comp : m_pComponents) {
			comp->Update();
		}

		for (GameObject* child : m_pChildren) {
			child->Update();
		}
}

void dae::GameObject::LateUpdate()
{
}


void dae::GameObject::Render() const
{
		for (const std::unique_ptr<Component>& comp : m_pComponents) {
			if (comp->MarkedForDelete) continue;
			comp->Render();
		}

		for (GameObject* child : m_pChildren) {
			if (child->MarkedForDelete) continue;
			child->Render();
		}
}

#pragma region Component
void dae::GameObject::RemoveComponent(const std::unique_ptr<Component>& comp)
{
	if (comp) {
		m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), comp));
	}
}


void dae::GameObject::SetParent(GameObject* const parent)
{

	//Remove itself as a child from the previous parent(if any).
	if (parent->m_pParent) {
		auto it = find(parent->m_pParent->m_pChildren.begin(), parent->m_pParent->m_pChildren.end(), parent);
		parent->m_pParent->m_pChildren.erase(it);
	}
	//Set the given parent on itself.
	parent->m_pParent = this;
	//Add itself as a child to the given parent.
	m_pChildren.push_back(parent);
	//Update position, rotationand scale
	m_pTransform->UpdateTransforms();

}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}
#pragma endregion


void dae::GameObject::RemoveChild(GameObject* pObject)
{
	//Remove the given child from the children list
	auto it = find(m_pChildren.begin(), m_pChildren.end(), pObject);
	m_pChildren.erase(it);
	//	� Remove itself as a parent of the child.
	pObject->m_pParent = nullptr;
	//	� Update position, rotationand scale
	//m_pTransform->UpdateTransforms();
}

void dae::GameObject::AddChild(GameObject* const go)
{
	// Remove the given child from the child's previous parent
	//go->m_pParent
	//Set itself as parent of the child
	go->m_pParent = this;
	//Add the child to its children list.
	m_pChildren.push_back(go);
	//Update position, rotationand scale
	go->GetTransform()->UpdateTransforms();

	go->Init();
}

dae::GameObject* dae::GameObject::GetChild(std::string name) {
	for (auto obj : m_pChildren)
	{
		if (obj && obj->GetName() == name)
			return obj;
	}
	return nullptr;
}

std::vector<dae::GameObject*> dae::GameObject::GetChildren(std::string name)
{
	std::vector<GameObject*> children;

	for (auto obj : m_pChildren)
	{
		if (obj && obj->GetName() == name)
			children.push_back(obj);
	}
	return children;
}