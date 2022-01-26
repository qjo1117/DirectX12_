#pragma once
#include "Component.h"

class Mesh;
class Material;


union InstanceID
{
	struct
	{
		uint32 meshID;
		uint32 materialID;
	};
	uint64 id;
};

/*------------------
	MeshRenderer
-------------------*/
class MeshRenderer : public Component
{
public:
	/* ----- Constructor Function ----- */
	MeshRenderer();
	virtual ~MeshRenderer();

public:
	/* ----- Helper Function ----- */
	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }

	inline shared_ptr<Material> GetMaterial() { return _material; }
	inline shared_ptr<Mesh> GetMesh() { return _mesh; }
	uint64 GetInstanceID();

	/* ----- virtual Function ----- */
	void Render();
	void Render(shared_ptr<class InstancingBuffer>& buffer);
	void RenderShadow();

private:
	/* ----- MeshRender Variable ----- */
	shared_ptr<Mesh>		_mesh;
	shared_ptr<Material>	_material;

	shared_ptr<Material>	_shadow;
};

