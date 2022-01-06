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

void CommandQueue::WaitSync()
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


void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	/* ----- 그리기 전 사전 단계 ----- */
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	/* ----- Barrier를 만들어준다. ----- */
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,			// 화면 출력		Before State
		D3D12_RESOURCE_STATE_RENDER_TARGET);	// 외주 결과물	After State

	/* ----- 사용할 영역을 정의해준다. ----- */
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	for (uint8 type = 0; type < static_cast<uint8>(CONSTANT_BUFFER_TYPE::END); ++type) {
		GEngine->GetConstantBuffer(static_cast<CONSTANT_BUFFER_TYPE>(type))->Clear();
	}
	GEngine->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);

	_cmdList->ResourceBarrier(1, &barrier);

	/* ----- 그려질 리소스의 크기를 정의해준다. ----- */
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	/* ----- BackBuffer를 정의해준다. ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);

	/* ----- 깊이 버퍼의 정보를 기입해준다. ----- */
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle();
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	_cmdList->ClearDepthStencilView(
		depthStencilView, 
		D3D12_CLEAR_FLAG_DEPTH /* | Stencil타입은 비트플래그로 추가가능*/, 
		1.0f, 0, 0, nullptr
	);
}

void CommandQueue::RenderEnd()
{
	/* ----- Barrier를 다시 생성해준다. ----- */
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
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

void CommandQueue::FlushResourceCommnadQueue()
{
	/* ----- Resource Command List에 잇는 내용을 업로딩해준다. ----- */
	_resCmdList->Close();		// 제출

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}
