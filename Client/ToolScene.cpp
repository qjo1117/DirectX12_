#include "pch.h"
#include "ToolScene.h"
#include "GameObject.h"
#include "Engine.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Timer.h"
#include "Camera.h"
#include "CameraController.h"
#include "RotationXComponent.h"
#include "EngineGUI.h"
#include "Resources.h"


ToolScene::ToolScene()
{
#pragma region Object

	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName("Player");


		shared_ptr<Material> material = make_shared<Material>();

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		material->SetShader(shader);

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Init(L"..\\Resources\\Texture\\Picture.jpg");
		material->SetTexture(0, texture);

		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 200.0f));
		go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

		go->AddComponent(renderer);
		go->AddComponent(make_shared<RotationXComponent>());
		AddGameObject(go, LAYER_TYPE::DEFAULT);
		_player = go;
	}

	{
		shared_ptr<GameObject> go = make_shared<GameObject>();

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadSphereMesh();
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shared_ptr<Material> material = make_shared<Material>();

		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\Picture1.png");

		material->SetShader(shader);
		material->SetTexture(0, texture);

		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->AddComponent(renderer);
		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<RotationXComponent>());

		go->GetTransform()->SetLocalPosition(Vec3(50.0f, 0.0f, 200.0f));
		go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		AddGameObject(go, LAYER_TYPE::DEFAULT);
		_obj1 = go;
	}
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadQuadMesh();
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shared_ptr<Material> material = make_shared<Material>();

		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\Picture1.png");

		material->SetShader(shader);
		material->SetTexture(0, texture);
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->AddComponent(renderer);
		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<RotationXComponent>());

		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 1.0f, 200.0f));
		go->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		go->GetTransform()->SetParent(_player->GetTransform());
		AddGameObject(go, LAYER_TYPE::DEFAULT);
		_obj2 = go;
	}
#pragma endregion

#pragma region Camera
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->SetName(L"MainCamera");
		go->SetGUIName("MainCamera");

		go->Init();
		go->AddComponent(make_shared<Camera>());
		go->AddComponent(make_shared<CameraController>());
		_camera = go;
		_camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.f, 0.0f));
		AddGameObject(_camera, LAYER_TYPE::CAMERA);
	}
#pragma endregion

	GEngine->GetCmdQueue()->WaitSync();

	/* ----- 바인딩 부분 ----- */
	GUI->AddFunction([=]() { TestEditor(); });
	GUI->AddFunction([=]() { TestObjects(); });
	GUI->AddFunction([=]() { TestButtonCreateObject(); });
}

ToolScene::~ToolScene()
{
}

void ToolScene::Awake()
{
	Scene::Awake();
}

void ToolScene::Start()
{
	Scene::Start();
}

void ToolScene::Update()
{
	Scene::Update();

	/* ----- TODO Test SceneChange ----- */
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE)) {
		GET_SINGLE(SceneManager)->LoadScene(SCENE_TYPE::START);
	}
}

void ToolScene::LateUpdate()
{
	Scene::LateUpdate();
}

void ToolScene::FinalUpdate()
{
	Scene::FinalUpdate();
}

void ToolScene::TestEditor()
{
	/* ----- 객체의 정보를 보여줍니다. ----- */
	ImGui::Begin("Editor");
	ImGui::SetWindowSize("Editor", ImVec2(350, 600));

	// 선택한 녀석이 없으면 종료
	if (_pick == nullptr) {
		ImGui::End();
		return;
	}

	ImGui::Text(_pick->GetGUIName().data());		// 이름을 보여줌

	// Transform.
	if (_pick->GetFixedComponent(COMPONENT_TYPE::TRANSFORM) != nullptr) {
		if (ImGui::CollapsingHeader("Transform")) {
			_state.transform = !_state.transform;

			const float dragFloatWidth = 60.0f;
			const float dragSpeed = 0.1f;

			static Vec3 position;
			static Vec3 rotation;
			static Vec3 scale;

			position = _pick->GetTransform()->GetLocalPosition();
			rotation = _pick->GetTransform()->GetLocalRotation();
			scale = _pick->GetTransform()->GetLocalScale();

			// TODO : x,y,z값을 독립성을 만들어서 X,Y,Z 텍스트를 공용으로 사용할 수 있도록 해야함.
#pragma region Position
			ImGui::Text("Position");

			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("X", &position.x, dragSpeed, -10000.0f, 10000.0f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("Y", &position.y, dragSpeed, -10000.0f, 10000.0f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("Z", &position.z, dragSpeed, -10000.0f, 10000.0f);

			ImGui::Separator();
#pragma endregion
#pragma region Rotation
			ImGui::Text("Rotation");

			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("rX", &rotation.x, dragSpeed, -10000.0f, 10000.0f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("rY", &rotation.y, dragSpeed, -10000.0f, 10000.0f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("rZ", &rotation.z, dragSpeed, -10000.0f, 10000.0f);

			ImGui::Separator();
#pragma endregion
#pragma region Scale
			ImGui::Text("Scale");

			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("sX", &scale.x, dragSpeed, -10000.0f, 10000.0f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("sY", &scale.y, dragSpeed, -10000.0f, 10000.0f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(dragFloatWidth);
			ImGui::DragFloat("sZ", &scale.z, dragSpeed, -10000.0f, 10000.0f);

			ImGui::Separator();
#pragma endregion

			_pick->GetTransform()->SetLocalPosition(position);
			_pick->GetTransform()->SetLocalRotation(rotation);
			_pick->GetTransform()->SetLocalScale(scale);
		}


	}

	// MeshRenderer
	if (_pick->GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER) != nullptr) {
		if (ImGui::CollapsingHeader("MeshRenderer")) {

			if (ImGui::Button("Quad")) {
				_pick->GetMeshRenderer()->SetMesh(GET_SINGLE(Resources)->LoadQuadMesh());
			}
			if (ImGui::Button("Sphere")) {
				_pick->GetMeshRenderer()->SetMesh(GET_SINGLE(Resources)->LoadSphereMesh());
			}
			if (ImGui::Button("Cube")) {
				_pick->GetMeshRenderer()->SetMesh(GET_SINGLE(Resources)->LoadCubeMesh());
			}
		}
	}

	// Constorm Scripts
	for (shared_ptr<MonoBehaviour>& script : _pick->GetScripts()) {
		script->DrawGUI();
	}

	ImGui::End();
}

void ToolScene::TestObjects()
{
	/* ----- 객체들이 뭐뭐 있는지 보여줍니다. ----- */
	ImGui::Begin("ObjectList");
	ImGui::SetWindowSize("ObjectList", ImVec2(200, 600));

	for (shared_ptr<GameObject>& go : _objects) {
		if (ImGui::Button(go->GetGUIName().data())) {			// 일단 선택해야되서 생각나는게 버튼이라 버튼으로 만듬
			_pick = go;
		}
	}

	uint32 fps = GET_SINGLE(Timer)->GetFps();
	static char fpsText[50];
	::sprintf_s(fpsText, 50, "FPS : %u", fps);
	ImGui::Text(fpsText);

	ImGui::End();
}

void ToolScene::TestButtonCreateObject()
{
	if (ImGui::Button("CreateButton", ImVec2(100, 100))) {
		{
			shared_ptr<GameObject> go = make_shared<GameObject>();

			shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shared_ptr<Material> material = make_shared<Material>();

			shader->Init(L"..\\Resources\\Shader\\default.hlsli");
			texture->Init(L"..\\Resources\\Texture\\Picture1.png");

			material->SetShader(shader);
			material->SetTexture(0, texture);
			renderer->SetMesh(mesh);
			renderer->SetMaterial(material);

			go->AddComponent(renderer);
			go->AddComponent(make_shared<Transform>());
			go->AddComponent(make_shared<RotationXComponent>());

			int32 ranX = rand() % 500 - 250;
			int32 ranZ = rand() % 500 - 250;

			go->GetTransform()->SetLocalPosition(Vec3(ranX, 1.0f, ranZ));
			go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
			AddGameObject(go, LAYER_TYPE::DEFAULT);
		}
	}
	if (ImGui::Button("RemoveButton", ImVec2(100, 100))) {
		if (_pick == nullptr) {
			return;
		}

		RemoveObject(_pick, LAYER_TYPE::DEFAULT);
	}
}
