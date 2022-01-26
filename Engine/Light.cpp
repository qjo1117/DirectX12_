#include "pch.h"
#include "Light.h"
#include "Transform.h"
#include "Engine.h"
#include "Resources.h"
#include "Camera.h"
#include "Transform.h"
#include "Texture.h"
#include "SceneManager.h"


Light::Light() : Component(COMPONENT_TYPE::LIGHT)
{
	_shadowCamera = make_shared<GameObject>();
	_shadowCamera->AddComponent(make_shared<Transform>());
	_shadowCamera->AddComponent(make_shared<Camera>());
	_shadowCamera->GetCamera()->SetCullingMaskLayerOnOff(static_cast<uint32>(LAYER_TYPE::UI), true);
/*	_shadowCamera->GetTransform()->SetParent(GetGameObject()->GetTransform());	*/	// 개인적인 추가
	_shadowCamera->SetName(L"ShadowCamera");

	_shadowTex = GET_SINGLE(Resources)->Get<Texture>(L"ShadowTarget");
}

Light::~Light()
{
}

void Light::FinalUpdate()
{
	_lightInfo.position = GetTransform()->GetWorldPosition();

	_shadowCamera->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());
	_shadowCamera->GetTransform()->SetLocalRotation(GetTransform()->GetLocalRotation());
	_shadowCamera->GetTransform()->SetLocalScale(GetTransform()->GetLocalScale());

	_shadowCamera->FinalUpdate();
}

void Light::Render()
{
	assert(_lightIndex >= 0);

	GetTransform()->PushData();

	switch (static_cast<LIGHT_TYPE>(_lightInfo.lightType)) {
	case LIGHT_TYPE::DIRECTIONAL_LIGHT: {
		_lightMaterial->SetTexture(2, _shadowTex);

		Matrix matVP = _shadowCamera->GetCamera()->GetViewMatrix() * _shadowCamera->GetCamera()->GetProjectionMatrix();
		_lightMaterial->SetMatrix(0, matVP);
		break;
	}
	case LIGHT_TYPE::POINT_LIGHT:
	case LIGHT_TYPE::SPOT_LIGHT:
		float scale = 2 * _lightInfo.range;
		GetTransform()->SetLocalScale(Vec3(scale, scale, scale));
		break;
	}

	_lightMaterial->SetInt(0, _lightIndex);		// Shadar에서 넘겨주는 거 생각하면 됨
	_lightMaterial->PushGraphicsData();

	_volumeMesh->Render();
}

void Light::RenderShadow()
{
	_shadowCamera->GetCamera()->SortShadowGameObject();
	_shadowCamera->GetCamera()->Render_Shadow();

}

void Light::SetLightDirection(Vec3 direction)
{
	direction.Normalize();
	_lightInfo.direction = direction;
	GetTransform()->LookAt(direction);
}

void Light::SetLightType(LIGHT_TYPE type)
{
	_lightInfo.lightType = static_cast<int32>(type);

	switch (type) {
	case LIGHT_TYPE::DIRECTIONAL_LIGHT:
		_volumeMesh = GET_SINGLE(Resources)->LoadRectangleMesh();
		_lightMaterial = GET_SINGLE(Resources)->Get<Material>(L"DirLight");

		_shadowCamera->GetCamera()->SetScale(1.f);
		_shadowCamera->GetCamera()->SetFar(10000.f);
		_shadowCamera->GetCamera()->SetWidth(4096);
		_shadowCamera->GetCamera()->SetHeight(4096);
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
