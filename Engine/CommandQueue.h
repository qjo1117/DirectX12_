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

	void FlushResourceCommnadQueue();		// ���ҽ��� �ε��ϴ� �Լ�

	/* ----- Helper Function ----- */
	ComPtr<ID3D12CommandQueue>			GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList>	GetCmdList() { return	_cmdList; }
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

