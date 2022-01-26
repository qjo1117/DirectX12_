#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "ParticleSystem.h"
#include "InstancingManager.h"
#include "SceneManager.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
	_vecDeferred.reserve(1000);
	_vecForward.reserve(1000);
	_vecParticle.reserve(1000);
	_vecShadow.reserve(1000);

	_width = static_cast<float>(GEngine->GetWindow().width);
	_height = static_cast<float>(GEngine->GetWindow().height);

}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	// 상세설명 : view는 꺼꾸로 봐야지 맞다.
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	if (_type == PROJECTION_TYPE::PERSPECTIVE) {
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
	}
	else {
		_matProjection = ::XMMatrixOrthographicLH(_width * _scale, _height * _scale, _near, _far);
	}

	_frustum.FinalUpdate();
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetAllGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();
	_vecParticle.clear();

	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (const shared_ptr<GameObject>& go : gameObjects) {
		if (IsCulled(go->GetLayer())) {
			continue;
		}
		if (go->GetActive() == false) {
			continue;
		}

		if (go->GetCheckFrustum()) {
			if (_frustum.ContainSphere(
				go->GetTransform()->GetLocalPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}
		if (go->GetMeshRenderer() == nullptr && go->GetParticleSystem() == nullptr) {
			continue;
		}

		// MeshRenderer가 있을경우에만 실행을 해준다.
		if (go->GetMeshRenderer()) {
			SHADER_TYPE shaderType = go->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (shaderType) {
			case SHADER_TYPE::DEFERRED:
				_vecDeferred.push_back(go);
				break;
			case SHADER_TYPE::FORWARD:
				_vecForward.push_back(go);
				break;
			}
		}
		else {
			_vecParticle.push_back(go);
		}
	}

}

void Camera::SortShadowGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetAllGameObjects();

	_vecShadow.clear();

	S_MatView = _matView;
	S_MatProjection = _matProjection;

	for (const shared_ptr<GameObject>& go : gameObjects) {
		if (go->IsStatic()) {
			continue;
		}
		if (IsCulled(go->GetLayer())) {
			continue;
		}
		if (go->GetCheckFrustum()) {
			if (_frustum.ContainSphere(
				go->GetTransform()->GetLocalPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}
		if (go->GetMeshRenderer() == nullptr) {
			continue;
		}

		_vecShadow.push_back(go);
	}
}

void Camera::Render_Deferred()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	// 그냥 그리지말고 나눠준다.
	GET_SINGLE(InstancingManager)->Render(_vecDeferred);
}

void Camera::Render_Forward()
{
	GET_SINGLE(InstancingManager)->Render(_vecForward);

	for (const shared_ptr<GameObject>& go : _vecParticle) {
		go->GetParticleSystem()->Render();
	}
}

void Camera::Render_Shadow()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;
	for (const shared_ptr<GameObject>& go : _vecShadow) {
		go->GetMeshRenderer()->RenderShadow();
	}

}
