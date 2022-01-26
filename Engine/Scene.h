#pragma once

class GameObject;

/*----------
	Scene
-----------*/
class Scene : public enable_shared_from_this<Scene>
{
public:
	/* ----- Constructor Function ----- */
	Scene();
	virtual ~Scene();

public:
	/* ----- External Function ----- */
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

	void Render();

	void ClearRTV();

	void RenderGameObjectDivision();

	void RenderShadow();
	void RenderDeferred();
	void RenderLights();
	void RenderFinal();

	void RenderFoward();

private:
	void PushLightData();

public:
	/* ----- Helper Function ----- */
	void AddGameObject(shared_ptr<GameObject> gameObject, LAYER_TYPE layer = LAYER_TYPE::DEFAULT);
	void RemoveObject(shared_ptr<GameObject> gameObject, LAYER_TYPE layer = LAYER_TYPE::END);

	const wstring& GetName() { return _name; }
	void SetName(const wstring& name) { _name = name; }

	array<vector<shared_ptr<GameObject>>, static_cast<uint32>(LAYER_TYPE::END)>& GetGameObjects() { return _gameObjects; }
	vector<shared_ptr<GameObject>>& GetGameObjects(LAYER_TYPE type) { return _gameObjects[static_cast<int32>(type)]; }
	vector<shared_ptr<GameObject>>& GetAllGameObjects() { return _objects; }
protected:
	/* ----- Helper Function ----- */
	bool RemoveObject(shared_ptr<GameObject> gameObject, uint32 layer);
	void AddGameObject(shared_ptr<GameObject> gameObject, uint32 layer);

private:
	/* ----- Scene Object Mapping Variable ----- */
	array<vector<shared_ptr<GameObject>>, static_cast<uint32>(LAYER_TYPE::END)> _gameObjects;
	vector<shared_ptr<GameObject>> _objects;		// 한번에 전체순회
	wstring _name;

	vector<shared_ptr<GameObject>>& _light;
	vector<shared_ptr<GameObject>>& _camera;
};

