#pragma once

#include "Object.h"

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
	void Init(const wstring& path);
	void Update();

private:
	/* ----- Create Function ----- */
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	/* ----- PipeLine Variable ----- */
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

