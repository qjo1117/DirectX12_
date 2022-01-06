#pragma once
#include "Component.h"

/*------------------
	MonoBehaviour
-------------------*/
class MonoBehaviour : public Component
{
public:
	/* ----- Constructor Function ----- */
	MonoBehaviour();
	virtual ~MonoBehaviour();

public:
	/* ----- virtual Function ----- */
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
};

