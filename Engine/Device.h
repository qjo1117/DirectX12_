#pragma once


/*-----------
	Device
------------*/
class Device
{
public:
	/* ----- External Function ----- */
	void Init();

	/* ----- Helper Function ----- */
	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	/* ----- Device Variable ----- */
	ComPtr<ID3D12Debug>			_debugController;
	ComPtr<IDXGIFactory>		_dxgi;		// ȭ�� ���� ��ɵ�
	ComPtr<ID3D12Device>		_device;	// ���� ��ü ����
};

