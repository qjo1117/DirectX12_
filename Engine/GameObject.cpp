#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MonoBehaviour.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "ParticleSystem.h"

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
	for (shared_ptr<Component>& component : _components) {
		if (component) {
			component = nullptr;
		}
	}
	for (auto& script : _scripts) {
		if (script.second) {
			script.second = nullptr;
		}
	}
	_scripts.clear();
}

void GameObject::Init()
{
	AddComponent(make_shared<Transform>());
}

void GameObject::Awake()
{
	for (const shared_ptr<Component>& component : _components) {
		if (component) {
			component->Awake();
		}
	}
	for (const pair<string, shared_ptr<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->Awake();
		}
	}
}

void GameObject::Start()
{
	for (const shared_ptr<Component>& component : _components) {
		if (component) {
			component->Start();
		}
	}
	for (const pair<string, shared_ptr<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->Start();
		}
	}
}

void GameObject::Update()
{
	for (const shared_ptr<Component>& component : _components) {
		if (component) {
			component->Update();
		}
	}
	for (const pair<string, shared_ptr<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->Update();
		}
	}
}

void GameObject::LateUpdate()
{
	for (const shared_ptr<Component>& component : _components) {
		if (component) {
			component->LateUpdate();
		}
	}
	for (const pair<string, shared_ptr<MonoBehaviour>>& script : _scripts) {
		if (script.second) {
			script.second->LateUpdate();
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

shared_ptr<Light> GameObject::GetLight()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::LIGHT);
	return static_pointer_cast<Light>(component);
}

shared_ptr<ParticleSystem> GameObject::GetParticleSystem()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM);
	return static_pointer_cast<ParticleSystem>(component);
}

