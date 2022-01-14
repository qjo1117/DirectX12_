#pragma once

#include "Object.h"


enum class RASTERIZER_TYPE
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAM,
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,					// 기본상태	: DepthStencil값보다 적었을때 그리자.
	LESS_EQUAL,
	GREATER,				
	GERATER_EQUAL,
};

struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

/*-----------
	Shader
------------*/
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();
public:
	/* ----- External Function ----- */
	void Init(const wstring& path, ShaderInfo info = ShaderInfo());
	void Update();

private:
	/* ----- Create Function ----- */
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	void SetRasterizeInfo();
	void SetDepthStencilInfo();

private:
	/* ----- PipeLine Variable ----- */
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};

	ShaderInfo							_info;
};

