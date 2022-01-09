#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"

void Engine::Init(const WindowInfo& info)
{
	_winInfo = info;

	/* ----- 그려질 화면 크기를 설정 ----- */
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	/* ----- 객체 생성및 초기화 부분 ----- */
#pragma region MakeShaded

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_tableDescHeap = make_shared<TableDescriptorHeap>();
	_depthStencilBuffer = make_shared<DepthStencilBuffer>();
	_engineGUI = make_shared<EngineGUI>();
#pragma endregion

	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(info);
	_engineGUI->Init();
	
	INPUT->Init(_winInfo.hWnd);
	TIMER->Init(_winInfo.hWnd);

	ResizeWindow(info.width, info.height);

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformMatrix), 256);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParam), 256);
}

void Engine::Render()
{

	RenderBegin();

<<<<<<< Updated upstream
	// TODO : 나머지 물체들 그려준다
=======
	GET_SINGLE(SceneManager)->Render();

	if (GET_SINGLE(SceneManager)->GetType() == SCENE_TYPE::TOOL) {
		_engineGUI->Render();
	}
>>>>>>> Stashed changes

	RenderEnd();


}

void Engine::Update()
{
<<<<<<< Updated upstream
	INPUT->Update();
	TIMER->Update();
=======

	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();


	Render();
>>>>>>> Stashed changes
}

void Engine::LateUpdate()
{
	// TODO
}

void Engine::End()
{
	_engineGUI->End();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_winInfo.width = width;
	_winInfo.height = height;

	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_winInfo.hWnd, 0, 100, 100, rect.right - rect.left, rect.bottom - rect.top, 0);

	_depthStencilBuffer->Init(_winInfo);
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}
