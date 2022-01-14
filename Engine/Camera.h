#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,		// 원근투영
	ORTHOGRAPHIC,		// 직교투영
};

/*-----------
	Camera
------------*/
class Camera : public Component
{
public:
	/* ----- Constroct Function ----- */
	Camera();
	virtual ~Camera();

	/* ----- External Function ----- */
	virtual void FinalUpdate() override;
	void Render();

	/* ----- Helper Function ----- */
	float GetNear() { return _near; }
	float GetFar() { return _far; }

	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.0f;
	float _far = 1000.0f;
	float _fov = XM_PI / 4.0f;
	float _scale = 1.0f;

	Matrix _matView = {};
	Matrix _matProjection = {};

	Frustum _frustum;

public:
	static Matrix S_MatView;			
	static Matrix S_MatProjection;		// 외부에서 쓰기 편하게 만든녀석
};

