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
	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
	for (auto& layerObjects : sceneObjects) {
		// �������� ������ �༮�� ��� �������Ѵ�.q
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
	/* ----- Scene�� �����ϴ��� üũ�ϰ� ������ ����ȯ�� �Ѵ�. ----- */
	assert(_scenes[static_cast<uint32>(type)]);

	if (_currentScene == nullptr) {
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	else {
		// TODO : ���빰 �ʱ�ȭ
		_currentScene = _scenes[static_cast<uint32>(type)];
	}
	_type = type;
	_currentScene->Awake();
	_currentScene->Start();
}