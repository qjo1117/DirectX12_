#pragma once

/*--------------------
	ConstantBuffer
---------------------*/
class ConstantBuffer
{
public:
	/* ----- Constructor Function ----- */
	ConstantBuffer();
	~ConstantBuffer();

	/* ----- External Function ----- */
	void Init(uint32 size, uint32 count);
	void Clear();

	D3D12_CPU_DESCRIPTOR_HANDLE PushData(int32 rootParamIndex, void* buffer, uint32 size);

	/* ----- Helper Function ----- */
	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	/* ----- Create Function ----- */
	void CreateBuffer();
	void CreateView();

private:
	/* ----- Constructor Variable ----- */
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE*					_mappedBuffer = nullptr;

	uint32					_elementSize = 0;		// ũ��
	uint32					_elementCount = 0;		// ����

	/* ----- Table Descriptor Variable ----- */
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;

	uint32					_currentIndex = 0;
};

// ���� ConstantBuffer�� ��� ����ϳĸ�
// �������� �����ؼ� ���߿� ����ϰ� �մ� Index�� currentIndex�� ����ŵ�ϴ�.
// [O][O][O][][][][][][][][]
//		  C
// ���� ���� ��Ȳ�϶� PushData�� �� ��� C���� Ŭ ��� ���� ������ ������ �������ְ�
// CurrentIndex ���� �����մϴ�. (Stack)