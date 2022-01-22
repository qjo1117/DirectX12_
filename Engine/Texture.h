#pragma once

#include "Object.h"

/*------------
	Texture
-------------*/
class Texture : public Object
{
public:
	Texture();
	virtual ~Texture();
public:
	/* ----- External Function ----- */
	virtual void Load(const wstring& path) override;		// 로딩

public:
	void Create(DXGI_FORMAT format, uint32 width, uint32 height, const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags, D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor = Vec4());
	void CreateFromResource(ComPtr<ID3D12Resource> tex2D);	// 리소스가 존재할때

public:
	/* ----- Helper Function ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE		GetSRVCpuHandle() { return _srvHeapBegin; }
	D3D12_CPU_DESCRIPTOR_HANDLE		GetUAVCpuHandle() { return _uavHeapBegin; }

	D3D12_GPU_DESCRIPTOR_HANDLE		GetSRVGpuHandle() { return _srvGpuHandle; }

	
	ComPtr<ID3D12Resource>			GetTex2D() { return _tex2D; }
	ComPtr<ID3D12DescriptorHeap>	GetSRV() { return _srvHeap; }
	ComPtr<ID3D12DescriptorHeap>	GetRTV() { return _rtvHeap; }
	ComPtr<ID3D12DescriptorHeap>	GetDSV() { return _dsvHeap; }

	uint32 GetWidth() { return  static_cast<uint32>(_image.GetImages()->width); }
	uint32 GetHeight() { return static_cast<uint32>(_image.GetImages()->height); }

private:
	/* ----- Image Resource Variable ----- */
	ScratchImage					_image;
	ComPtr<ID3D12Resource>			_tex2D;

	/* ----- GPU Handle Variable ----- */
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	ComPtr<ID3D12DescriptorHeap>	_dsvHeap;
	ComPtr<ID3D12DescriptorHeap>	_uavHeap;

private:
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHeapBegin = {};
	D3D12_CPU_DESCRIPTOR_HANDLE		_uavHeapBegin = {};
	D3D12_GPU_DESCRIPTOR_HANDLE		_srvGpuHandle = {};
};

