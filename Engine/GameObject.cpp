#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MonoBehaviour.h"
#include "MeshRenderer.h"
#include "Camera.h"

uint32 GameObject::ID = 0;

GameObject::GameObject() : Object(OBJECT_TYPE::GAMEOBJECT)
{
	GameObject::ID += 1;
	static char nameText[50];
	::sprintf_s(nameText, 50, "Unkwon(%u)", GameObject::ID);
	_guiName = nameText;
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

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : _components) {
		if (component) {
			component->FinalUpdate();
		}
	}
}

shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER);
	return static_pointer_cast<MeshRenderer>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
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
