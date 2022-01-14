#include "pch.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture() : Object(OBJECT_TYPE::TEXTURE)
{
}

Texture::~Texture()
{
}

void Texture::Init(const wstring& path)
{
	CreateTexture(path);
	CreateView();
}

void Texture::CreateTexture(const wstring& path)
{
	/* ----- 파일확장자가 무엇인지 확인한다. ----- */
	wstring ext = fs::path(path).extension();	// 파일 확장자 얻기

	if (ext == L".dds" || ext == L".DDS") {
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
	}
	else if (ext == L".tga" || ext == L".TGA") {
		::LoadFromTGAFile(path.c_str(), nullptr, _image);
	}
	else {	// png, jpg, jpeg, bmp
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);
	}

	/* ----- 리소스를 생성한다 ----- */
	HRESULT hr = ::CreateTexture(DEVICE.Get(), _image.GetMetadata(), &_tex2D);
	if (FAILED(hr)) {
		assert(nullptr);
	}

	/* ----- Resource 데이터를 GPU전용데이터로 만들어 줄 준비를 한다. ----- */
	vector<D3D12_SUBRESOURCE_DATA> subResources;

	hr = ::PrepareUpload(
		DEVICE.Get(),
		_image.GetImages(),
		_image.GetImageCount(),
		_image.GetMetadata(),
		subResources
	);

	if (FAILED(hr)) {
		assert(nullptr);
	}

	const uint64 bufferSize = ::GetRequiredIntermediateSize(_tex2D.Get(), 0, static_cast<uint32>(subResources.size()));

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> textureUploadHeap;
	hr = DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf())
	);

	if (FAILED(hr)) {
		assert(nullptr);
	}

	
	/* ----- SubResource를 업데이트해서 정보를 갱신합니다. ----- */
	::UpdateSubresources(
		RESOURCE_CMD_LIST.Get(),
		_tex2D.Get(),
		textureUploadHeap.Get(),
		0,
		0,
		static_cast<uint32>(subResources.size()),
		subResources.data()
	);

	GEngine->GetCmdQueue()->FlushResourceCommnadQueue();
}

void Texture::CreateView()
{
	/* ----- GPU에서 쓸 정보를 서술해주고 할당한다. ----- */
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DEVICE->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap));

	_srvCpuHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();
	_srvGpuHandle = _srvHeap->GetGPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _image.GetMetadata().format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	DEVICE->CreateShaderResourceView(_tex2D.Get(), &srvDesc, _srvCpuHandle);
}
