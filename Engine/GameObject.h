#pragma once

class Component;
class Transform;
class MeshRenderer;
class MonoBehaviour;

/*---------------
	GameObject
----------------*/
class GameObject : public enable_shared_from_this<GameObject>
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

	/* ----- Helper Function ----- */
	shared_ptr<Transform> GetTransform();
	void AddComponent(shared_ptr<Component> component);

private:
	/* ----- Component Variable ----- */
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehaviour>> _scripts;

};

