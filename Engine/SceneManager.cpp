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
	if (_currentScene == nullptr) {
		return;
	}

	auto& sceneObjects = _currentScene->GetGameObjects();
	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (auto& layerObjects : sceneObjects) {
		// 렌더링이 가능한 녀석을 골라서 렌더링한다.q
		for (auto& gameObject : layerObjects) {
			if (gameObject->GetCamera() == nullptr) {
				continue;
			}

			gameObject->GetCamera()->Render();
		}
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