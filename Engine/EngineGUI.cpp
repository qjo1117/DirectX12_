#include "pch.h"
#include "EngineGUI.h"
#include "Engine.h"
#include <ImGui/imgui_internal.h>
#include "Texture.h"


void EngineGUI::Init()
{
	/* ----- GUI를 생성할 Heap 생성 및, 초기화 작업 ----- */
	D3D12_DESCRIPTOR_HEAP_DESC descSrv = {};
	descSrv.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descSrv.NumDescriptors = RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT + RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT + SRV_REGISTER_COUNT;
	descSrv.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DEVICE->CreateDescriptorHeap(&descSrv, IID_PPV_ARGS(&_srvHeap));


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplWin32_Init(GEngine->GetWindow().hWnd);
	ImGui_ImplDX12_Init(DEVICE.Get(), descSrv.NumDescriptors,
		DXGI_FORMAT_R8G8B8A8_UNORM, _srvHeap.Get(),
		_srvHeap->GetCPUDescriptorHandleForHeapStart(),
		_srvHeap->GetGPUDescriptorHandleForHeapStart());

	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = g.Viewports[0];
	g.Viewports[0]->Size.x = GEngine->GetWindow().width;
	g.Viewports[0]->Size.y = GEngine->GetWindow().height;

	/* ----- 현재 생각하는 GUI 갯수가 10이상은 아닐꺼같아서 10개로 미리 잡음 ----- */
	_renderFuncs.reserve(10);
}

void EngineGUI::Render()
{
	ID3D12DescriptorHeap* descriptorHeaps[] = { GEngine->GetGUI()->GetSRVHeap().Get() };
	GRAPHICS_CMD_LIST->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	ImGui::GImageCount = 1;

	// Start the Dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (function<void()>& func : _renderFuncs) {
		func();
	}

	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), GRAPHICS_CMD_LIST.Get());
}

void EngineGUI::End()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EngineGUI::ResizeWindow()
{
	ImGuiContext& g = *GImGui;
	ImGuiViewportP* viewport = g.Viewports[0];
	g.Viewports[0]->Size.x = GEngine->GetWindow().width;
	g.Viewports[0]->Size.y = GEngine->GetWindow().height;
}

void EngineGUI::AddFunction(function<void()> func)
{
	_renderFuncs.push_back(func);
}

void ImGui::Image(shared_ptr<Texture> texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	if (texture == nullptr) {
		return;
	}


	uint32 increase = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	uint32 destSize = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GEngine->GetGUI()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(), (increase * GImageCount));

	uint32 srcSize = 1;
	ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = texture->GetSRV();
	D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

	DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE guiGPU = GEngine->GetGUI()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	ImGui::Image((ImTextureID)(guiGPU.ptr + (increase * GImageCount)), size, uv0, uv1, tint_col, border_col);

	GImageCount += 1;
}


//uint32 increase = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle = GET_SINGLE(Resources)->Get<Texture>(L"DiffuseLightTarget")->GetSRV()->GetGPUDescriptorHandleForHeapStart();
////my_texture_srv_gpu_handle.ptr += DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"A", L"..\\Resources\\Texture\\Picture0.png");
////io.Fonts->TexID = (ImTextureID)my_texture_srv_gpu_handle.ptr;
//
//uint32 destSize = 1;
//D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GEngine->GetGUI()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(), increase);
//
//uint32 srcSize = 1;
//ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = texture->GetSRV();
//D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();
//
//DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//D3D12_GPU_DESCRIPTOR_HANDLE guiGPU = GEngine->GetGUI()->GetSRVHeap().Get()->GetGPUDescriptorHandleForHeapStart();
//ImGui::Text("GPU handle = %p", (ImTextureID)my_texture_srv_gpu_handle.ptr);
//ImGui::Image((ImTextureID)(guiGPU.ptr + increase), ImVec2(100.0f, 100.0f));


void ImGui::ImageButton(shared_ptr<Texture> texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	uint32 increase = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	uint32 destSize = 1;
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GEngine->GetGUI()->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(), increase * GImageCount);

	uint32 srcSize = 1;
	ComPtr<ID3D12DescriptorHeap> srcRtvHeapBegin = texture->GetSRV();
	D3D12_CPU_DESCRIPTOR_HANDLE srcHandle = srcRtvHeapBegin->GetCPUDescriptorHandleForHeapStart();

	DEVICE->CopyDescriptors(1, &destHandle, &destSize, 1, &srcHandle, &srcSize, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE guiGPU = GEngine->GetGUI()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	ImGui::ImageButton((ImTextureID)(guiGPU.ptr + (increase * GImageCount)), size, uv0, uv1, frame_padding, bg_col, tint_col);
}
