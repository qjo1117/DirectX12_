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
	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	/* ----- Create Function ----- */
	void CreateSamplerDesc();
	void CreateRootSignature();

private:
	/* ----- RootSignature Variable ----- */
	ComPtr<ID3D12RootSignature>		_signature;
	D3D12_STATIC_SAMPLER_DESC		_samplerDesc;	// Sampler Á¤Ã¥
};

