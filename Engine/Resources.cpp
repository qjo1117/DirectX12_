#include "pch.h"
#include "Resources.h"
#include "Engine.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Light.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "PathManager.h"
#include "Camera.h"

void Resources::Init()
{
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultGameObject();
}

shared_ptr<Mesh> Resources::LoadRectangleMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Rectangle");
	if (findMesh)
		return findMesh;

	float w2 = 0.5f;
	float h2 = 0.5f;

	vector<Vertex> vec(4);

	// 앞면
	vec[0] = Vertex(Vec3(-w2, -h2, 0), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, 0), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, 0), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, 0), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(6);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add(L"Rectangle", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadCircleMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Circle");
	if (findMesh)
		return findMesh;

	uint32 sliceCount = 20;

	vector<Vertex> vertices;
	vector<uint32> indices;

	// 윗 뚜껑
	{
		// 버텍스
		float y = 0.0f;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = 1.0f * ::cosf(i * phi);
			float z = 1.0f * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		}

		vertices.push_back(Vertex(Vec3(0.0f, y, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));


		// 인덱스
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);
		}
	}
	
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vertices, indices);
	Add(L"Circle", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadCubeMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Cube");
	if (findMesh)
		return findMesh;

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<Vertex> vec(24);

	// 앞면
	vec[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
	// 뒷면
	vec[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 윗면
	vec[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vec[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	// 아랫면
	vec[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vec[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 왼쪽면
	vec[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vec[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	// 오른쪽면
	vec[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vec[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

	vector<uint32> idx(36);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add(L"Cube", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadSphereMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Sphere");
	if (findMesh)
		return findMesh;

	float radius = 0.5f; // 구의 반지름
	uint32 stackCount = 20; // 가로 분할
	uint32 sliceCount = 20; // 세로 분할

	vector<Vertex> vec;

	Vertex v;

	// 북극
	v.pos = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 1.0f);
	vec.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.pos.x = radius * sinf(phi) * cosf(theta);
			v.pos.y = radius * cosf(phi);
			v.pos.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.pos;
			v.normal.Normalize();

			v.tangent.x = -radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vec.push_back(v);
		}
	}

	// 남극
	v.pos = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.pos;
	v.normal.Normalize();
	v.tangent = Vec3(1.0f, 0.0f, 0.0f);
	vec.push_back(v);

	vector<uint32> idx(36);

	// 북극 인덱스
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// 몸통 인덱스
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	uint32 bottomIndex = static_cast<uint32>(vec.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add(L"Sphere", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadCylinderMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Cylinder");
	if (findMesh)
		return findMesh;

	float topRadius = 0.5f;
	float bottomRadius = 0.5f;

	uint32 sliceCount = 10;
	uint32 stackCount = 10;

	float height = 2.5f;

	vector<Vertex> vertices;

	// 실린더의 높이만큼을 몇개의 사각형으로 구성하지
	// Ex) stackCount에 2개가 들어오면 실린더의 한면이 2개의 사각형으로 구성됨
	// (두개를 합친 사각형 높이 = height)
	float stackHeight = height / static_cast<float>(stackCount);

	// 위뚜껑과 아래뚜껑의 반지름이 다를 경우 반지름을 맞춰주기 위함.
	float radiusStep = (topRadius - bottomRadius) / static_cast<float>(stackCount);

	uint32 ringCount = stackCount + 1;

	for (uint32 i = 0; i < ringCount; ++i) {
		float y = (-0.5f * height) + (i * stackHeight);
		float r = bottomRadius + (i * radiusStep);
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 k = 0; k <= sliceCount; ++k) {
			float x = r * ::cosf(k * phi);
			float z = r * ::sinf(k * phi);

			Vertex vertex;
			vertex.pos = Vec3(x, y, z);

			vertex.tangent = Vec3(-z, 0.0f, x);
			float dr = bottomRadius - topRadius;
			Vec3 biTangent = Vec3(dr * x, -height, dr * z);

			vertex.normal.Cross(vertex.tangent, biTangent);
			vertex.normal.Normalize();

			float u = k / static_cast<float>(sliceCount);
			float v = i / static_cast<float>(ringCount);
			vertex.uv = Vec2(u, v);

			vertices.push_back(vertex);
		}
	}

	vector<uint32> indices;
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount; ++y) {
		for (uint32 x = 0; x < sliceCount; ++x) {
			indices.push_back(y * ringVertexCount + x);
			indices.push_back((y + 1) * ringVertexCount + x);
			indices.push_back((y + 1) * ringVertexCount + (x + 1));

			indices.push_back(y * ringVertexCount + x);
			indices.push_back((y + 1) * ringVertexCount + (x + 1));
			indices.push_back(y * ringVertexCount + (x + 1));
		}
	}


	// 윗 뚜껑
	{
		// 버텍스
		float y = 0.5f * height;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = topRadius * ::cosf(i * phi);
			float z = topRadius * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		}

		vertices.push_back(Vertex(Vec3(0.0f, y, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));


		// 인덱스
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);
		}
	}

	// 아래뚜껑
	{
		// 버텍스
		float y = -0.5f * height;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = bottomRadius * ::cosf(i * phi);
			float z = bottomRadius * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)));
		}
		vertices.push_back(Vertex(Vec3(0.0f, y, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)));


		// 인덱스
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);

		}
	}
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vertices, indices);
	Add(L"Cylinder", mesh);

	return mesh;
}

shared_ptr<Mesh> Resources::LoadConeMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Cone");
	if (findMesh)
		return findMesh;

	uint32 sliceCount = 20;

	vector<Vertex> vertices;
	vector<uint32> indices;
	float height = 2.0f;

	// 윗 뚜껑
	{
		// 버텍스
		float y = 0.0f;
		float phi = 2.0f * XM_PI / (float)sliceCount;

		for (uint32 i = 0; i <= sliceCount; ++i) {
			float x = 1.0f * ::cosf(i * phi);
			float z = 1.0f * ::sinf(i * phi);

			float u = x * 0.5f + 0.5f;
			float v = z * 0.5f + 0.5f;

			vertices.push_back(Vertex(Vec3(x, y, z), Vec2(u, v), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));
		}

		vertices.push_back(Vertex(Vec3(0.0f, height, 0.0f), Vec2(0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)));


		// 인덱스
		uint32 baseIndex = vertices.size() - sliceCount - 2;
		uint32 centerIndex = vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i) {
			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i);
			indices.push_back(baseIndex + i + 1);

			indices.push_back(centerIndex);
			indices.push_back(baseIndex + i + 1);
			indices.push_back(baseIndex + i);

		}
	}

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vertices, indices);
	Add(L"Cone", mesh);
	return mesh;
}

shared_ptr<Mesh> Resources::LoadPointMesh()
{
	shared_ptr<Mesh> findMesh = Get<Mesh>(L"Point");
	if (findMesh)
		return findMesh;

	vector<Vertex> vec(1);
	vec[0] = Vertex(Vec3(0, 0, 0), Vec2(0.5f, 0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));

	vector<uint32> idx(1);
	idx[0] = 0;

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(vec, idx);
	Add(L"Point", mesh);

	return mesh;
}

shared_ptr<Texture> Resources::CreateTexture(const wstring& name, DXGI_FORMAT format, uint32 width, uint32 height,
	const D3D12_HEAP_PROPERTIES& heapProperty, D3D12_HEAP_FLAGS heapFlags,
	D3D12_RESOURCE_FLAGS resFlags, Vec4 clearColor)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->Create(format, width, height, heapProperty, heapFlags, resFlags, clearColor);
	Add(name, texture);

	return texture;
}

shared_ptr<Texture> Resources::CreateTextureFromResource(const wstring& name, ComPtr<ID3D12Resource> tex2D)
{
	shared_ptr<Texture> texture = make_shared<Texture>();
	texture->CreateFromResource(tex2D);
	Add(name, texture);

	return texture;
}

void Resources::CreateDefaultShader()
{
	/* ----- Skybox ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::LESS_EQUAL
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"skybox.fx", info);
		Add<Shader>(L"Skybox", shader);
	}

	/* ----- Deferred (Deferred) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::DEFERRED
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"deferred.fx", info);
		Add<Shader>(L"Deferred", shader);
	}

	/* ----- Forward (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"forward.fx", info);
		Add<Shader>(L"Forward", shader);
	}

	/* ----- Wirefram (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::WIREFRAME,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"wirefram.fx", info);
		Add<Shader>(L"Wirefram", shader);
	}

	/* ----- Texture (Forward) ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::FORWARD,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"forward.fx", info, "VS_Tex", "PS_Tex");
		Add<Shader>(L"Texture", shader);
	}

	/* ----- DirLight ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"lighting.fx", info, "VS_DirLight", "PS_DirLight");
		Add<Shader>(L"DirLight", shader);
	}

	/* ----- PointLight ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_NONE,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
			BLEND_TYPE::ONE_TO_ONE_BLEND
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"lighting.fx", info, "VS_PointLight", "PS_PointLight");
		Add<Shader>(L"PointLight", shader);
	}

	/* ----- Final ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::LIGHTING,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE,
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"lighting.fx", info, "VS_Final", "PS_Final");
		Add<Shader>(L"Final", shader);
	}


	/* ----- ComputeShader ----- */
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"compute.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeShader", shader);
	}


	/* ----- GraphicsParticle ----- */
	{
		ShaderInfo info =
		{
			SHADER_TYPE::PARTICLE,
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_TYPE::LESS_NO_WRITE,
			BLEND_TYPE::ALPHA_BLEND,
			D3D_PRIMITIVE_TOPOLOGY_POINTLIST
		};

		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateGraphicsShader(L"particle.fx", info, "VS_Main", "PS_Main", "GS_Main");
		Add<Shader>(L"Particle", shader);
	}
	/* ----- ComputeParticle ----- */
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shader->CreateComputeShader(L"particle.fx", "CS_Main", "cs_5_0");
		Add<Shader>(L"ComputeParticle", shader);
	}
}

void Resources::CreateDefaultMaterial()
{
	/* ----- Skybox ----- */
	{
		shared_ptr<Shader> shader = Get<Shader>(L"Skybox");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Skybox", material);
	}

	/* ----- DirLight ----- */
	{
		shared_ptr<Shader> shader = Get<Shader>(L"DirLight");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, Get<Texture>(L"PositionTarget"));
		material->SetTexture(1, Get<Texture>(L"NormalTarget"));
		Add<Material>(L"DirLight", material);
	}

	/* ----- PointLight ----- */
	{
		const WindowInfo& window = GEngine->GetWindow();
		Vec2 resolution = { static_cast<float>(window.width), static_cast<float>(window.height) };

		shared_ptr<Shader> shader =Get<Shader>(L"PointLight");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(0, Get<Texture>(L"PositionTarget"));
		material->SetTexture(1, Get<Texture>(L"NormalTarget"));
		material->SetVec2(0, resolution);
		Add<Material>(L"PointLight", material);
	}

	/* ----- Final ----- */
	{
		shared_ptr<Shader> shader = Get<Shader>(L"Final");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);

		shared_ptr<Texture> tex1 = Get<Texture>(L"DiffuseTarget");
		shared_ptr<Texture> tex2 = Get<Texture>(L"DiffuseLightTarget");
		shared_ptr<Texture> tex3 = Get<Texture>(L"SpecularLightTarget");

		material->SetTexture(0, tex1);
		material->SetTexture(1, tex2);
		material->SetTexture(2, tex3);
		Add<Material>(L"Final", material);
	}

	/* ----- Compute Shader ----- */
	{
		shared_ptr<Shader> shader = Get<Shader>(L"ComputeShader");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"ComputeShader", material);
	}

	/* ----- Particle ----- */
	{
		shared_ptr<Shader> shader = Get<Shader>(L"Particle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"Particle", material);
	}

	/* ----- Compute Shader ----- */
	{
		shared_ptr<Shader> shader = Get<Shader>(L"ComputeParticle");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		Add<Material>(L"ComputeParticle", material);
	}
}

void Resources::CreateDefaultGameObject()
{
	const wstring& texPath = GET_SINGLE(PathManager)->FindPath(TEXTURE_PATH);
	const wstring& shaderPath = GET_SINGLE(PathManager)->FindPath(SHADER_PATH);

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>());

		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			meshRenderer->SetMesh(GET_SINGLE(Resources)->LoadCubeMesh());
		}
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Skybox", texPath + L"SkyBox_1.jpg");
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		skybox->SetCheckFrustum(false);
		skybox->SetLayer(LAYER_TYPE::SKYBOX);
		Add<GameObject>(L"Skybox", skybox);
	}
#pragma endregion

#pragma region Main Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());
		camera->SetGUIName(L"MainCamera");

		camera->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, true); // UI는 안찍음
		Add<GameObject>(L"MainCamera", camera);
	}
#pragma endregion

#pragma region UI Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();

		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>());

		camera->SetGUIName(L"UI_Camera");

		camera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 0.0f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);

		camera->GetCamera()->SetCullingMaskAll();
		camera->GetCamera()->SetCullingMaskLayerOnOff(LAYER_TYPE::UI, false);	// UI만 찍음

		Add<GameObject>(L"UICamera", camera);
	}
#pragma endregion

#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetGUIName(L"Directional Light");
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0.0f, -1.0f, 0.0f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.7f, 0.5f, 0.6f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		Add<GameObject>(L"DirectionalLight", light);
	}
#pragma endregion

#pragma region Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->SetGUIName(L"Point Light");
		light->AddComponent(make_shared<Transform>());
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.5f, 0.5f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetLightRange(500.0f);

		Add<GameObject>(L"PointLight", light);
	}
#pragma endregion
}
