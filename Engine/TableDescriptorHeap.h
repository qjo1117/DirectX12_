#pragma once


/*-------------------------
	TableDescriptorHeap
--------------------------*/
class TableDescriptorHeap
{
public:
	/* ----- External Function ----- */
	void Init(uint32 count);
	void Clear();
	void CommitTable();

	/* ----- Helper Function ----- */
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);
	
	ComPtr<ID3D12DescriptorHeap>	GetDescriptorHeap() { return _descHeap; }
	D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUHandle(CBV_REGISTER reg);
	D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUHandle(SRV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint8 reg);

private:
	/* ----- TableDescriptorHeap Variable ----- */
	ComPtr<ID3D12DescriptorHeap>	_descHeap;
	uint64							_handleSize = 0;

	/* ----- Group Difference Varible ----- */
	uint64							_groupSize = 0;
	uint64							_groupCount = 0;

	uint32							_currentGroupIndex = 0;
};
