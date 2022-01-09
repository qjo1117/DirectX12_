#pragma once

#include "Scene.h"

/*---------------
	StartScene
----------------*/
class StartScene : public Scene
{
public:
	StartScene();
	virtual ~StartScene();

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;

private:
	shared_ptr<GameObject> _camera;
};

