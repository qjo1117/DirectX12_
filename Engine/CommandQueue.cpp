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

	// �۾��� CommandList
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	_cmdList->Close();	// Close : ���� / Open : ����Ʈ �Է�

	// Resource�� CommandList
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

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
		D3D12_RESOURCE_STATE_PRESENT,			// ȭ�� ���		Before State
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// ���� �����	After State

	/* ----- ����� ������ �������ش�. ----- */
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	for (uint8 type = 0; type < static_cast<uint8>(CONSTANT_BUFFER_TYPE::END); ++type) {
		GEngine->GetConstantBuffer(static_cast<CONSTANT_BUFFER_TYPE>(type))->Clear();
	}
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

	/* ----- ���� ������ ������ �������ش�. ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle();
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	_cmdList->ClearDepthStencilView(
		depthStencilView, 
		D3D12_CLEAR_FLAG_DEPTH /* | StencilŸ���� ��Ʈ�÷��׷� �߰�����*/, 
		1.0f, 0, 0, nullptr
	);
}

void CommandQueue::RenderEnd()
{
	/* ----- Barrier�� �ٽ� �������ش�. ----- */
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,		// ���� �����	Before
		D3D12_RESOURCE_STATE_PRESENT			// ȭ�� ���			Atfer
	);

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

void CommandQueue::FlushResourceCommnadQueue()
{
	/* ----- Resource Command List�� �մ� ������ ���ε����ش�. ----- */
	_resCmdList->Close();		// ����

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}
