#include "pch.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "InstancingBuffer.h"

MeshRenderer::MeshRenderer() :
	Component(COMPONENT_TYPE::MESH_RENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render()
{
	_material->SetInt(static_cast<uint32>(CBV_REGISTER::b3), 0);
	GetTransform()->PushData();
	_material->PushGraphicsData();
	_mesh->Render();
}

void MeshRenderer::Render(shared_ptr<InstancingBuffer>& buffer)
{
	_material->SetInt(static_cast<uint32>(CBV_REGISTER::b3), 1);
	buffer->PushData();
	_material->PushGraphicsData();
	_mesh->Render(buffer);
}

uint64 MeshRenderer::GetInstanceID()
{
	// 다 있을경우에만 Instancing을 할 수 있다는 전제조건으로 만들어야한다.
	if (_mesh == nullptr && _material == nullptr)
		return 0;

	// 같은 의미
	//uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}