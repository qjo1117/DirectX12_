#pragma once
#include "MonoBehaviour.h"


class RotationXComponent : public MonoBehaviour
{
public:
	/* ----- Constructor Function ----- */
	RotationXComponent();
	virtual ~RotationXComponent();

public:
	/* ----- virtual Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
public:
	/* ----- GUI Function ----- */
	virtual void DrawGUI();

private:
	float _speedX = 1.0f;
	float _speedY = 1.0f;
	float _speedZ = 1.0f;
	static Random random;
};

