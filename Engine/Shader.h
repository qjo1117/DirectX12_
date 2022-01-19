#pragma once

#include "Object.h"

enum class SHADER_TYPE  : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
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
	D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
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
	void Init(const wstring& path, ShaderInfo info = ShaderInfo(), const string& vs = "VS_Main", const string& ps = "PS_Main");
	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

private:
	/* ----- Create Function ----- */
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	void SetShaderInfo();
	void SetRasterizeInfo();
	void SetDepthStencilInfo();
	void SetBlendInfo();

private:
	/* ----- PipeLine Variable ----- */
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};

	ShaderInfo							_info;
};

