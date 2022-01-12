#include "pch.h"
#include "Timer.h"

void Timer::Init(HWND hWnd)
{
	_hWnd = hWnd;

	/* ----- �ϵ������ ���� �󵵼��� ���Ѵ� ----- */
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU Ŭ��
}

void Timer::Update()
{
	/* ----- DeltaTime, Frame�� �����ش�. ----- */
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
	����
	C++ 11���� �����ϴ� Chrono�� ���� �ʰ� QueryPerformanceCounter�� ���� ����
	ReleaseMode������ ���� �� ���� ������ ����������
	DebugMode������ QueryPerformanceCounter�� 8�� �̻� �� ���� ������ ����.
*/