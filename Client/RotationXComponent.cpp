#include "pch.h"
#include "RotationXComponent.h"
#include "Transform.h"
#include "Timer.h"

RotationXComponent::RotationXComponent()
{
}

RotationXComponent::~RotationXComponent()
{
}

void RotationXComponent::Awake()
{
}

void RotationXComponent::Start()
{
}

void RotationXComponent::Update()
{
	GetTransform()->SetLocalRotation(GetTransform()->GetLocalRotation() + Vec3(0.0f, 0.0f, _speed) * DELTATIME);
}

void RotationXComponent::LateUpdate()
{
}

void RotationXComponent::DrawGUI()
{
	if (ImGui::CollapsingHeader("RotationXComponent")) {
		ImGui::DragFloat("Speed", &_speed, 0.1f, 0.0f, 10.0f);
	}
}
