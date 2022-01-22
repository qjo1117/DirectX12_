#pragma once

/*--------------------
	RootSignature
---------------------*/
class RootSignature
{
public:
	/* ----- External Function ----- */
	void Init();

	/* ----- Helper Function ----- */
	ComPtr<ID3D12RootSignature>	GetGraphicsSignature() { return _graphicsRootSignature; }
	ComPtr<ID3D12RootSignature>	GetComputeSignature() { return _computeRootSignature; }

private:
	/* ----- Create Function ----- */
	void CreateGraphicsRootSignature();
	void CreateComputeRootSignature();

private:
	/* ----- GraphicsRootSignature Variable ----- */
	ComPtr<ID3D12RootSignature>		_graphicsRootSignature;
	D3D12_STATIC_SAMPLER_DESC		_samplerDesc;	// Sampler Á¤Ã¥

	/* ----- ComputeRootSignature Variable ----- */
	ComPtr<ID3D12RootSignature>		_computeRootSignature;
};

