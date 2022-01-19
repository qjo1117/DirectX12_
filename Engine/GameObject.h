#pragma once

#include "Component.h"
#include "Object.h"

class Component;
class Transform;
class MeshRenderer;
class MonoBehaviour;
class Camera;
class Light;

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

	/* ----- Helper Component Function ----- */
	inline shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();

	map<string, shared_ptr<MonoBehaviour>>& GetScripts() { return _scripts; }

	template<typename T>
	void AddComponent(shared_ptr<T> component);
	template<typename T>
	shared_ptr<T> GetComponent();
	template<typename T>
	shared_ptr<T> GetOrAddComponent();

	/* ----- Helper Function ----- */
	const string&	GetGUIName() { return _guiName; }
	void			SetGUIName(const string& name) { _guiName = name; }
	void			SetGUIName(const wstring& name) { const wchar_t* wName = name.data(); char strName[MAX_PATH]; ::memset(strName, 0, MAX_PATH); ::WideCharToMultiByte(CP_ACP, 0, wName, -1, strName, lstrlenW(wName), 0, 0); SetGUIName(strName); SetName(name); }

	uint32			GetLayer() { return _layerType; }
	void			SetLayer(uint32 type) { _layerType = type; }

	bool			GetCheckFrustum() { return _isFrustum; }
	void			SetCheckFrustum(bool check) { _isFrustum = check; }

	bool			GetActive() { return _isActive; }
	void			SetActive(bool active) { _isActive = active; }
private:
	uint32 _layerType = 0;

	/* ----- Component Variable ----- */
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	map<string, shared_ptr<MonoBehaviour>> _scripts;

	string _guiName = "Unknown";

	bool _isFrustum = true;
	bool _isActive = true;
public:
	static uint32 ID;
};

template<typename T>
inline void GameObject::AddComponent(shared_ptr<T> component)
{
	if (!std::is_convertible_v<T, Component>) {
		return;
	}

	/* ----- 컴포넌트의 주인을 알려주고 맵핑번호가 넘어가면 MonoBehaviour라고 취급 ----- */
	component->SetGameObject(shared_from_this());

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT) {
		_components[index] = component;
	}
	else {
		static char* namePointer = (char*)(&typeid(component).name()[28]);
		namePointer[::strlen(namePointer) - 1] = '\0';
		_scripts.insert(pair<string, shared_ptr<MonoBehaviour>>(namePointer, dynamic_pointer_cast<MonoBehaviour>(component)));
	}
}

template<typename T>
inline shared_ptr<T> GameObject::GetComponent()
{
	uint8 index = static_cast<uint8>(GetType());

	if (index < FIXED_COMPONENT_COUNT) {
		return static_pointer_cast<T>(_components[index]);
	}

	auto findIt = _scripts.find((char*)(&typeid(T).name()[6]));

	if (findIt == _scripts.end()) {
		return nullptr;
	}
	return static_pointer_cast<T>(findIt->second);
}

template<typename T>
inline shared_ptr<T> GameObject::GetOrAddComponent()
{
	shared_ptr<T> component = GetComponent<T>();
	if (component == nullptr) {
		component = make_shared<T>();
		AddComponent(component);
	}
	return component;
}