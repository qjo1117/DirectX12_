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

	S_MatView = _matView;
	S_MatProjection = _matProjection;
}

void Camera::Render()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetCurrentScene();

	// Auto�� �ϴ� ���� : ����ϱ⿡�� �ʹ� ���� ��
	auto sceneObjects = scene->GetGameObjects();

	/* ----- ù��°�� ���̾ ��ȸ�� �Ѵ�, �ι�°�� ���̾�ȿ� GameObject�� ��ȸ�Ѵ�. ----- */
	for (auto& layerObjects : sceneObjects) {
		// �������� ������ �༮�� ��� �������Ѵ�.
		for (auto& gameObject : layerObjects) {
			if (gameObject->GetMeshRenderer() == nullptr) {
				continue;
			}

			gameObject->GetMeshRenderer()->Render();
		}
	}
}
