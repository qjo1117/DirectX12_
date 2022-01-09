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
	Vec3 move;

	if (INPUT->GetButton(KEY_TYPE::A)) {
		move.x -= 1;
	}
	if (INPUT->GetButton(KEY_TYPE::D)) {
		move.x += 1;
	}
	if (INPUT->GetButton(KEY_TYPE::W)) {
		move.z += 1;
	}
	if (INPUT->GetButton(KEY_TYPE::S)) {
		move.z -= 1;
	}
	if (INPUT->GetButton(KEY_TYPE::Q)) {
		move.y += 1;
	}
	if (INPUT->GetButton(KEY_TYPE::E)) {
		move.y -= 1;
	}
	move = move * 100.f * DELTATIME;
	GetTransform()->MovePosition(move);
}

void CameraController::LateUpdate()
{
}

void CameraController::DrawGUI()
{
}
