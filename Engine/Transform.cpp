#include "pch.h"
#include "Transform.h"
#include "Component.h"
#include "Camera.h"
#include "Engine.h"


Transform::Transform() : 
	Component(COMPONENT_TYPE::TRANSFORM)
{
}

Transform::~Transform()
{
}

void Transform::FinalUpdate()
{
	/* ----- 현재 기본적인 Scale, Rotation, Translation Matrix 셋팅후 SRT 순서로 합친다. ----- */
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	_matLocal = matScale * matRotation * matTranslation;
	_matWorld = _matLocal;

	/* ----- 부모가 있을경우 계층구조를 이용해준다. ----- */
	shared_ptr<Transform> parent = GetParent().lock();
	if (parent != nullptr) {
		_matWorld *= parent->GetLocalToWorldMatrix();
	}
}

void Transform::PushData()
{
	Matrix matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&matWVP, sizeof(matWVP));
}
