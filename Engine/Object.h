#pragma once

enum class OBJECT_TYPE : uint8
{
	NONE,
	GAMEOBJECT,		// PREFAB
	COMPONENT,
	MATERIAL,
	MESH,
	SHADER,
	TEXTURE,

	// TODO

	END
};

enum
{
	OBJECT_TYPE_COUNT = static_cast<uint8>(OBJECT_TYPE::END)
};

/*-----------
	Object
------------*/
class Object
{
public:
	Object(OBJECT_TYPE type);
	virtual ~Object();

	Object GetType() { return _objectType; }

	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }

	// TODO : Intantiate 복사본 역할

protected:
	friend class Resources;
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:
	OBJECT_TYPE _objectType = OBJECT_TYPE::NONE;
	wstring _name;
};

