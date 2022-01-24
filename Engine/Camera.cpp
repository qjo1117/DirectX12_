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
	// �󼼼��� : view�� ���ٷ� ������ �´�.
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE) {
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	}
	else {
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);
	}

	// ���� : �������ҿ��� Camera�� ������� �޴µ� Render_Forward������ ����� ��Ȳ�̱⶧���� ī�޶��� ������ ����������Ѵ�.
	S_MatProjection = _matProjection;
	S_MatView = _matView;

	_frustum.FinalUpdate();
}

void Camera::SortGameObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetAllGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();
	_vecParticle.clear();

	for (const shared_ptr<GameObject>& go : gameObjects) {
		if (go->GetMeshRenderer() == nullptr && go->GetParticleSystem() == nullptr) {
			continue;
		}
		if (IsCulled(go->GetLayer())) {
			continue;
		}
		if (go->GetActive() == false) {
			continue;
		}

		if (go->GetCheckFrustum()) {
			if (_frustum.ContainSphere(
				go->GetTransform()->GetWorldPosition(),
				go->GetTransform()->GetBoundingSphereRadius()) == false) {
				continue;
			}
		}

		// MeshRenderer�� ������쿡�� ������ ���ش�.
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

void Camera::Render_Deferred()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	// �׳� �׸������� �����ش�.
	GET_SINGLE(InstancingManager)->Render(_vecDeferred);
}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	GET_SINGLE(InstancingManager)->Render(_vecForward);

	for (const shared_ptr<GameObject>& go : _vecParticle) {
		go->GetParticleSystem()->Render();
	}
}
