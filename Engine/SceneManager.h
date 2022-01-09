#pragma once



class Scene;


/*-----------------
	SceneManager
------------------*/
class SceneManager
{
private:
	/* ----- SingleTon ----- */
	DECLARE_SINGLE(SceneManager)

public:
	/* ----- External Function ----- */
	void Update();
	void Render();
	void LoadScene(SCENE_TYPE type);

public:
	/* ----- Template Function ----- */
	template<typename T>
	void CreateScene(SCENE_TYPE type)
	{
		uint32 index = static_cast<uint32>(type);
		assert(_scenes[static_cast<uint32>(type)] == nullptr);

		shared_ptr<T> scene = make_shared<T>();

		if (_scenes[index] == nullptr) {
			_scenes[index] = scene;
		}
	}

public:
	/* ----- Helper Function ----- */
	shared_ptr<Scene>	GetCurrentScene() { return _currentScene; }
	SCENE_TYPE			GetType() { return _type; }
private:
	/* ----- Scene Variable ----- */
	shared_ptr<Scene> _currentScene;
	array<shared_ptr<Scene>, static_cast<uint32>(SCENE_TYPE::END)> _scenes;
	SCENE_TYPE _type;
};


/* ----------------------------------------
	TODO :	Load�Ҷ� ���빰 Ŭ�����ϴ� ��
			Change�Ҷ� ���빰 Ŭ�����ϴ� ��
			��File���� �����͸� �ε��
---------------------------------------- */
