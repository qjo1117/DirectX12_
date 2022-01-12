#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	/* ----- Group ������ �����ϰ� �׿� �°� *ũ��*������ش�. ----- */
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * (REGISTER_COUNT - 1);			// ����
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;		// Shader�ڵ忡�� ������
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;			// � Ÿ�Ե��� ��������

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	/* ----- �ϵ����� ũ�Ⱑ �ٸ��� �ԷµǴ� ��찡 �־ ó���� ����� ���ش�. ----- */
	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * (REGISTER_COUNT - 1);
}

void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	/* ----- ConstantBuffer Set ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	/* ----- Sampler Resource View Set ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void TableDescriptorHeap::CommitTable()
{
	/* ----- ���� Heap�� �ִ� ������ CMD_LIST�� ������Ѵ޶�� �����Ѵ�. ----- */
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	CMD_LIST->SetGraphicsRootDescriptorTable(1, handle);

	_currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint8 reg)
{
	assert(reg > 0);

	/* ----- ����Ʈ / 103�� / 104ȣ ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	handle.ptr += (reg - 1) * _handleSize;
	return handle;
}