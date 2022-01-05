#pragma once

/*---------
	Mesh
----------*/
class Mesh
{
public:
	/* ----- External Function ----- */
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render();

	/* ----- Helper Function ----- */
	void SetTransform(const Transform& t) { _transform = t; }

private:
	/* ----- Create Function ----- */
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);

private:
	/* ----- Vertex Variable ----- */
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	/* ----- Indices Variable ----- */
	ComPtr<ID3D12Resource>		_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW		_indexBufferView;
	uint32 _indexCount = 0;

	/* ----- Transform Variable ----- */
	Transform _transform = {};
};

