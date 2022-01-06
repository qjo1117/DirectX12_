#pragma once


/*------------
	Texture
-------------*/
class Texture
{
public:
	/* ----- External Function ----- */
	void Init(const wstring& path);

	/* ----- Helper Function ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }
	
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
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle;
};

