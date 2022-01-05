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

	uint32					_elementSize = 0;		// 크기
	uint32					_elementCount = 0;		// 갯수

	/* ----- Table Descriptor Variable ----- */
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;

	uint32					_currentIndex = 0;
};

// 현재 ConstantBuffer를 어떻게 사용하냐면
// 여러개를 정의해서 그중에 사용하고 잇는 Index를 currentIndex가 가르킵니다.
// [O][O][O][][][][][][][][]
//		  C
// 위와 같은 상황일때 PushData를 할 경우 C보다 클 경우 다음 공간에 정보를 전달해주고
// CurrentIndex 값을 증가합니다. (Stack)