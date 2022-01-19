#pragma once

#include "Object.h"

enum
{
	MATERIAL_INT_COUNT = 4,
	MATERIAL_FLOAT_COUNT = 4,
	MATERIAL_TEXTURE_COUNT = 4,
	MATERIAL_VECTOR2_COUNT = 4,
};

struct MaterialParam
{
	void SetInt(uint8 index, int32 value) { intParam[index] = value; }
	void SetFloat(uint8 index, float value) { floatParam[index] = value; }
	void SetTexOn(uint8 index, int32 value) { texOnParam[index] = value; }
	void SetVec2(uint8 index, Vec2 value) { vec2Param[index] = value; }

	array<int32, MATERIAL_INT_COUNT>		intParam;
	array<float, MATERIAL_FLOAT_COUNT>		floatParam;
	array<int32, MATERIAL_TEXTURE_COUNT>	texOnParam;
	array<Vec2,  MATERIAL_VECTOR2_COUNT>	vec2Param;
};

/*-------------
	Material
--------------*/
class Material : public Object
{
public:
	Material();
	virtual ~Material();

public:
	/* ----- External Function ----- */
	void PushData();

	/* ----- Helper Function ----- */
	shared_ptr<class Shader> GetShader() { return _shader; }

	void SetShader(shared_ptr<class Shader> shader) { _shader = shader; }
	void SetInt(uint8 index, int32 value) { _params.SetInt(index, value); }
	void SetFloat(uint8 index, float value) { _params.SetFloat(index, value); }
	void SetTexture(uint8 index, shared_ptr<class Texture> texture) { 
		_textures[index] = texture;
		_params.SetTexOn(index, (texture == nullptr) ? 0 : 1);
	}
	void SetVec2(uint8 index, Vec2 value) { _params.SetVec2(index, value); }
	
	shared_ptr<class Texture> GetTexture(uint8 index) { return _textures[index]; }
private:
	/* ----- Material Part Variable ----- */
	shared_ptr<class Shader>		_shader;
	MaterialParam					_params = {};
	array<shared_ptr<class Texture>, MATERIAL_TEXTURE_COUNT>	_textures;
};

