#pragma once

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/imgui_impl_dx12.h>

/*---------------
	EngineGUI
----------------*/
class EngineGUI
{
public:
	/* ----- External Function ----- */
	void Init();
	void Render();
	void End();

	void ResizeWindow();

public:
	/* ----- Helper Function ----- */
	void AddFunction(function<void()> func);
	
	ComPtr<ID3D12DescriptorHeap> GetSRVHeap() { return _srvHeap; }

private:
	/* ----- Draw Variable ----- */
	ComPtr<ID3D12DescriptorHeap> _srvHeap;

	shared_ptr<class Texture> _guiTex;

	/* ----- GUI Variable ----- */
	vector<function<void()>> _renderFuncs;
	//shared_ptr<Texture> texture;
};

class Texture;

namespace ImGui
{
	// 이유 너무 쓰기 힘들어서 간단하게 가져오기 위함
	void Image(shared_ptr<Texture> texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	void ImageButton(shared_ptr<Texture> texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));

	static uint32 GImageCount;
}

