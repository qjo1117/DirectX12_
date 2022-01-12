#pragma once
#include "Component.h"

class Mesh;
class Material;

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

	// TODO : ¿Â≥≠
	shared_ptr<Material> GetMaterial() { return _material; }

	/* ----- virtual Function ----- */
	void Render();

private:
	/* ----- MeshRender Variable ----- */
	shared_ptr<Mesh>		_mesh;
	shared_ptr<Material>	_material;
};

