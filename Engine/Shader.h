#pragma once

#include "Object.h"

enum class SHADER_TYPE  : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	COMPUTE,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	LESS,					// 기본상태	: DepthStencil값보다 적었을때 그리자.
	LESS_EQUAL,
	GREATER,				
	GREATER_EQUAL,
	NO_DEPTH_TEST,				// 깊이 테스트X, 기록O
	NO_DEPTH_TEST_NO_WRITE,		// 깊이 테스트X, 기록X
	LESS_NO_WRITE,				// 깊이 테스트O, 기록X
};

enum class BLEND_TYPE : uint8	// 픽셀과 렌타겟을 어떻게 섞을 것인지
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_TO_ONE_BLEND,
	END,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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
	void CreateGraphicsShader(const wstring& path, ShaderInfo info = ShaderInfo(), const string& vs = "VS_Main", const string& ps = "PS_Main", const string& gs = "");
	void CreateComputeShader(const wstring& path, const string& name, const string& version = "cs_5_0");
	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	/* ----- Create Function ----- */
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);
	void CreateGeometryShader(const wstring& path, const string& name, const string& version);

private:
	void SetShaderInfo();
	void SetRasterizeInfo();
	void SetDepthStencilInfo();
	void SetBlendInfo();

private:
	/* ----- Unit Variable ----- */
	ShaderInfo							_info;
	ComPtr<ID3D12PipelineState>			_pipelineState;

	/* ----- Graphics Variable ----- */
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_errBlob;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _graphicsPipelineDesc = {};

	/* ----- Compute Variable ----- */
	ComPtr<ID3DBlob>					_csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC	_computePipelineDesc = {};
};

