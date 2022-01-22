#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "Light.h"
#include "Resources.h"

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

	/* ----- CommandQueue���� �����ؿ� ----- */
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	// MainCamera�� �������� ���ش�.
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	// 0���� ����ī�޶��� ������ �ϰ� ������ Deferred�� �ʿ��� RTV�� �غ��Ѵ�.
	shared_ptr<Camera> mainCamera = _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)][0]->GetCamera();
	mainCamera->SortGameObject();
	mainCamera->Render_Deferred();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

	RenderFinal();

	mainCamera->Render_Forward();

	// UI, Forward�� �׷��ش�.
	for (const auto& camera : _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)]) {
		if(camera->GetCamera() == nullptr) {
			continue;
		}
		if (camera->GetActive() == false) {
			continue;
		}
		if (camera->GetCamera() == mainCamera) {
			continue;
		}

		camera->GetCamera()->SortGameObject();
		camera->GetCamera()->Render_Forward();
	}
}

void Scene::RenderLights()
{
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->OMSetRenderTargets();

	// ������ �׸���.
	for (const shared_ptr<GameObject>& light : _gameObjects[static_cast<uint8>(LAYER_TYPE::LIGHT)]) {
		light->GetLight()->Render();
	}
}

void Scene::RenderFinal()
{
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->OMSetRenderTargets(1, backIndex);

	shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Final");
	material->PushGraphicsData();
	GET_SINGLE(Resources)->Get<Mesh>(L"Rectangle")->Render();
}

void Scene::PushLightData()
{
	LightParams lightParams = {};

	for (const shared_ptr<GameObject>& light : _gameObjects[static_cast<uint8>(LAYER_TYPE::LIGHT)]) {
		if (light->GetLight() == nullptr) {
			continue;
		}
		if (light->GetActive() == false) {
			continue;
		}

		const LightInfo& lightInfo = light->GetLight()->GetLightInfo();

		light->GetLight()->SetLightIndex(lightParams.lightCount);		// ���° ������ �˷��ش�.

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount += 1;
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGraphicsGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject, uint32 layer)
{
	/* ----- ���ǻ��� ----- */
	// Camera, Light�� ���� ��ȸ�ؼ� ó���ؾ��ϴ� �۾��� �����Ƿ� ���� ���� LAYER�� ������ �־��ش�.

	if (gameObject->GetLight()) {
		layer = static_cast<uint8>(LAYER_TYPE::LIGHT);
	}
	else if (gameObject->GetCamera()) {
		layer = static_cast<uint8>(LAYER_TYPE::CAMERA);
	}

	// ���� ����ڰ� ���� ���̾ ������ ��쿡�� �׿� �°� �־��ش�.
	if (gameObject->GetLayer() != static_cast<uint8>(LAYER_TYPE::DEFAULT)) {
		layer = gameObject->GetLayer();
	}

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
