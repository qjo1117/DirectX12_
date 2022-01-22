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

	/* ----- CommandQueue에서 이전해옴 ----- */
	int8 backIndex = GEngine->GetSwapChain()->GetBackBufferIndex();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->ClearRenderTargetView(backIndex);
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->ClearRenderTargetView();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->ClearRenderTargetView();

	// MainCamera가 렌더링을 해준다.
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->OMSetRenderTargets();

	// 0번을 메인카메라라고 지정을 하고 나머지 Deferred에 필요한 RTV를 준비한다.
	shared_ptr<Camera> mainCamera = _gameObjects[static_cast<uint8>(LAYER_TYPE::CAMERA)][0]->GetCamera();
	mainCamera->SortGameObject();
	mainCamera->Render_Deferred();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->WaitTargetToResource();

	RenderLights();
	GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->WaitTargetToResource();

	RenderFinal();

	mainCamera->Render_Forward();

	// UI, Forward를 그려준다.
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

	// 광원을 그린다.
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

		light->GetLight()->SetLightIndex(lightParams.lightCount);		// 몇번째 빛인지 알려준다.

		lightParams.lights[lightParams.lightCount] = lightInfo;
		lightParams.lightCount += 1;
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::GLOBAL)->SetGraphicsGlobalData(&lightParams, sizeof(lightParams));
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject, uint32 layer)
{
	/* ----- 주의사항 ----- */
	// Camera, Light는 따로 순회해서 처리해야하는 작업이 있으므로 따로 빼서 LAYER에 강제로 넣어준다.

	if (gameObject->GetLight()) {
		layer = static_cast<uint8>(LAYER_TYPE::LIGHT);
	}
	else if (gameObject->GetCamera()) {
		layer = static_cast<uint8>(LAYER_TYPE::CAMERA);
	}

	// 만약 사용자가 따로 레이어를 설정한 경우에는 그에 맞게 넣어준다.
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
