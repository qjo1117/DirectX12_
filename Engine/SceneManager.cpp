#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"

void SceneManager::Update()
{
	if (_currentScene == nullptr) {
		return;
	}
	_currentScene->Update();
	_currentScene->LateUpdate();
	_currentScene->FinalUpdate();
}

void SceneManager::Render()
{
	if (_currentScene) {
		_currentScene->Render();
	}

}

void SceneManager::LoadScene(SCENE_TYPE type)
{
	/* ----- Scene이 존재하는지 체크하고 있으면 씬전환을 한다. ----- */
	assert(_scenes[static_cast<uint32>(type)]);

	if (_currentScene == nullptr) {
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	else {
		// TODO : 내용물 초기화
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	_type = type;
	_currentScene->Awake();
	_currentScene->Start();
}