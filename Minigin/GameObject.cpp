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
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		m_pComponents[i].release();
	}	
	
	for (size_t i = 0; i < m_pChildren.size(); i++)
	{
		auto it = find(m_pChildren.begin(), m_pChildren.end(), m_pChildren[i]);
		m_pChildren.erase(it);
	}


	m_pComponents.clear();
	m_pChildren.clear();
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

	// I would not remove "marked for destroy" objects before you loop, but after the loop.
	// Also: do that loop for all objects at once, at the end of the entire update loop.
	// You do it per game object for the children, but then you still are affected by the order of update having an impact on objects being deleted or not, which is what you want to avoid.
	// You also wont have to check in the render loop then.

	//for (size_t i = 0; i < m_pChildren.size(); i++)
	//{
	//	if (m_pChildren[i]->IsMarkedForDestroy()) {
	//		RemoveChild(m_pChildren[i]);
	//		break;
	//	}
	//}

	//for (const std::unique_ptr<Component>& comp : m_pComponents) {
	//	comp->Update(deltaTime);
	//}


	//for (GameObject* child : m_pChildren) {
	//	child->Update(deltaTime);
	//}

	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i]->IsMarkedForDestroy()) {
			RemoveComponent(m_pComponents[i]);
		}
		else {
			m_pComponents[i]->Update();
		}
	}

	for (size_t i = 0; i < m_pChildren.size(); i++)
	{
		if (m_pChildren[i]->IsMarkedForDestroy()) {
			RemoveChild(m_pChildren[i]);
		}
		else {
			m_pChildren[i]->Update();
		}
	}
}

void dae::GameObject::LateUpdate()
{
	for (size_t i = 0; i < m_pComponents.size(); i++)
	{
		if (m_pComponents[i]->IsMarkedForDestroy()) {
			RemoveComponent(m_pComponents[i]);
		}
		else {
			m_pComponents[i]->LateUpdate();
		}
	}

	//for (const std::unique_ptr<Component>& comp : m_pComponents) {
	//	if (comp->IsMarkedForDestroy()) {
	//		RemoveComponent(comp);
	//		break;
	//	}
	//}	
	//
	for (const std::unique_ptr<Component>& comp : m_pComponents) {
		comp->LateUpdate();
	}

	for (GameObject* child : m_pChildren) {
		child->LateUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const std::unique_ptr<Component>& comp : m_pComponents) {
		comp->Render();
	}

	for (GameObject* child : m_pChildren) {
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

void dae::GameObject::SetParent(GameObject* const child, bool updateTransforms)
{
	//recursive child check, check slides too

	if (child) {
		//Remove itself as a child from the previous parent(if any).
		if (child->m_pParent) {
			auto it = find(child->m_pParent->m_pChildren.begin(), child->m_pParent->m_pChildren.end(), child);
			child->m_pParent->m_pChildren.erase(it);
		}

		if (updateTransforms) {
			//Update position, rotation and scale
			m_pTransform->SetDirty();
		}

		//Set the given parent on itself.
		child->m_pParent = this;
		//Add itself as a child to the given parent.
		m_pChildren.push_back(child);
	}
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pParent;
}
#pragma endregion


void dae::GameObject::RemoveChild(GameObject* gameObject)
{
	if (gameObject) {
		//Remove the given child from the children list
		auto it = find(m_pChildren.begin(), m_pChildren.end(), gameObject);
		m_pChildren.erase(it);
		//	Update position, rotationand scale
		m_pTransform->UpdateTransforms();
		//	Remove itself as a parent of the child.
		gameObject->m_pParent = nullptr;
	}
}

dae::GameObject* dae::GameObject::AddChild(GameObject* gameObject, bool updateTransforms)
{
	if (gameObject) {
		// Remove the given child from the child's previous parent if it had a parent before
		//go->m_pParent
		//Set itself as parent of the child
		gameObject->m_pParent = this;
		//Add the child to its children list.
		m_pChildren.push_back(gameObject);

		if (updateTransforms) {
			//Update position, rotation and scale
			gameObject->GetTransform()->UpdateTransforms();
		}

		gameObject->Init();
	}

	return gameObject;
}

dae::GameObject* dae::GameObject::GetChild(std::string name) {
	for (auto child : m_pChildren)
	{
		if (child && child->GetName() == name)
			return child;
	}
	return nullptr;
}

std::vector<dae::GameObject*> dae::GameObject::GetChildren(std::string name)
{
	std::vector<GameObject*> children;

	for (auto child : m_pChildren)
	{
		if (child && child->GetName() == name)
			children.push_back(child);
	}
	return children;
}