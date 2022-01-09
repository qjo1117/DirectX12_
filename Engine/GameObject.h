#pragma once

#include "Component.h"
#include "Object.h"

class Component;
class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;

/*---------------
	GameObject
----------------*/
class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	/* ----- Constructor Function ----- */
	GameObject();
	virtual ~GameObject();

	/* ----- FrameWork Function ----- */
	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	/* ----- Helper Function ----- */
	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();

	vector<shared_ptr<MonoBehaviour>>& GetScripts() { return _scripts; }

	void AddComponent(shared_ptr<Component> component);

	const string& GetGUIName() { return _guiName; }
	void SetGUIName(const string& name) { _guiName = name; }

private:
	/* ----- Component Variable ----- */
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

	string _guiName = "Unknown";
	static uint32 ID;
};

