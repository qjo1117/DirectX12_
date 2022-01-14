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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
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
		if (object == nullptr) {		// �̰� �ٷ� ���������ʰ� FinalUpdate�κп��� ���� ��û ó����
										// ó���ϴ� �κ��� �߰��Ǹ� nullptr������ �����״� ����
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
		/* ----- Layer�� �������� ��� Layer�ϳ��� ��ȸ�� �غ���. ----- */
		RemoveObject(gameObject, static_cast<uint32>(layer));
	}
	else {
		/* ----- Layer�� �� �������� ��� ��ü�� ��ȸ�Ѵ�. ----- */
		for (uint32 i = 0; i < static_cast<uint32>(LAYER_TYPE::END); ++i) {
			if (RemoveObject(gameObject, i) == true) {
				// ã������ ������ ��ȸ�� ���´�.
				break;
			}
		}
	}

	auto findGo = std::find(_objects.begin(), _objects.end(), gameObject);
	_objects.erase(findGo);
}

bool Scene::RemoveObject(shared_ptr<GameObject> gameObject, uint32 layer)
{
	/* ----- Layer�� �������� ��� Find�� ���ؼ� ã�� �����Ѵ�. ----- */
	auto findGo = std::find(_gameObjects[layer].begin(), _gameObjects[layer].end(), gameObject);
	if (findGo != _gameObjects[layer].end()) {
		_gameObjects[layer].erase(findGo);
		return true;
	}
	return false;
}
