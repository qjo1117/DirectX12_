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

public:
	/* ----- Helper Function ----- */
	void AddFunction(function<void()> func);
	
	ComPtr<ID3D12DescriptorHeap> GetSRVHeap() { return _srvHeap; }

private:
	/* ----- Draw Variable ----- */
	ComPtr<ID3D12DescriptorHeap> _srvHeap;

	/* ----- GUI Variable ----- */
	vector<function<void()>> _renderFuncs;
};

