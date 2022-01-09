#pragma once

#include "Object.h"

class GameObject;
class Transform;

/*--------------
	Component
---------------*/
class Component : public Object
{
public:
	/* ----- Constructor Function ----- */
	Component(COMPONENT_TYPE type);
	virtual ~Component();

public:
	/* ----- Virtual Function ----- */
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void FinalUpdate() { }

public:
	/* ----- Helper Function ----- */
	COMPONENT_TYPE			GetType() { return _type; }
	bool					IsValid() { return _gameObject.expired() == false; }

	shared_ptr<GameObject>	GetGameObject();
	shared_ptr<Transform>	GetTransform();

private:
	/* ----- Friend Helper Function ----- */
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	/* ----- Component Variable ----- */
	COMPONENT_TYPE			_type;
	weak_ptr<GameObject>	_gameObject;
};

