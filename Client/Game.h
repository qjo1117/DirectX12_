#pragma once

class Game
{
public:

	void Init(const WindowInfo& info);
	void Update();
	void End();


private:
	WindowInfo _info;
};

