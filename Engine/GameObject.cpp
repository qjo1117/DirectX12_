#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "MonoBehaviour.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
	AddComponent(make_shared<Transform>());
}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components) {
		if (component) {
			component->Awake();
		}
	}
	for (shared_ptr<MonoBehaviour>& script : _scripts) {
		if (script) {
			script->Awake();
		}
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components) {
		if (component) {
			component->Start();
		}
	}
	for (shared_ptr<MonoBehaviour>& script : _scripts) {
		if (script) {
			script->Start();
		}
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components) {
		if (component) {
			component->Update();
		}
	}
	for (shared_ptr<MonoBehaviour>& script : _scripts) {
		if (script) {
			script->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components) {
		if (component) {
			component->LateUpdate();
		}
	}
	for (shared_ptr<MonoBehaviour>& script : _scripts) {
		if (script) {
			script->LateUpdate();
		}
	}
}

shared_ptr<Transform> GameObject::GetTransform()
{
	uint8 index = static_cast<uint8>(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(_components[index]);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	/* ----- 컴포넌트의 주인을 알려주고 맵핑번호가 넘어가면 MonoBehaviour라고 취급 ----- */
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT) {
		_components[index] = component;
	}
	else {
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}
