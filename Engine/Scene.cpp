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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
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
