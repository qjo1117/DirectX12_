#pragma once
#include "Scene.h"


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
	void	TestResources();
	void	TestRenderTargetView();

	void	TestButtonCreateObject();

	/* ----- 너무 길어서 각 컴포넌트 별로 함수를 만들어줌 ----- */
	void	TransformComponent();
	void	MeshRendererComponent();
	void	CameraComponent();
	void	LightComponent();
	void	ParticleComponent();

private:
	void	RemoveSelectObject();
	void	CreateCubeGameObject();

private:
	void	ParentTree(const shared_ptr<GameObject>& obj);

private:
	/* ----- Fick Variable ----- */
	shared_ptr<class GameObject>		_goPick;
	shared_ptr<class Object>			_objPick;
	int32								_isPick = -1;
	shared_ptr<GameObject>				_dragPick;

	shared_ptr<GameObject>				_player;
	shared_ptr<GameObject>				_camera;

	Random								_random;

	vector<function<void()>>			_createList;

	vector < shared_ptr<GameObject>>	_renderTargetView;

	char								_text[MAX_PATH];
	uint64								_defauleImage;
};
