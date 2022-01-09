#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
<<<<<<< Updated upstream

shared_ptr<GameObject> gameObject = make_shared<GameObject>();
=======
#include "StartScene.h"
#include "ToolScene.h"
>>>>>>> Stashed changes

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);

	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.3f, 0.3f, 0.2f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.0f, 0.0f);
	vec[1].pos = Vec3(0.3f, 0.3f, 0.2f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.0f, 0.0f);
	vec[2].pos = Vec3(0.3f, -0.3f, 0.2f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.0f, 1.0f);
	vec[3].pos = Vec3(-0.3f, -0.3f, 0.2f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.0f, 1.0f);

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}


	gameObject->Init();

	shared_ptr<MeshRenderer> renderer = make_shared<MeshRenderer>();

	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shared_ptr<Material> material = make_shared<Material>();

		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\Picture.jpg");

<<<<<<< Updated upstream
		material->SetShader(shader);
		material->SetTexture(0, texture);
		material->SetFloat(0, 0.3f);
		material->SetFloat(1, -0.3f);
		material->SetFloat(2, 0.5f);

		mesh->Init(vec, indexVec);

		renderer->SetMesh(mesh);
		renderer->SetMaterial(material);
	}
	gameObject->AddComponent(renderer);

	GEngine->GetCmdQueue()->WaitSync();
=======
	GET_SINGLE(SceneManager)->LoadScene(SCENE_TYPE::TOOL);
>>>>>>> Stashed changes
}

void Game::Update()
{
	GEngine->Update();

	GEngine->RenderBegin();

	gameObject->Update();

	GEngine->RenderEnd();
}

void Game::End()
{
	GEngine->End();
}
