#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"

Scene::Scene()
{
	_objects.reserve(1000);
}

Scene::~Scene()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (vector<shared_ptr<GameObject>>& gameObjects : _gameObjects) {
		for (shared_ptr<GameObject>& obj : gameObjects) {
			obj = nullptr;
		}
	}
	for (shared_ptr<GameObject>& obj : _objects) {
		obj = nullptr;
	}
}

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& object : _objects) {
		if (object == nullptr) {		// 이건 바로 삭제하지않고 FinalUpdate부분에서 삭제 요청 처리를
										// 처리하는 부분이 추가되면 nullptr문제가 없을테니 빼자
			continue;
		}
		if (object->GetActive() == false) {
			continue;
		}

		object->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& object : _objects) {
		if (object == nullptr) {
			continue;
		}

		if (object->GetActive() == false) {
			continue;
		}
		object->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& object : _objects) {
		if (object == nullptr) {
			continue;
		}

		if (object->GetActive() == false) {
			continue;
		}

		object->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& object : _objects) {
		if (object == nullptr) {
			continue;
		}

		if (object->GetActive() == false) {
			continue;
		}

		object->LateUpdate();
	}
}

void Scene::FinalUpdate()
{
	for (const shared_ptr<GameObject>& object : _objects) {
		if (object == nullptr) {
			continue;
		}

		if (object->GetActive() == false) {
			continue;
		}
		object->FinalUpdate();
	}
}

void Scene::Render()
{
	PushLightData();

	for (const shared_ptr<GameObject>& object : _objects) {
		if (object->GetCamera() == nullptr) {
			continue;
		}

		object->GetCamera()->Render();
	}

}

void Scene::PushLightData()
{
	LightParam lightParams = {};

	for (const shared_ptr<GameObject>& object : _objects) {
		if (object->GetLight() == nullptr) {
			continue;
		}

		const LightInfo& lightInfo = object->GetLight()->GetLightInfo();

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount += 1;
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject, uint32 layer)
{
	_gameObjects[layer].push_back(gameObject);
	_objects.push_back(gameObject);
	gameObject->SetLayer(layer);
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject, LAYER_TYPE layer)
{
	AddGameObject(gameObject, static_cast<uint32>(layer));
}

void Scene::RemoveObject(shared_ptr<GameObject> gameObject, LAYER_TYPE layer)
{
	if (layer != LAYER_TYPE::END) {
		/* ----- Layer를 지정해준 경우 Layer하나만 순회를 해본다. ----- */
		RemoveObject(gameObject, static_cast<uint32>(layer));
	}
	else {
		/* ----- Layer를 안 지정해준 경우 전체를 순회한다. ----- */
		for (uint32 i = 0; i < static_cast<uint32>(LAYER_TYPE::END); ++i) {
			if (RemoveObject(gameObject, i) == true) {
				// 찾았으면 빠르게 순회를 끊는다.
				break;
			}
		}
	}

	auto findGo = std::find(_objects.begin(), _objects.end(), gameObject);
	_objects.erase(findGo);
}

bool Scene::RemoveObject(shared_ptr<GameObject> gameObject, uint32 layer)
{
	/* ----- Layer를 지정해준 경우 Find를 통해서 찾고 삭제한다. ----- */
	auto findGo = std::find(_gameObjects[layer].begin(), _gameObjects[layer].end(), gameObject);
	if (findGo != _gameObjects[layer].end()) {
		_gameObjects[layer].erase(findGo);
		return true;
	}
	return false;
}
