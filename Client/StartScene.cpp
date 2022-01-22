#include "pch.h"
#include "StartScene.h"
#include "Engine.h"
#include "GameObject.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "CameraController.h"

StartScene::StartScene()
{
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName("Player");


		shared_ptr<Material> material = make_shared<Material>();

		material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Forward"));

		material->SetTexture(0, GET_SINGLE(Resources)->Get<Texture>(L"Metal"));

		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 200.0f));
		go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 1.0f));

		go->AddComponent(renderer);
		AddGameObject(go, LAYER_TYPE::DEFAULT);
	}

	{
		_camera = make_shared<GameObject>();
		_camera->AddComponent(make_shared<Camera>());
		_camera->AddComponent(make_shared<Transform>());
		_camera->AddComponent(make_shared<CameraController>());
		AddGameObject(_camera, LAYER_TYPE::CAMERA);
	}
	GEngine->GetGraphicsCmdQueue()->WaitSync();
}

StartScene::~StartScene()
{
}

void StartScene::Awake()
{
	Scene::Awake();
}

void StartScene::Start()
{
	Scene::Start();
}

void StartScene::Update()
{
	Scene::Update();

	/* ----- TODO Test SceneChange ----- */
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) {
		GET_SINGLE(SceneManager)->LoadScene(SCENE_TYPE::TOOL);
	}
}

void StartScene::LateUpdate()
{
	Scene::LateUpdate();
}

void StartScene::FinalUpdate()
{
	Scene::FinalUpdate();
}
