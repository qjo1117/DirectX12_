#pragma once

class SwapChain;
class DescriptorHeap;

/*--------------------------
	GraphicsCommandQueue
---------------------------*/
class GraphicsCommandQueue
{
public:
	/* ----- Constructor Function ----- */
	~GraphicsCommandQueue();

	/* ----- External Function ----- */
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	/* ----- Render Function ----- */
	void RenderBegin();
	void RenderEnd();

	void FlushResourceCommnadQueue();		// ���ҽ��� �ε��ϴ� �Լ�

	/* ----- Helper Function ----- */
	ComPtr<ID3D12CommandQueue>			GetGraphicsCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList>	GetGraphicsCmdList() { return	_cmdList; }
	ComPtr<ID3D12GraphicsCommandList>	GetResourceCmdList() { return _resCmdList; }
private:
	/* ----- CommandQueue Variable ----- */
	ComPtr<ID3D12CommandQueue>			_cmdQueue;		// Queue�ϳ��� �����ؾ��Ѵ�.
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	/* ----- Resource CommandQueue Variable ----- */
	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;	
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;	// ���ҽ� �ε��뵵

	/* ----- Fence Variable ----- */
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	/* ----- Mapping Variable ----- */
	shared_ptr<SwapChain>		_swapChain;
};


/*--------------------------
	ComputeCommandQueue
---------------------------*/
class ComputeCommandQueue
{
public:
	/* ----- Constructor Function ----- */
	~ComputeCommandQueue();

	/* ----- External Function ----- */
	void Init(ComPtr<ID3D12Device> device);
	void WaitSync();

	void FlushResourceCommnadQueue();		// ���ҽ��� �ε��ϴ� �Լ�

	/* ----- Helper Function ----- */
	ComPtr<ID3D12CommandQueue>			GetComputeCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList>	GetComputeCmdList() { return	_cmdList; }
private:
	/* ----- CommandQueue Variable ----- */
	ComPtr<ID3D12CommandQueue>			_cmdQueue;		// Queue�ϳ��� �����ؾ��Ѵ�.
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	/* ----- Fence Variable ----- */
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;
};
