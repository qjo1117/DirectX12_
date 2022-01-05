#pragma once

/*--------------------
	RootSignature
---------------------*/
class RootSignature
{
public:
	/* ----- External Function ----- */
	void Init(ComPtr<ID3D12Device> device);

	/* ----- Helper Function ----- */
	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	/* ----- RootSignature Variable ----- */
	ComPtr<ID3D12RootSignature>	_signature;
};

