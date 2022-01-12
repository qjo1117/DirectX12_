#pragma once

/*----------
	Timer
-----------*/
class Timer
{
private:
	DECLARE_SINGLE(Timer)

public:
	/* ----- External Function ----- */
	void Init(HWND hWnd);
	void Update();

	/* ----- Helper Function ----- */
	uint32	GetFps() { return _fps; }
	float	GetDeltaTime() { return _deltaTime; }

	void	SetTimeScale(float timeScale) { _timeScale = timeScale; }

	/* ----- Time State Function ----- */
	void	Stop() { _state = TIMER_STATE::STOP; }
	void	Run() { _state = TIMER_STATE::NONE; }
	void	Slow() { _state = TIMER_STATE::SLOW; }
	void	Fast() { _state = TIMER_STATE::FAST; }

public:
	void ShowFps();

private:
	/* ----- Frequency Variable ----- */
	uint64		_frequency = 0;
	uint64		_prevCount = 0;
	float		_deltaTime = 0.0f;

private:
	/* ----- Visiable Frame Variable ----- */
	uint32		_frameCount = 0;
	float		_frameTime = 0.0f;
	uint32		_fps = 0;

private:
	/* ----- 시간이 변할때 쓰는 변수 ----- */
	float		_timeScale = 2.0f;
	TIMER_STATE	_state = TIMER_STATE::NONE;

private:
	/* ----- Mapping ----- */
	HWND		_hWnd;
};

