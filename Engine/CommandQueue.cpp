#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	/* ----- Mapping ----- */
	_swapChain = swapChain;

	/* ----- CommandQueue Ÿ�� ���� / ����Ʈ, �Ҵ�, ���� ----- */
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	_cmdList->Close();	// Close : ���� / Open : ����Ʈ �Է�

	/* ----- Fence ���� �� �̺�Ʈ ���� �������� Event ���� ----- */
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	_fenceValue++;

	/* ----- TimeLine�� ���ŵǾ��ٰ� �˷��ش�. ----- */
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	/* ----- ���ǥ�� ���� ���ڸ��ٸ� ��ٸ��� ������ش�. ----- */
	if (_fence->GetCompletedValue() < _fenceValue) {
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	/* ----- �׸��� �� ���� �ܰ� ----- */
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	/* ----- Barrier�� ������ش�. ----- */
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ���			Before State
		D3D12_RESOURCE_STATE_RENDER_TARGET); // ���� �����	After State

	/* ----- ����� ������ �������ش�. ----- */
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	GEngine->GetConstantBuffer()->Clear();
	GEngine->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	_cmdList->ResourceBarrier(1, &barrier);

	/* ----- �׷��� ���ҽ��� ũ�⸦ �������ش�. ----- */
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	/* ----- BackBuffer�� �������ش�. ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	/* ----- Barrier�� �ٽ� �������ش�. ----- */
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // ���� �����	Before
		D3D12_RESOURCE_STATE_PRESENT); // ȭ�� ���			Atfer

	/* ----- ���� ����Ʈ�� �ִ� ������ GPU���� �������ش�. ----- */
	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	/* ----- �׷��� �� SwapChain BackBuffer�� ���� �׷��ֱ⸦ ��û�Ѵ�. ----- */
	_swapChain->Present();

	/* ----- ������ ���� ���ؼ� �� �׷�������� ��ٸ���. ----- */
	WaitSync();

	/* ----- SwapChain�� ���� �ε����� ������ ���ش�. ----- */
	_swapChain->SwapIndex();
}
