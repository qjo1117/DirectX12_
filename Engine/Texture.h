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
	void Init(const wstring& path);

	/* ----- Helper Function ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvCpuHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() { return _srvGpuHandle; }
	ComPtr<ID3D12DescriptorHeap> GetSRVHeap() { return 	_srvHeap; }

	uint32 GetWidth() { return  static_cast<uint32>(_image.GetImages()->width); }
	uint32 GetHeight() { return static_cast<uint32>(_image.GetImages()->height); }
public:
	/* ----- Create Function ----- */
	void CreateTexture(const wstring& path);
	void CreateView();

private:
	/* ----- Image Resource Variable ----- */
	ScratchImage					_image;
	ComPtr<ID3D12Resource>			_tex2D;

	/* ----- GPU Handle Variable ----- */
	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvCpuHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE		_srvGpuHandle = {};
};

