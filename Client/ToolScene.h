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

	void Create();

public:
	/* ----- Test or GUI Function ----- */
	void	TestEditor();
	void	TestObjects();

	void	TestButtonCreateObject();

	/* ----- 너무 길어서 각 컴포넌트 별로 함수를 만들어줌 ----- */
	void	TransformComponent();
	void	MeshRendererComponent();
	void	CameraComponent();
	void	LightComponent();

private:
	void	RemoveSelectObject();
	void	CreateCubeGameObject();

private:
	/* ----- Fick Variable ----- */
	shared_ptr<GameObject>	_pick;
	int32					_isPick = -1;
	shared_ptr<GameObject>	_dragPick;

	shared_ptr<GameObject>	_player;
	shared_ptr<GameObject>	_camera;

	ToolState				_state;

	Random					_random;

	vector<function<void()>> _createList;

	char					_text[50];
};

