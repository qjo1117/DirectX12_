#pragma once
#include "MonoBehaviour.h"

class CameraController : public MonoBehaviour
{
public:
	/* ----- Constructor Function ----- */
	CameraController();
	virtual ~CameraController();

public:
	/* ----- virtual Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
public:
	/* ----- GUI Function ----- */
	virtual void DrawGUI();
};

