#include "pch.h"
#include "RotationXComponent.h"
#include "Transform.h"
#include "Timer.h"

Random RotationXComponent::random;

RotationXComponent::RotationXComponent()
{
}

RotationXComponent::~RotationXComponent()
{
}

void RotationXComponent::Awake()
{
	random.Init();
}

void RotationXComponent::Start()
{
}

void RotationXComponent::Update()
{
	GetTransform()->SetLocalRotation(GetTransform()->GetLocalRotation() + Vec3(_speedX, _speedY, _speedZ) * DELTATIME);

}

void RotationXComponent::LateUpdate()
{
}

void RotationXComponent::DrawGUI()
{
	ImGui::DragFloat("SpeedX", &_speedX, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("SpeedY", &_speedY, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("SpeedZ", &_speedZ, 0.1f, 0.0f, 10.0f);
}
