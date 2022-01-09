#pragma once
#include "Component.h"


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
	Camera();
	virtual ~Camera();


	virtual void FinalUpdate() override;
	void Render();

private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 1.0f;
	float _far = 1000.0f;
	float _fov = XM_PI / 4.0f;
	float _scale = 1.0f;

	Matrix _matView = {};
	Matrix _matProjection = {};

public:
	static Matrix S_MatView;			// 한놈만 필요헤서 쓰는 녀석
	static Matrix S_MatProjection;		// 외부에서 쓰기 편하게 만든녀석
};

