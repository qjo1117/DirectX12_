#include "pch.h"
#include "Billboard.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

Billboard::Billboard()
{
}

Billboard::~Billboard()
{
}

void Billboard::Awake()
{
	if (_scene == nullptr) {
		_scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	}
	// Camera Index�� ù��° �ִ� ������Ʈ�� �����̶�� �����Ѵ�.
	_camera = _scene->GetGameObjects(LAYER_TYPE::CAMERA)[0];


}

void Billboard::Start()
{
	
}

void Billboard::Update()
{
	if (_camera) {

	}

}

void Billboard::LateUpdate()
{
}

void Billboard::DrawGUI()
{
}
