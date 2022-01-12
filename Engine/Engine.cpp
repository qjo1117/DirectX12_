#include "pch.h"
#include "Engine.h"
#include "Material.h"
#include "Transform.h"
#include "Light.h"

void Engine::Init(const WindowInfo& info)
{
	// 지속적으로 걱정하지만 콘솔을 끄면 WM_QUIT 이벤트가 발생을 안해서
	// 메모리가 정상적으로 해제가 안되니 꼭꼭꼭 Window를 종료시키자.
#pragma region DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc();
	// 메모리 Leak이 있을때만 사용하면 됨

#ifdef _DEBUG
	AllocConsole();
#endif 
#pragma endregion

	/* ----- 정보 저장 ----- */
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
	
	GET_SINGLE(Input)->Init(_winInfo.hWnd);
	GET_SINGLE(Timer)->Init(_winInfo.hWnd);

	ResizeWindow(info.width, info.height);

	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParam), 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParam), 256);
}

void Engine::Render()
{

	RenderBegin();



	GET_SINGLE(SceneManager)->Render();

	if (GET_SINGLE(SceneManager)->GetType() == SCENE_TYPE::TOOL) {
		_engineGUI->Render();
	}

	RenderEnd();


}

void Engine::Update()
{

	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	GET_SINGLE(SceneManager)->Update();


	Render();
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
