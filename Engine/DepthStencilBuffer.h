#pragma once


/*-----------------------
	DepthStencilBuffer
------------------------*/
class DepthStencilBuffer
{
public:
	/* ----- External Function ----- */
	void Init(const WindowInfo& info, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	/* ----- Helper Function ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT					GetDSVFormat() { return _dsvFormat; }

private:
	/* ----- DepthStencil Variable ----- */
	ComPtr<ID3D12Resource>			_dsvBuffer;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_dsvHandle = {};
	DXGI_FORMAT						_dsvFormat = {};
};

