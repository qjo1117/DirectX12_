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
#include "Light.h"
#include "Utils.h"
#include "Billboard.h"

ToolScene::ToolScene()
{

#pragma region Player
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName("Player");

		shared_ptr<Material> material = make_shared<Material>();

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\wirefram.hlsli",
			{ RASTERIZER_TYPE::WIREFRAM, DEPTH_STENCIL_TYPE::LESS });
		material->SetShader(shader);

		// BaseColor
		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Init(L"..\\Resources\\Texture\\Stylized.jpg");
		material->SetTexture(0, texture);
		// Normal
		shared_ptr<Texture> textureNormal = make_shared<Texture>();
		textureNormal->Init(L"..\\Resources\\Texture\\Stylized_Normal.jpg");
		material->SetTexture(1, textureNormal);

		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();

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

#pragma region Plan
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->Init();
		go->SetGUIName("Plan");

		shared_ptr<Material> material = make_shared<Material>();

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		material->SetShader(shader);

		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
		// BaseColor
		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Init(L"..\\Resources\\Texture\\Metal.jpg");
		material->SetTexture(0, texture);
		// Normal
		shared_ptr<Texture> textureNormal = make_shared<Texture>();
		textureNormal->Init(L"..\\Resources\\Texture\\Metal_Normal.jpg");
		material->SetTexture(1, textureNormal);

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		go->GetTransform()->SetLocalScale(Vec3(1000.0f, 1.0f, 1000.0f));
		go->GetTransform()->SetLocalPosition(Vec3(0.0f, -100.0f, 0.0f));

		go->AddComponent(renderer);
		AddGameObject(go, LAYER_TYPE::DEFAULT);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadCubeMesh());
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();

			shader->Init(L"..\\Resources\\Shader\\skybox.hlsli",
				{ RASTERIZER_TYPE::CULL_NONE, DEPTH_STENCIL_TYPE::LESS_EQUAL });
			texture->Init(L"..\\Resources\\Texture\\SkyBox_1.jpg");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		skybox->SetCheckFrustum(false);
		AddGameObject(skybox, LAYER_TYPE::SKYBOX);

	}
#pragma endregion

#pragma region Camera
	{
		shared_ptr<GameObject> go = make_shared<GameObject>();
		go->AddComponent(make_shared<Transform>());
		go->AddComponent(make_shared<Camera>());
		go->AddComponent(make_shared<CameraController>());

		go->SetName(L"MainCamera");
		go->SetGUIName("MainCamera");
		_camera = go;
		_camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.f, 0.0f));
		AddGameObject(_camera, LAYER_TYPE::CAMERA);
	}
#pragma endregion

#pragma region White Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetGUIName("Directional Light");
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalScale(Vec3(20.0f, 20.0f, 20.0f));

		shared_ptr<Material> material = make_shared<Material>();

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->Init(L"..\\Resources\\Shader\\SimpleTexture.hlsli");
		material->SetShader(shader);

		shared_ptr<Texture> texture = make_shared<Texture>();
		texture->Init(L"..\\Resources\\Texture\\Light.png");
		material->SetTexture(0, texture);
		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadQuadMesh();

		shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);

		light->AddComponent(renderer);

		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));

		light->AddComponent(make_shared<Billboard>());

		AddGameObject(light);
	}

#pragma endregion

//#pragma region Red Point Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->SetGUIName("Point Light");
//
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(150.f, 150.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		//light->GetLight()->SetLightDirection(Vec3(0.f, -1.f, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(1.f, 0.1f, 0.1f));
//		light->GetLight()->SetAmbient(Vec3(0.1f, 0.f, 0.f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetLightRange(10000.f);
//		//light->GetLight()->SetLightAngle(XM_PI / 4);
//		AddGameObject(light);
//	}
//#pragma endregion
//
//#pragma region Blue Spot Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->SetGUIName("Spot Light");
//
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(-150.f, 0.f, 150.f));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(1.f, 0.f, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(0.f, 0.1f, 1.f));
//		//light->GetLight()->SetAmbient(Vec3(0.f, 0.f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetLightRange(10000.f);
//		light->GetLight()->SetLightAngle(XM_PI / 4);
//		AddGameObject(light);
//	}
//#pragma endregion

	GEngine->GetCmdQueue()->WaitSync();

	GET_SINGLE(Resources)->LoadQuadMesh();

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
		::sprintf_s(strTemp, "%s", _pick->GetGUIName().data());

		bool active = _pick->GetActive();
		ImGui::Checkbox(_pick->GetGUIName().data(), &active);
		_pick->SetActive(active);

		ImGui::SameLine();

		//auto layer = magic_enum::enum_name((LAYER_TYPE)_pick->GetLayer());	// magicLib사용
		string layer = EnumToStr((LAYER_TYPE)_pick->GetLayer());				// 매크로 사용
		::sprintf_s(strTemp, 50, "/ Layer : %s", layer.data());
		ImGui::Text(strTemp);
	}

	ImGuiIO& io = ImGui::GetIO();

	// Transform.
	if (_pick->GetFixedComponent(COMPONENT_TYPE::TRANSFORM) != nullptr) {
		TransformComponent();
	}
	
	// MeshRenderer
	// TODO : 아직 여러가지 매쉬들이 존재할때 따로 창으로 보여주는 작업은 만들지 못함.
	if (_pick->GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER) != nullptr) {
		MeshRendererComponent();
	}

	// Camera
	if (_pick->GetFixedComponent(COMPONENT_TYPE::CAMERA) != nullptr) {
		CameraComponent();
	}

	// Light
	if (_pick->GetFixedComponent(COMPONENT_TYPE::LIGHT) != nullptr) {
		LightComponent();
	}


	// Constorm Scripts
	for (auto& script : _pick->GetScripts()) {
		if(ImGui::CollapsingHeader(script.first.data())) {
			script.second->DrawGUI();
		}
	}

	//// Test Image
	//{
	//	if (_pick->GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER) == nullptr) {
	//		ImGui::End();
	//		return;
	//	}
	//	D3D12_GPU_DESCRIPTOR_HANDLE gpuPtr = GEngine->GetTableDescHeap()->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	//	ImGui::Text("GPU handle = %p", gpuPtr.ptr);
	//	ImGui::Image((void*)((intptr_t)gpuPtr.ptr), ImVec2(100.0f, 100.0f), ImVec2(0.0f, 0.0f), ImVec2(0.5f, 1.0f));
	//}
	//
	// 이 코드의 문제점. GUI Fonts GetTexID만 보여주고 있음.

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
				_pick = objects[i];
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

	ImGui::End();
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

		temp = _pick->GetTransform()->GetLocalPosition();
		float position[3] = { temp.x, temp.y, temp.z };

		ImGui::DragFloat3("Position", position, dragSpeed, -10000.0f, 10000.0f);

		_pick->GetTransform()->SetLocalPosition(Vec3(position[0], position[1], position[2]));


		temp = _pick->GetTransform()->GetLocalRotation();
		float rotation[3] = { temp.x,temp.y,temp.z };

		ImGui::DragFloat3("Rotation", rotation, dragSpeed / 36, -10000.0f, 10000.0f);

		_pick->GetTransform()->SetLocalRotation(Vec3(rotation[0], rotation[1], rotation[2]));


		temp = _pick->GetTransform()->GetLocalScale();
		float scale[3] = { temp.x,temp.y,temp.z };

		ImGui::DragFloat3("Scale", scale, dragSpeed, -10000.0f, 10000.0f);

		_pick->GetTransform()->SetLocalScale(Vec3(scale[0], scale[1], scale[2]));
	}
}

void ToolScene::MeshRendererComponent()
{
	if (ImGui::CollapsingHeader("MeshRenderer")) {
		for (const pair<wstring, shared_ptr<Object>>& mesh : GET_SINGLE(Resources)->GetResources(OBJECT_TYPE::MESH)) {
			string name;
			name.assign(mesh.first.begin(), mesh.first.end());
			if (ImGui::Button(name.c_str())) {
				_pick->GetMeshRenderer()->SetMesh(static_pointer_cast<Mesh>(mesh.second));
			}
		}
	}
}

void ToolScene::CameraComponent()
{
	if (ImGui::CollapsingHeader("Camera")) {
		float cameraNear = _pick->GetCamera()->GetNear();
		float cameraFar = _pick->GetCamera()->GetFar();

		ImGui::SliderFloat("Near", &cameraNear, 0.1f, cameraFar - 1);
		ImGui::SliderFloat("Far", &cameraFar, cameraNear + 1, 10000.0f);

		_pick->GetCamera()->SetNear(cameraNear);
		_pick->GetCamera()->SetFar(cameraFar);
	}
}

void ToolScene::LightComponent()
{
	if (ImGui::CollapsingHeader("Light")) {
		static float floatTemp[3] = {};
		shared_ptr<Light> light = _pick->GetLight();
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

			LightInfo info = _pick->GetLight()->GetLightInfo();

			const float dragSpeed = (ImGui::GetIO().KeyShift) ? 0.5f : 0.05f;

			vecTemp = info.direction;
			floatTemp[0] = vecTemp.x; floatTemp[1] = vecTemp.y; floatTemp[2] = vecTemp.z;
			ImGui::DragFloat3("DirectionLight", floatTemp, dragSpeed, -2 *XM_PI, 2 * XM_PI);
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
				ImGui::DragFloat("Angle", &floatTemp[0], XM_PI / 360, 0.0f, XM_PI);
				light->SetLightAngle(floatTemp[0]);

				floatTemp[1] = info.range;
				ImGui::DragFloat("Range", &floatTemp[1], dragSpeed, 0.0f, 10000.0f);
				light->SetLightRange(floatTemp[1]);
			}


			ImGui::TreePop();
		}
	}
}

void ToolScene::RemoveSelectObject()
{
	if (_isPick == -1) {
		return;
	}

	if (_pick != nullptr) {
		RemoveObject(_pick);
		_isPick = -1;
		_pick == nullptr;
	}
}

void ToolScene::CreateCubeGameObject()
{
	shared_ptr<GameObject> go = make_shared<GameObject>();
	go->Init();
	shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	shared_ptr<Material> material = make_shared<Material>();
	material->SetShader(shader);

	shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();
	renderer->SetMaterial(material);
	renderer->SetMesh(mesh);

	go->AddComponent(make_shared<Transform>());
	go->AddComponent(renderer);

	go->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
	go->GetTransform()->SetLocalPosition(Vec3(_random.Rand(-100.0f, 100.0f), _random.Rand(-100.0f, 100.0f), _random.Rand(-100.0f, 100.0f)));

	AddGameObject(go);
}
