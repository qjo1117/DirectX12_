#pragma once 

#include "Monobehaviour.h"



class Billboard : public MonoBehaviour
{
public:
	/* ----- Constructor Function ----- */
	Billboard();
	virtual ~Billboard();

public:
	/* ----- virtual Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

public:
	/* ----- GUI Function ----- */
	virtual void DrawGUI();
	
public:
	void SetScene(shared_ptr<class Scene> scene) { _scene = scene; }

private:
	shared_ptr<class GameObject>	_camera;
	shared_ptr<class Scene>			_scene;

	PUBLIC_PROPERTY(int32, Hp);		// ±èÄ¡»ï ±³¼ö´ÔÀÌ ¾Ë·ÁÁÖ½Å¹æ¹ý
};

