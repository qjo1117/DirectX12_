#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneManager.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	// 상세설명 : view는 꺼꾸로 봐야지 맞다.
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE) {
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	}
	else {
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);
	}

	S_MatView = _matView;
	S_MatProjection = _matProjection;
}

void Camera::Render()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();

	// Auto를 하는 이유 : 명시하기에는 너무 강한 적
	auto sceneObjects = scene->GetGameObjects();

	/* ----- 첫번째는 레이어를 순회를 한다, 두번째는 레이어안에 GameObject를 순회한다. ----- */
	for (auto& layerObjects : sceneObjects) {
		// 렌더링이 가능한 녀석을 골라서 렌더링한다.
		for (auto& gameObject : layerObjects) {
			if (gameObject->GetMeshRenderer() == nullptr) {
				continue;
			}

			gameObject->GetMeshRenderer()->Render();
		}
	}
}
