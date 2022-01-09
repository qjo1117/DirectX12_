#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Awake()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (const vector<shared_ptr<GameObject>>& gameObjects : _gameObjects) {
		for (const shared_ptr<GameObject> obj : gameObjects) {
			if (obj != nullptr) {
				obj->Awake();
			}
		}
	}
}

void Scene::Start()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (const vector<shared_ptr<GameObject>>& gameObjects : _gameObjects) {
		for (const shared_ptr<GameObject> obj : gameObjects) {
			if (obj != nullptr) {
				obj->Start();
			}
		}
	}
}

void Scene::Update()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (const vector<shared_ptr<GameObject>>& gameObjects : _gameObjects) {
		for (const shared_ptr<GameObject> obj : gameObjects) {
			if (obj != nullptr) {
				obj->Update();
			}
		}
	}
}

void Scene::LateUpdate()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (const vector<shared_ptr<GameObject>>& gameObjects : _gameObjects) {
		for (const shared_ptr<GameObject> obj : gameObjects) {
			if (obj != nullptr) {
				obj->LateUpdate();
			}
		}
	}
}

void Scene::FinalUpdate()
{
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (const vector<shared_ptr<GameObject>>& gameObjects : _gameObjects) {
		for (const shared_ptr<GameObject> obj : gameObjects) {
			if (obj != nullptr) {
				obj->FinalUpdate();
			}
		}
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject, uint32 layer)
{
	_gameObjects[layer].push_back(gameObject);
	_objects.push_back(gameObject);
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
