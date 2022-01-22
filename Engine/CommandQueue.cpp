#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"
#include "RenderTargetGroup.h"

/*--------------------------
	GraphicsCommandQueue
---------------------------*/

GraphicsCommandQueue::~GraphicsCommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void GraphicsCommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	/* ----- Mapping ----- */
	_swapChain = swapChain;

	/* ----- CommandQueue 타입 정의 / 리스트, 할당, 생성 ----- */
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// 작업용 CommandList
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	_cmdList->Close();	// Close : 제출 / Open : 리스트 입력

	// Resource용 CommandList
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

	/* ----- Fence 생성 및 이벤트 락을 쓰기위해 Event 생성 ----- */
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void GraphicsCommandQueue::WaitSync()
{
	_fenceValue++;

	/* ----- TimeLine이 갱신되었다고 알려준다. ----- */
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	/* ----- 대기표가 아직 모자르다면 기다리게 만들어준다. ----- */
	if (_fence->GetCompletedValue() < _fenceValue) {
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}


void GraphicsCommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	/* ----- 그리기 전 사전 단계 ----- */
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	/* ----- Barrier를 만들어준다. ----- */
	int8 backIndex = _swapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// 화면 출력		Before State
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// 외주 결과물	After State
	
	/* ----- 사용할 영역을 정의해준다. ----- */
	_cmdList->SetGraphicsRootSignature(GRAPHICS_ROOT_SIGNATURE.Get());

	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
	GEngine->GetGraphicsDescHeap()->Clear();

	ID3D12DescriptorHeap* descriptorHeaps[] = { GEngine->GetGraphicsDescHeap()->GetDescriptorHeap().Get() };
	GRAPHICS_CMD_LIST->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	_cmdList->ResourceBarrier(1, &barrier);

	/* ----- 그려질 리소스의 크기를 정의해준다. ----- */
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);
}

void GraphicsCommandQueue::RenderEnd()
{
	/* ----- Barrier를 다시 생성해준다. ----- */
	int8 backIndex = _swapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,		// 외주 결과물	Before
		D3D12_RESOURCE_STATE_PRESENT			// 화면 출력			Atfer
	);

	/* ----- 현재 리스트에 있는 내용을 GPU에게 전달해준다. ----- */
	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	/* ----- 그려진 뒤 SwapChain BackBuffer를 통해 그려주기를 요청한다. ----- */
	_swapChain->Present();

	/* ----- 간단한 락을 통해서 다 그려지기까지 기다린다. ----- */
	WaitSync();

	/* ----- SwapChain의 버퍼 인덱스를 스왑핑 해준다. ----- */
	_swapChain->SwapIndex();
}

void GraphicsCommandQueue::FlushResourceCommnadQueue()
{
	/* ----- Resource Command List에 잇는 내용을 업로딩해준다. ----- */
	_resCmdList->Close();		// 제출

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}

/*--------------------------
	ComputeCommandQueue
---------------------------*/

ComputeCommandQueue::~ComputeCommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void ComputeCommandQueue::Init(ComPtr<ID3D12Device> device)
{
	D3D12_COMMAND_QUEUE_DESC computeQueueDesc = {};
	computeQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	computeQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&computeQueueDesc, IID_PPV_ARGS(&_cmdQueue));

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&_cmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void ComputeCommandQueue::WaitSync()
{
	_fenceValue++;

	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void ComputeCommandQueue::FlushResourceCommnadQueue()
{
	_cmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	uint64 count = _countof(cmdListArr);
	_cmdQueue->ExecuteCommandLists(count, cmdListArr);

	WaitSync();

	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	COMPUTE_CMD_LIST->SetComputeRootSignature(COMPUTE_ROOT_SIGNATURE.Get());
}
