#pragma once
#include "Component.h"

enum class LIGHT_TYPE : int32
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

// ���������ϴ� �ǵ� HLSL�� 16byte ������ �⺻���� �ؼ�
// float3�� ��������� float4�������� ������ ���̴�.

struct LightColor
{
	Vec4 diffuse;
	Vec4 ambient;
	Vec4 specular;
};

struct LightInfo
{
	LightColor	color;
	Vec4		position;
	Vec4		direction;

	int32		lightType;
	float		range;
	float		angle;
	int32		padding;
};

// ������ Shader�ڵ忡 �Էµ� ����ü
struct LightParam
{
	uint32		lightCount;
	Vec3		padding;
	LightInfo	lights[50];
};

/*-----------------
	SceneManager
------------------*/
class Light : public Component
{
public:
	/* ----- Constructor Function ----- */
	Light();
	virtual ~Light();

	/* ----- virtual Function ----- */
	virtual void FinalUpdate() override;

public:
	/* ----- Helper Function ----- */
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }

	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specualr) { _lightInfo.color.specular = specualr; }

	const Vec4& GetDiffuse() { return _lightInfo.color.diffuse; }
	const Vec4& GetAmbient() { return _lightInfo.color.ambient; }
	const Vec4& GetSpecular() { return _lightInfo.color.specular; }

	void SetLightType(LIGHT_TYPE type) { _lightInfo.lightType = static_cast<int32>(type); }
	void SetLightRange(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

	const float& GetRange() { return _lightInfo.range; }
	const float& GetAngle() { return _lightInfo.angle; }

private:
	/* ----- LightInfo Variable ----- */
	LightInfo _lightInfo = {};
};

