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

	_frustum.FinalUpdate();
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetAllGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();

	for (const shared_ptr<GameObject>& go : gameObjects) {
		if (go->GetMeshRenderer() == nullptr) {
			continue;
		}
		if (IsCulled(go->GetLayer())) {
			continue;
		}

		if (go->GetCheckFrustum()) {
			if (_frustum.ContainSphere(
				go->GetTransform()->GetWorldPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}

		SHADER_TYPE shaderType = go->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
		switch(shaderType) {
		case SHADER_TYPE::DEFERRED:
			_vecDeferred.push_back(go);
			break;
		case SHADER_TYPE::FORWARD:
			_vecForward.push_back(go);
			break;
		}
	}

}

void Camera::Render_Deferred()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (const shared_ptr<GameObject>& go : _vecDeferred) {
		go->GetMeshRenderer()->Render();
	}
}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (const shared_ptr<GameObject>& go : _vecForward) {
		go->GetMeshRenderer()->Render();
	}
}
