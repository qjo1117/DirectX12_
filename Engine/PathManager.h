#pragma once




class PathManager
{
private:
	DECLARE_SINGLE(PathManager);
public:
	void Init();
	void Clear();

	bool CreatePath(const string& key, const wstring& path, const string& basePathKey = BASE_PATH);
	const wstring& FindPath(const string& key);
	const string& FindPathMultiByte(const string& key);

private:
	unordered_map<string, wstring> _paths;
	char _strPath[MAX_PATH];
	
};

