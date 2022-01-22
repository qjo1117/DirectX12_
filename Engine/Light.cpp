#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Material.h"
#include "Mesh.h"
#include "Resources.h"

Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	_lightInfo.position = GetTransform()->GetWorldPosition();
}

void Light::Render()
{
	assert(_lightIndex >= 0);

	GetTransform()->PushData();

	_lightMaterial->SetInt(0, _lightIndex);		// Shadar에서 넘겨주는 거 생각하면 됨
	_lightMaterial->PushGraphicsData();

	switch (static_cast<LIGHT_TYPE>(_lightInfo.lightType)) {
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * _lightInfo.range;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
		break;
	}

	_volumeMesh->Render();
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int32>(type);

	switch (type) {
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");
		break;
	case LIGHT_TYPE::POINT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->LoadSphereMesh();
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	case LIGHT_TYPE::SPOT_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->LoadConeMesh();
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"PointLight");
		break;
	}

}
