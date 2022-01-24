#include "pch.h"
#include "InstancingManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Material.h"


void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	map<uint64,vector<shared_ptr<GameObject>>> cache;

	/* ----- ºÐ·ù ----- */
	for (shared_ptr<GameObject>& go : gameObjects) {
		const uint64 instanceId = go->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(go);
	}

	/* ----- Rendering ----- */
	for (auto& pair : cache) {
		const vector<shared_ptr<GameObject>>& vec = pair.second;


		if (vec.size() == 1) {
			/* ----- Not Instancing ----- */
			vec[0]->GetMeshRenderer()->Render();
		}
		else {
			/* ----- Instancing ----- */
			const uint64 instanceId = pair.first;

			for (const shared_ptr<GameObject>& go : vec) {
				InstancingParams params;
				params.matWorld = go->GetTransform()->GetLocalToWorldMatrix();
				params.matWV = params.matWorld * Camera::S_MatView;
				params.matWVP = params.matWV * Camera::S_MatProjection;

				AddParam(instanceId, params);
			}

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->Render(buffer);
		}
	}

}

void InstancingManager::ClearBuffer()
{
	for (auto& pair : _buffers) {
		shared_ptr<InstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void InstancingManager::AddParam(uint64 instanceId, InstancingParams& data)
{
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}
