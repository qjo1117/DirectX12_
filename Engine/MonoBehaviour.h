#pragma once
#include "Component.h"

/*------------------
	MonoBehaviour
-------------------*/
class  MonoBehaviour abstract : public Component
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
	virtual void FinalUpdate() sealed { }

public:
	/* ----- GUI Function ----- */
	virtual void DrawGUI() { }
};

