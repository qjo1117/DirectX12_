#pragma once
#include "Component.h"


enum class PROJECTION_TYPE
{
	PERSPECTIVE,		// ��������
	ORTHOGRAPHIC,		// ��������
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
	static Matrix S_MatView;			// �ѳ� �ʿ��켭 ���� �༮
	static Matrix S_MatProjection;		// �ܺο��� ���� ���ϰ� ����༮
};

