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
#include "PlayerController.h"
#include "PathManager.h"
#include "Light.h"
#include "Utils.h"
#include "Billboard.h"
#include "RenderTargetGroup.h"
#include "ParticleSystem.h"
#include "Component.h"
#include "RenderTargetGroup.h"

ToolScene::ToolScene()
{
#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadCubeMesh());
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Skybox", L"..\\Resources\\Texture\\SkyBox_1.jpg");
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		skybox->SetCheckFrustum(false);
		AddGameObject(skybox, LAYER_TYPE::SKYBOX);
	}
#pragma endregion

#pragma region Player
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName(L"Player1");

		shared_ptr<Material> material = make_shared<Material>();

		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
		material->SetShader(shader);
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadConeMesh();

		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Picture1", L"..\\Resources\\Texture\\Picture1.png");
		material->SetTexture(0, texture);


		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		go->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));

		go->AddComponent(renderer);
		go->GetOrAddComponent<PlayerController>();
		AddGameObject(go, LAYER_TYPE::DEFAULT);
		_player = go;
	}
#pragma endregion
#pragma region Player
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName(L"Player");

		shared_ptr<Material> material = make_shared<Material>();

		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Wirefram");
		material->SetShader(shader);
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCylinderMesh();

		shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Picture", L"..\\Resources\\Texture\\Picture2.png");
		material->SetTexture(0, texture);

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		go->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));

		go->AddComponent(renderer);
		go->GetOrAddComponent<PlayerController>();
		AddGameObject(go, LAYER_TYPE::DEFAULT);
	}
#pragma endregion

#pragma region Plan
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName("Plan");

		shared_ptr<Material> material = make_shared<Material>();

		material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));

		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		// BaseColor
		material->SetTexture(0, GET_SINGLE(Resources)->Load<Texture>(L"Metal", L"..\\Resources\\Texture\\Metal.jpg"));
		// Normal
		material->SetTexture(1, GET_SINGLE(Resources)->Load<Texture>(L"MetalNormal", L"..\\Resources\\Texture\\Metal_Normal.jpg"));

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->GetTransform()->SetLocalScale(Vec3(1000.0f, 1.0f, 1000.0f));
		go->GetTransform()->SetLocalPosition(Vec3(0.0f, -100.0f, 0.0f));

		go->AddComponent(renderer);
		AddGameObject(go, LAYER_TYPE::DEFAULT);
	}
#pragma endregion

#pragma region Main Camera
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<Camera>());
		go->AddComponent(make_shared<CameraController>());
		go->SetGUIName(L"MainCamera");
		_camera = go;
		_camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.f, 0.0f));
		AddGameObject(_camera, LAYER_TYPE::CAMERA);

		_camera->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, true); // UI는 안찍음
	}
#pragma endregion

#pragma region UI Camera
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go = go;

		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<Camera>());

		go->SetGUIName(L"UI_Camera");

		go->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		go->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);

		go->GetCamera()->SetCullingMaskAll();
		go->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, false);	// UI만 찍음

		AddGameObject(go, LAYER_TYPE::CAMERA);
	}
#pragma endregion

#pragma region ComputeShader
	{
		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"ComputeShader");

		// UAV 용 Texture 생성
		shared_ptr<Texture> texture = GET_SINGLE(Resources)->CreateTexture(L"UAVTexture",
			DXGI_FORMAT_R8G8B8A8_UNORM, 1024, 1024,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);

		shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"ComputeShader");
		material->SetShader(shader);
		material->SetInt(0, 1);
		GEngine->GetComputeDescHeap()->SetUAV(texture->GetUAVCpuHandle(), UAV_REGISTER::u0);

		// 쓰레드 그룹 (1 * 1024 * 1)
		material->Dispatch(1, 1024, 1);
	}
#pragma endregion


#pragma region UI_Test
	{
		shared_ptr<GameObject> uiTest = make_shared<GameObject>();
		uiTest->AddComponent(make_shared<Transform>());
		uiTest->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		uiTest->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		uiTest->SetGUIName(L"RenderTarget");

		for (int32 i = 0; i < 6; ++i) {
			shared_ptr<GameObject> renderTarget = make_shared<GameObject>();
			renderTarget->SetGUIName("RenderTarget" + std::to_string(i));
			renderTarget->AddComponent(make_shared<Transform>());
			renderTarget->GetTransform()->SetLocalPosition(Vec3(-350.0f + (i * 100.0f), 350.0f, 500.0f));
			renderTarget->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));

			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadRectangleMesh());

			{
				shared_ptr<Texture> texture;
				if (i < 3) {
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
				}
				else if (i < 5) {
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
				}
				else {
					texture = GET_SINGLE(Resources)->Get<Texture>(L"UAVTexture");
				}

				shared_ptr<Material> material = make_shared<Material>();
				material->SetTexture(0, texture);
				material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Texture"));
				meshRenderer->SetMaterial(material);
			}

			renderTarget->GetTransform()->SetParent(uiTest->GetTransform());
			renderTarget->AddComponent(meshRenderer);
			_renderTargetView.push_back(renderTarget);
			AddGameObject(renderTarget, LAYER_TYPE::UI);
		}

		AddGameObject(uiTest, LAYER_TYPE::DEFAULT);
	}
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetGUIName(L"Directional Light");
		light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(1.0f, -1.0f, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.7f, 0.5f, 0.6f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		AddGameObject(light);
	}
#pragma endregion

#pragma region Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetGUIName(L"Point Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 50.f, -50.f));
		light->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetLightRange(200.0f);

		AddGameObject(light);
	}
#pragma endregion

#pragma region Spot Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetGUIName(L"Spot Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 50.f, -0.0f));
		light->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetLightRange(1000.0f);

		AddGameObject(light);
	}
#pragma endregion

#pragma region Particle
	{
		shared_ptr<GameObject> particle = make_shared<GameObject>();
		particle->SetGUIName(L"Particle");

		particle->AddComponent(make_shared<Transform>());
		particle->AddComponent(make_shared<ParticleSystem>());
		particle->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 100.0f));

		AddGameObject(particle);
	}
#pragma endregion


	GEngine->GetGraphicsCmdQueue()->WaitSync();

	/* ----- 바인딩 부분 ----- */
	GUI->AddFunction([=]() { TestEditor(); });
	GUI->AddFunction([=]() { TestObjects(); });
	GUI->AddFunction([=]() { TestButtonCreateObject(); });
	GUI->AddFunction([=]() { TestResources(); });
	GUI->AddFunction([=]() { TestRenderTargetView(); });
}

ToolScene::~ToolScene()
{
	_goPick = nullptr;
	_player = nullptr;
	_camera = nullptr;
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

	if (INPUT->GetButtonDown(KEY_TYPE::DEL)) {
		RemoveSelectObject();
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

void ToolScene::Create()
{
	for (auto& func : _createList) {
		func();
	}
	_createList.clear();
}

void ToolScene::TestEditor()
{
	/* ----- 객체의 정보를 보여줍니다. ----- */
	ImGui::Begin("Editor");

	// 선택한 녀석이 없으면 종료
	if (_isPick == -1) {
		ImGui::End();
		return;
	}

	// Name And Layer
	{
		static char strTemp[50] = {};
		::sprintf_s(strTemp, "%s", _goPick->GetGUIName().data());

		bool active = _goPick->GetActive();
		ImGui::Checkbox(_goPick->GetGUIName().data(), &active);
		_goPick->SetActive(active);

		ImGui::SameLine();

		string layer = EnumToStr((LAYER_TYPE)_goPick->GetLayer());				// 매크로 사용
		::sprintf_s(strTemp, 50, "/ Layer : %s", layer.data());
		ImGui::Text(strTemp);
	}

	ImGuiIO& io = ImGui::GetIO();

	// Transform.
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::TRANSFORM) != nullptr) {
		TransformComponent();
	}
	
	// MeshRenderer
	// TODO : 아직 여러가지 매쉬들이 존재할때 따로 창으로 보여주는 작업은 만들지 못함.
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER) != nullptr) {
		MeshRendererComponent();
	}

	// Camera
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::CAMERA) != nullptr) {
		CameraComponent();
	}

	// Light
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::LIGHT) != nullptr) {
		LightComponent();
	}	
	
	// Particle
	if (_goPick->GetFixedComponent(COMPONENT_TYPE::PARTICLE_SYSTEM) != nullptr) {
		ParticleComponent();
	}


	// Constorm Scripts
	for (auto& script : _goPick->GetScripts()) {
		if(ImGui::CollapsingHeader(script.first.data())) {
			script.second->DrawGUI();
		}
	}

	ImGui::End();
}

void ToolScene::TestObjects()
{
	/* ----- 객체들이 뭐뭐 있는지 보여줍니다. ----- */
	ImGui::Begin("ObjectList");
	ImGui::SetWindowSize("ObjectList", ImVec2(200, 600));

	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::TreeNode("Tool Scene Object List")) {
		uint32 count = 0;
		
		// TODO : 미완 오른쪽 클릭하면 Popup창 나오게 하기
		if (io.MouseDown[1]) {
			// TODO : 이름을 뭘로 할지 고민해보자
			ImGui::OpenPopup("my_select_popup"); 

			if (ImGui::BeginPopup("my_select_popup")) {
				ImGui::Text("Create");
				
				if (ImGui::Selectable("cube")) {
					CreateCubeGameObject();
				}

				ImGui::EndPopup();
			}

		}

		// Editor Object Index Change
		vector<shared_ptr<GameObject>>& objects = GetAllGameObjects();
		for (int32 i = 0; i < objects.size(); ++i) {
			if (ImGui::Selectable(objects[i]->GetGUIName().data())) {
				_isPick = count;
				_goPick = objects[i];
			}

			// Editor Object Index Change
			if (ImGui::IsItemActive() && !ImGui::IsItemHovered()) {
				int32 i_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
				_dragPick = objects[i];			// 혹시나...?
				if (0 <= i_next && i_next < objects.size()) {
					::swap(objects[i], objects[i_next]);
					ImGui::ResetMouseDragDelta();
				}
			}
		}

		ImGui::TreePop();
	}

	{
		static char fpsStr[30] = {};
		::sprintf_s(fpsStr, 30, "Fps : %d", GET_SINGLE(Timer)->GetFps());
		ImGui::Text(fpsStr);
	}
	{
		if (_dragPick) {
			ImGui::Text(_dragPick->GetGUIName().data());
		}
	}
	{
		string guiName;
		if (_objPick) {
			guiName.assign(_objPick->GetName().begin(), _objPick->GetName().end());
			ImGui::Text(guiName.data());
		}
	}

	ImGui::End();
}

void ToolScene::TestResources()
{
	ImGui::Begin("Resources");

	string guiName;

	if (ImGui::TreeNode("Texture")) {
		for (const pair<wstring, shared_ptr<Object>>& tex : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::TEXTURE)) {
			guiName.assign(tex.first.begin(), tex.first.end());
			if (ImGui::Selectable(guiName.data())) {
				_objPick = tex.second;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shader")) {
		for (const pair<wstring, shared_ptr<Object>>& shader : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::SHADER)) {
			guiName.assign(shader.first.begin(), shader.first.end());
			if (ImGui::Selectable(guiName.data())) {
				_objPick = shader.second;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Material")) {
		for (const pair<wstring, shared_ptr<Object>>& material : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MATERIAL)) {
			guiName.assign(material.first.begin(), material.first.end());
			if (ImGui::Selectable(guiName.data())) {
				_objPick = material.second;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("GameObject")) {
		for (const pair<wstring, shared_ptr<Object>>& go : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::GAMEOBJECT)) {
			guiName.assign(go.first.begin(), go.first.end());
			if (ImGui::Selectable(guiName.data())) {
				_objPick = go.second;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Compoenent")) {
		for (const pair<wstring, shared_ptr<Object>>& component : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::COMPONENT)) {
			guiName.assign(component.first.begin(), component.first.end());
			if (ImGui::Selectable(guiName.data())) {
				_objPick = component.second;
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Mesh")) {
		for (const pair<wstring, shared_ptr<Object>>& mesh : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MESH)) {
			guiName.assign(mesh.first.begin(), mesh.first.end());
			if (ImGui::Selectable(guiName.data())) {
				_objPick = mesh.second;
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();
}

void ToolScene::TestRenderTargetView()
{
//	ImGui::Begin("RenderTargetView");
//
//	shared_ptr<RenderTargetGroup> renderTarget;
//
//	string guiName;
//	for (shared_ptr<GameObject>& obj : _renderTargetView) {
//		shared_ptr<Texture> tex = obj->GetMeshRenderer()->GetMaterial()->GetTexture(0);
//		ImGui::Image((ImTextureID)tex->GetSRV()->GetGPUDescriptorHandleForHeapStart().ptr , ImVec2(100.0f, 100.0f));
//		ImGui::SameLine();
//		ImGui::Image(tex, ImVec2(100.0f, 100.0f));
//	}
//
//	//renderTarget = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING);
//	//for (uint32 i = 0; i < RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT; ++i) {
//	//	shared_ptr<Texture> tex = renderTarget->GetRTTexture(i);
//	//	ImGui::ImageRTV(tex, ImVec2(100.0f, 100.0f));
//	//	guiName.assign(tex->GetName().begin(), tex->GetName().end());
//	//	ImGui::Text(guiName.data());
//	//}
//
//	ImGui::End();
}

// 계층구조때 쓸 녀석
//if (align_label_with_current_x_position)
//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
//
//// 'selection_mask' is dumb representation of what may be user-side selection state.
////  You may retain selection state inside or outside your objects in whatever format you see fit.
//// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
///// of the loop. May be a pointer to your own node type, etc.
//static int selection_mask = (1 << 2);
//int node_clicked = -1;
//for (int i = 0; i < 6; i++)
//{
//	// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
//	// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
//	ImGuiTreeNodeFlags node_flags = base_flags;
//	const bool is_selected = (selection_mask & (1 << i)) != 0;
//	if (is_selected)
//		node_flags |= ImGuiTreeNodeFlags_Selected;
//	if (i < 3)
//	{
//		// Items 0..2 are Tree Node
//		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
//		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
//			node_clicked = i;
//		if (test_drag_and_drop && ImGui::BeginDragDropSource())
//		{
//			ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
//			ImGui::Text("This is a drag and drop source");
//			ImGui::EndDragDropSource();
//		}
//		if (node_open)
//		{
//			ImGui::BulletText("Blah blah\nBlah Blah");
//			ImGui::TreePop();
//		}
//	}
//	else
//	{
//		// Items 3..5 are Tree Leaves
//		// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
//		// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
//		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
//		ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
//		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
//			node_clicked = i;
//		if (test_drag_and_drop && ImGui::BeginDragDropSource())
//		{
//			ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
//			ImGui::Text("This is a drag and drop source");
//			ImGui::EndDragDropSource();
//		}
//	}
//}
//if (node_clicked != -1)
//{
//	// Update selection state
//	// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
//	if (ImGui::GetIO().KeyCtrl)
//		selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
//	else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
//		selection_mask = (1 << node_clicked);           // Click to single-select
//}
//if (align_label_with_current_x_position)
//ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
//ImGui::TreePop();
//		}

void ToolScene::TestButtonCreateObject()
{
	ImGui::Begin("TestButton");

	if (ImGui::Button("CreateButton")) {		
		CreateCubeGameObject();
	}
	if (ImGui::Button("RemoveButton")) {
		RemoveSelectObject();
	}

	ImGui::End();
}

void ToolScene::TransformComponent()
{
	if (ImGui::CollapsingHeader("Transform")) {
		const float dragSpeed = 1.0f;
		Vec3 temp;

		temp = _goPick->GetTransform()->GetLocalPosition();
		float position[3] = { temp.x, temp.y, temp.z };

		ImGui::DragFloat3("Position", position, dragSpeed, -10000.0f, 10000.0f);

		_goPick->GetTransform()->SetLocalPosition(Vec3(position[0], position[1], position[2]));


		temp = _goPick->GetTransform()->GetLocalRotation();
		float rotation[3] = { temp.x,temp.y,temp.z };

		ImGui::DragFloat3("Rotation", rotation, dragSpeed / 36, -10000.0f, 10000.0f);

		_goPick->GetTransform()->SetLocalRotation(Vec3(rotation[0], rotation[1], rotation[2]));


		temp = _goPick->GetTransform()->GetLocalScale();
		float scale[3] = { temp.x,temp.y,temp.z };

		ImGui::DragFloat3("Scale", scale, dragSpeed, -10000.0f, 10000.0f);

		_goPick->GetTransform()->SetLocalScale(Vec3(scale[0], scale[1], scale[2]));
	}
}

void ToolScene::MeshRendererComponent()
{
	shared_ptr<MeshRenderer> renderer = _goPick->GetMeshRenderer();
	shared_ptr<Material> material = _goPick->GetMeshRenderer()->GetMaterial();
	string guiName;

	if (ImGui::CollapsingHeader("MeshRenderer")) {
		ImGui::Text("Current Mesh : ");
		ImGui::SameLine();
		guiName.assign(renderer->GetMesh()->GetName().begin(), renderer->GetMesh()->GetName().end());
		if (ImGui::Selectable(guiName.data())) {

		}

		/* ----- Shader ----- */
		if (renderer->GetMaterial()->GetShader() == nullptr) {
			return;
		}
		ImGui::Text("Current Shader : ");
		ImGui::SameLine();
		guiName.assign(material->GetShader()->GetName().begin(), material->GetShader()->GetName().end());
		if (ImGui::Selectable(guiName.data())) {

		}

		/* ----- Texture ----- */
		if (renderer->GetMaterial()->GetShader() == nullptr) {
			return;
		}

		for (uint32 i = 0; i < MATERIAL_TEXTURE_COUNT; ++i) {
			if (material->GetTexture(i) == nullptr) {
				continue;
			}

			guiName = "Current Texture" + std::to_string(i) + " : ";
			ImGui::Image(renderer->GetMaterial()->GetTexture(i), ImVec2(50.0f, 50.0f));
			ImGui::SameLine();
			ImGui::Text(guiName.data());
			ImGui::SameLine();
			guiName.assign(material->GetTexture(i)->GetName().begin(), material->GetTexture(i)->GetName().end());
			if (ImGui::Selectable(guiName.data())) {

			}
		}
	}




}

void ToolScene::CameraComponent()
{
	if (ImGui::CollapsingHeader("Camera")) {
		// TODO : 직교랑 절두랑 분기를 시켜줘야한다.
		float cameraNear = _goPick->GetCamera()->GetNear();
		float cameraFar = _goPick->GetCamera()->GetFar();
		float cameraFov = _goPick->GetCamera()->GetFov();

		ImGui::SliderFloat("Near", &cameraNear, 0.001f, cameraFar - 1);
		ImGui::SliderFloat("Far", &cameraFar, cameraNear + 1, 10000.0f);
		ImGui::SliderFloat("Fov", &cameraFov, 0.0f, XM_PI);

		_goPick->GetCamera()->SetNear(cameraNear);
		_goPick->GetCamera()->SetFar(cameraFar);
		_goPick->GetCamera()->SetFov(cameraFov);
	}
}

void ToolScene::LightComponent()
{
	if (ImGui::CollapsingHeader("Light")) {
		static float floatTemp[3] = {};
		shared_ptr<Light> light = _goPick->GetLight();
		Vec4 vecTemp;

		/* ----- BaseLight ----- */
		if (ImGui::TreeNode("BaseLight")) {
			vecTemp = light->GetDiffuse();


			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::ColorEdit3("Diffuse", floatTemp);
			light->SetDiffuse(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			vecTemp = light->GetAmbient();
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::ColorEdit3("Ambient", floatTemp);
			light->SetAmbient(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			vecTemp = light->GetSpecular();
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::ColorEdit3("Specular", floatTemp);
			light->SetSpecular(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			ImGui::TreePop();
		}

		ImGui::Separator();

		/* ----- LightInfo ----- */
		if (ImGui::TreeNode("LightInfo")) {

			LightInfo info = _goPick->GetLight()->GetLightInfo();

			const float dragSpeed = (ImGui::GetIO().KeyShift) ? 0.5f : 0.05f;

			vecTemp = info.direction;
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::DragFloat3("DirectionLight", floatTemp, dragSpeed, -2 * XM_PI, 2 * XM_PI);
			light->SetLightDirection(Vec3(floatTemp[0], floatTemp[1], floatTemp[2]));

			floatTemp[0] = 0.0f;
			if (info.lightType == static_cast<int32>(LIGHT_TYPE::DIRECTIONAL_LIGHT)) {

			}
			else if (info.lightType == static_cast<int32>(LIGHT_TYPE::POINT_LIGHT)) {
				floatTemp[0] = info.range;
				ImGui::DragFloat("Range", &floatTemp[0], dragSpeed, 0.0f, 10000.0f);
				light->SetLightRange(floatTemp[0]);

			}
			else if (info.lightType == static_cast<int32>(LIGHT_TYPE::SPOT_LIGHT)) {
				floatTemp[0] = info.angle;
				ImGui::DragFloat("Angle", &floatTemp[0], XM_PI / 360, 0.0f, XM_PI * 2);
				light->SetLightAngle(floatTemp[0]);

				floatTemp[1] = info.range;
				ImGui::DragFloat("Range", &floatTemp[1], dragSpeed, 0.0f, 10000.0f);
				light->SetLightRange(floatTemp[1]);
			}


			ImGui::TreePop();
		}
	}
}

void ToolScene::ParticleComponent()
{
	shared_ptr<ParticleSystem> particle = _goPick->GetParticleSystem();
	if (ImGui::CollapsingHeader("ParticleSystem")) {
		float fTemp[4] = {};
		int iTemp[4] = {};

		iTemp[0] = particle->GetMaxParticleCount();
		ImGui::DragInt("ParticleCount", &iTemp[0], 1, 0, 10000);
		particle->SetMaxParticleCount(iTemp[0]);

		fTemp[0] = particle->GetMinLifeTime();
		fTemp[1] = particle->GetMaxLifeTime();
		ImGui::DragFloat2("ParticleLifeTime", fTemp, 0.1f, 0.0f, 1000.0f);
		particle->SetLifeTime(Vec2(fTemp[0], fTemp[1]));

		fTemp[0] = particle->GetMinSpeed();
		fTemp[1] = particle->GetMaxSpeed();
		ImGui::DragFloat2("ParticleSpeed", fTemp, 0.1f, 0.0f, 1000.0f);
		particle->SetSpeed(Vec2(fTemp[0], fTemp[1]));

		fTemp[0] = particle->GetStartScale();
		fTemp[1] = particle->GetEndScale();
		ImGui::DragFloat2("ParticleScale", fTemp, 0.1f, 0.0f, 1000.0f);
		particle->SetScale(Vec2(fTemp[0], fTemp[1]));

		shared_ptr<Texture> tex = particle->GetTexture();
		string strName;
		strName.assign(tex->GetName().begin(), tex->GetName().end());
		ImGui::Image(tex, ImVec2(50.0f, 50.0f));
		ImGui::Selectable(strName.data());
	}
}


void ToolScene::RemoveSelectObject()
{
	if (_isPick == -1) {
		return;
	}

	if (_goPick != nullptr) {
		RemoveObject(_goPick);
		_isPick = -1;
		_goPick = nullptr;
	}
}

void ToolScene::CreateCubeGameObject()
{
	shared_ptr<GameObject> go = make_shared<GameObject>();
	go->Init();
	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();

	shared_ptr<Material> material = make_shared<Material>();
	material->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"Deferred"));

	shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
	renderer->SetMaterial(material);
	renderer->SetMesh(mesh);

	go->AddComponent(make_shared<Transform>());
	go->AddComponent(renderer);

	go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	go->GetTransform()->SetLocalPosition(Vec3(_random.Rand(-100.0f, 100.0f), _random.Rand(-100.0f, 100.0f), _random.Rand(-100.0f, 100.0f)));

	AddGameObject(go);
}
