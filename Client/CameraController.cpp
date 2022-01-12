#include "pch.h"
#include "CameraController.h"
#include "Engine.h"
#include "Transform.h"

CameraController::CameraController()
{
}

CameraController::~CameraController()
{
}

void CameraController::Awake()
{
}

void CameraController::Start()
{
}

void CameraController::Update()
{
	if (_target == nullptr) {
		CameraMove();
	}
	else {
		TargetFollow();
	}

}

void CameraController::LateUpdate()
{
}

void CameraController::DrawGUI()
{
	ImGui::DragFloat("Speed", &_speed, 1.0f, 0.0f, 1000.0f);
	ImGui::DragFloat3("Offset", reinterpret_cast<float*>(&_offset), 1.0f, -1000.0f, 1000.0f);
}

void CameraController::TargetFollow()
{
	Vec3 offset = _target->GetTransform()->GetLocalPosition() + _offset;
	GetTransform()->SetLocalPosition(offset);
}

void CameraController::CameraMove()
{
	Vec3 move;

	if (INPUT->GetButton(KEY_TYPE::A)) {
		move += -GetTransform()->GetRight();
	}
	if (INPUT->GetButton(KEY_TYPE::D)) {
		move += GetTransform()->GetRight();
	}
	if (INPUT->GetButton(KEY_TYPE::W)) {
		move += GetTransform()->GetForward();
	}
	if (INPUT->GetButton(KEY_TYPE::S)) {
		move += -GetTransform()->GetForward();
	}
	if (INPUT->GetButton(KEY_TYPE::Q)) {
		move += GetTransform()->GetUp();
	}
	if (INPUT->GetButton(KEY_TYPE::E)) {
		move += -GetTransform()->GetUp();
	}
	move = move * _speed * DELTATIME;

	if (INPUT->GetButton(KEY_TYPE::SHIFT)) {
		move *= 5.0f;
	}

	GetTransform()->MovePosition(move);

	move = Vec3(0.0f, 0.0f, 0.0f);
	if (INPUT->GetButton(KEY_TYPE::Z)) {
		move.x -= 1;
	}
	if (INPUT->GetButton(KEY_TYPE::C)) {
		move.x += 1;
	}
	move.x *= DELTATIME;
	move += GetTransform()->GetLocalRotation();
	GetTransform()->SetLocalRotation(move);
}
