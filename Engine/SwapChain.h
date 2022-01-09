#pragma once


/*---------------
	SwapChain
----------------*/
class SwapChain
{
public:
	/* ----- External Function ----- */
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

	/* ----- Helper Function ----- */
	ComPtr<IDXGISwapChain>		GetSwapChain() { return _swapChain; }
	ComPtr<ID3D12Resource>		GetRenderTarget(int32 index) { return _rtvBuffer[index]; }

	ComPtr<ID3D12Resource>		GetBackRTVBuffer() { return _rtvBuffer[_backBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetBackRTV() { return _rtvHandle[_backBufferIndex]; }
private:
	/* ----- Create Function ----- */
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);

private:
	/* ----- SwapChain Variable ----- */
	ComPtr<IDXGISwapChain>	_swapChain;
	
	/* ----- SwapChain Resource ----- */
	ComPtr<ID3D12Resource>			_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	uint32					_backBufferIndex = 0;
};

