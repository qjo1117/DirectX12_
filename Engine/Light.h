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
struct LightParams
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
	void Render();
	void RenderShadow();

public:
	/* ----- Helper Function ----- */
	const LightInfo& GetLightInfo() { return _lightInfo; }

	void SetLightDirection(Vec3 direction);

	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specualr) { _lightInfo.color.specular = specualr; }

	const Vec4& GetDiffuse() { return _lightInfo.color.diffuse; }
	const Vec4& GetAmbient() { return _lightInfo.color.ambient; }
	const Vec4& GetSpecular() { return _lightInfo.color.specular; }
	LIGHT_TYPE GetLightType() { return static_cast<LIGHT_TYPE>(_lightInfo.lightType); }

	void SetLightType(LIGHT_TYPE type);
	void SetLightRange(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

	const float& GetRange() { return _lightInfo.range; }
	const float& GetAngle() { return _lightInfo.angle; }

	void SetLightIndex(int8 index) { _lightIndex = index; }


private:
	/* ----- LightInfo Variable ----- */
	LightInfo						_lightInfo = {};

	int8							_lightIndex = -1;
	shared_ptr<class Mesh>			_volumeMesh;
	shared_ptr<class Material>		_lightMaterial;
	shared_ptr<class GameObject>	_shadowCamera;

	shared_ptr<class Texture> _shadowTex;
};

