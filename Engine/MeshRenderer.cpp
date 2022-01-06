#include "pch.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"

MeshRenderer::MeshRenderer() :
	Component(COMPONENT_TYPE::MESH_RENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	_material->Update();
	_mesh->Render();
}
