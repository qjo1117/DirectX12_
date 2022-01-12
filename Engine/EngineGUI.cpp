#include "pch.h"
#include "EngineGUI.h"
#include "Engine.h"
#include <ImGui/imgui_internal.h>


void EngineGUI::Init()
{
	/* ----- GUI를 생성할 Heap 생성 및, 초기화 작업 ----- */
	D3D12_DESCRIPTOR_HEAP_DESC descSrv = {};
	descSrv.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descSrv.NumDescriptors = 2;
	descSrv.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&descSrv, IID_PPV_ARGS(&_srvHeap));


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(GEngine->GetWindow().hWnd);
	ImGui_ImplDX12_Init(DEVICE.Get(), 2,
		DXGI_FORMAT_R8G8B8A8_UNORM, _srvHeap.Get(),
		_srvHeap->GetCPUDescriptorHandleForHeapStart(),
		_srvHeap->GetGPUDescriptorHandleForHeapStart());

	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = g.Viewports[0];
	viewport->Size.x = GEngine->GetWindow().width;
	viewport->Size.y = GEngine->GetWindow().height;

	/* ----- 현재 생각하는 GUI 갯수가 10이상은 아닐꺼같아서 10개로 미리 잡음 ----- */
	_renderFuncs.reserve(10);
}

void EngineGUI::Render()
{
	// TODO:DELETE
	ID3D12DescriptorHeap* descriptorHeaps[] = { _srvHeap.Get() };
	CMD_LIST->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	
	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (function<void()>& func : _renderFuncs) {
		func();
	}

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CMD_LIST.Get());
}

void EngineGUI::End()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EngineGUI::AddFunction(function<void()> func)
{
	_renderFuncs.push_back(func);
}

