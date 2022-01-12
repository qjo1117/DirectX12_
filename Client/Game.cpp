#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "StartScene.h"
#include "ToolScene.h"

void Game::Init(const WindowInfo& info)
{
	_info = info;
	GEngine->Init(info);

	GET_SINGLE(SceneManager)->CreateScene<ToolScene>(SCENE_TYPE::TOOL);
	GET_SINGLE(SceneManager)->CreateScene<StartScene>(SCENE_TYPE::START);

	GET_SINGLE(SceneManager)->LoadScene(SCENE_TYPE::TOOL);
}

void Game::Update()
{
	GEngine->Update();
}

void Game::End()
{
	GEngine->End();
}
