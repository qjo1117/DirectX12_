#pragma once

#include "Component.h"

class Material;
class Mesh;
class StructuredBuffer;

struct ParticleInfo
{
	Vec3	worldPos;
	float	curTime;
	Vec3	worldDir;
	float	lifeTime;
	int32	alive;
	int32	padding[3];
};

struct ComputeShaderdInfo
{
	int32 addCount;
	int32 padding[3];
};

class ParticleSystem : public Component
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();

public:
	virtual void FinalUpdate();
	void Render();

public:
	virtual void Load(const wstring& path) override {}
	virtual void Save(const wstring& path) override {}


public:
	/* ----- Get Helper Function ----- */
	uint32 GetMaxParticleCount() { return _maxParticle; }

	float GetMinLifeTime() { return _minLifeTime; }
	float GetMaxLifeTime() { return _maxLifeTime; }
	Vec2 GetLifeTime() { return Vec2(_minLifeTime, _maxLifeTime); }

	float GetMinSpeed() { return _minSpeed; }
	float GetMaxSpeed() { return _maxSpeed; }
	Vec2 GetSpeed() { return Vec2(_minSpeed, _maxSpeed); }

	float GetStartScale() { return _startScale; }
	float GetEndScale() { return _endScale; }
	Vec2 GetScale() { return Vec2(_startScale, _endScale); }

	shared_ptr<class Texture>  GetTexture();

	/* ----- Set Helper Function ----- */
	void SetMaxParticleCount(uint32 count) { _maxParticle = count; }

	void SetMinLifeTime(float time) { _minLifeTime = time; }
	void SetMaxLifeTime(float time) { _maxLifeTime = time; }
	void SetLifeTime(Vec2 time) { _minLifeTime = time.x; _maxLifeTime = time.y; }

	void SetMinSpeed(float time) { _minSpeed = time; }
	void SetMaxSpeed(float time) { _maxSpeed = time; }
	void SetSpeed(Vec2 time) { _minSpeed = time.x; _maxSpeed = time.y; }

	void SetStartScale(float time) { _startScale = time; }
	void SetEndScale(float time) { _endScale = time; }
	void SetScale(Vec2 time) { _startScale = time.x; _endScale = time.y; }

	void SetTexture(shared_ptr<class Texture> tex);

private:
	shared_ptr<StructuredBuffer>	_particleBuffer;
	shared_ptr<StructuredBuffer>	_computeSharedBuffer;
	uint32							_maxParticle = 1000;

	shared_ptr<Material>			_computeMaterial;
	shared_ptr<Material>			_material;
	shared_ptr<Mesh>				_mesh;

	float			_createInterval = 0.005f;
	float			_accTime = 0.0f;
	
	float			_minLifeTime = 0.5f;
	float			_maxLifeTime = 1.0f;
	float			_minSpeed = 100.0f;
	float			_maxSpeed = 50.0f;
	float			_startScale = 10.0f;
	float			_endScale = 5.0f;

};

