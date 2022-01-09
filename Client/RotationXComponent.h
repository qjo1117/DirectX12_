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
	float _speed = 5.0f;
};

