#pragma once
#include "Scene.h"


struct ToolState
{
	bool transform = false;
};

/*--------------
	ToolScene
---------------*/
class ToolScene : public Scene
{
public:
	/* ----- Constract Function ----- */
	ToolScene();
	virtual ~ToolScene();

public:
	/* ----- External Function ----- */
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate() override;

public:
	/* ----- Test or GUI Function ----- */
	void TestEditor();
	void TestObjects();

	void TestButtonCreateObject();

private:
	shared_ptr<GameObject> _pick;

	shared_ptr<GameObject> _player;
	shared_ptr<GameObject> _camera;

	shared_ptr<GameObject> _obj1;
	shared_ptr<GameObject> _obj2;

	ToolState _state;
};

