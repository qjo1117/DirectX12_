#include "pch.h"
#include "Timer.h"

void Timer::Init(HWND hWnd)
{
	_hWnd = hWnd;

	/* ----- 하드웨어의 갱신 빈도수를 구한다 ----- */
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void Timer::Update()
{
	/* ----- DeltaTime, Frame를 구해준다. ----- */
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	/* ----- DeltaTime ----- */
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	/* ----- Frame ----- */
	if (_frameTime > 1.f) {
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;

		//ShowFps();
	}

	/* ----- State ----- */
	switch (_state) {
	case TIMER_STATE::STOP:
		_deltaTime = 0.0f;
		break;
	case TIMER_STATE::FAST:
		_deltaTime *= _timeScale;
		break;
	case TIMER_STATE::SLOW:
		_deltaTime /= _timeScale;
		break;
	}
}

void Timer::ShowFps()
{
	uint32 fps = _fps;

	WCHAR text[15] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowTextW(_hWnd, text);
}


/*
	문제
	C++ 11에서 제공하는 Chrono를 쓰지 않고 QueryPerformanceCounter를 쓰는 이유
	ReleaseMode에서는 조금 더 나은 성능을 보여주지만
	DebugMode에서는 QueryPerformanceCounter가 8배 이상 더 좋은 성능을 낸다.
*/