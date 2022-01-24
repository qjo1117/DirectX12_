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
	// �� ������쿡�� Instancing�� �� �� �ִٴ� ������������ �������Ѵ�.
	if (_mesh == nullptr && _material == nullptr)
		return 0;

	// ���� �ǹ�
	//uint64 id = (_mesh->GetID() << 32) | _material->GetID();
	InstanceID instanceID{ _mesh->GetID(), _material->GetID() };
	return instanceID.id;
}