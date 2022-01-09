#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();

}

void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);		// 기본상태는 고정 블랜딩이 되는 상태 추가적으로는 MSDN ㄱㄱ
}

void RootSignature::CreateRootSignature()
{
	/* ----- TableDescriptor 초기화 방식 ----- */
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT, 0), // b0~b4
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4
	};

	/* ----- TableDescriptor 방식이 여러가지면 더 추가될 것이다. ----- */
	CD3DX12_ROOT_PARAMETER param[1];
	param[0].InitAsDescriptorTable(_countof(ranges), ranges);

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	/* ----- RootSignature 정보를 기록한다. ----- */
	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

/*
*	Table을 사용안하는 방식
	CD3DX12_ROOT_PARAMETER param[2];
	param->InitAsConstantBufferView(static_cast<int32>(CBV_REGISTER::b0));
	param->InitAsConstantBufferView(static_cast<int32>(CBV_REGISTER::b1));

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param)
*/