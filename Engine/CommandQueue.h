#pragma once

class SwapChain;
class DescriptorHeap;

/*-----------------
	CommandQueue
------------------*/
class CommandQueue
{
public:
	/* ----- Constructor Function ----- */
	~CommandQueue();

	/* ----- External Function ----- */
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync();

	/* ----- Render Function ----- */
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommnadQueue();		// 리소스를 로딩하는 함수

	/* ----- Helper Function ----- */
	ComPtr<ID3D12CommandQueue>			GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList>	GetCmdList() { return	_cmdList; }
	ComPtr<ID3D12GraphicsCommandList>	GetResourceCmdList() { return _resCmdList; }
private:
	/* ----- CommandQueue Variable ----- */
	ComPtr<ID3D12CommandQueue>			_cmdQueue;		// Queue하나만 존재해야한다.
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	/* ----- Resource CommandQueue Variable ----- */
	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;	
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;	// 리소스 로딩용도

	/* ----- Fence Variable ----- */
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	/* ----- Mapping Variable ----- */
	shared_ptr<SwapChain>		_swapChain;
};

