#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
}

Material::~Material()
{
}

void Material::PushData()
{
	/* ----- ConstantBuffer(CBV) 정보 기입 ----- */ 
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	/* ----- TextureBuffer(SRV) 정보 기입 ----- */
	for (size_t i = 0; i < _textures.size(); ++i) {
		if (_textures[i] == nullptr) {
			continue;
		}

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetTableDescHeap()->SetSRV(_textures[i]->GetSRVCpuHandle(), reg);
	}

	/* ----- Shader 정보 기입 ----- */
	_shader->Update();
}
