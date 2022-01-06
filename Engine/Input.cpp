#include "pch.h"
#include "Input.h"

void Input::Init(HWND hWnd)
{
	_hWnd = hWnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void Input::Update()
{
	/* ----- ���� �����츦 �����ߴ��� ----- */
	HWND hWnd = ::GetActiveWindow();
	// ���Ʈ�������� GetFouce�� ����

	/* ----- ���� �����찡 ������ �ȵ� ���¶�� KEY_STATE�� �ʱ�ȭ���ش�. ----- */
	if (hWnd != _hWnd) {
		for (uint32 key = 0; key < KEY_STATE_COUNT; key++) {
			_states[key] = KEY_STATE::NONE;
		}

		return;
	}

	/* ----- �ѹ��� �� Ű�� ���ȴ��� Ȯ���Ѵ�. ----- */
	if (::GetKeyboardState(_ascilKeys) == false) {
		return;
	}

	/* ----- �� Ű���� ���ε� ���¸� Ȯ���Ѵ�. ----- */
	for (uint32 key = 0; key < KEY_TYPE_COUNT; ++key) {
		if (_ascilKeys[key] & 0x80) {
			KEY_STATE& state = _states[key];

			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN) {
				state = KEY_STATE::PRESS;		// ���
			}
			else {
				state = KEY_STATE::DOWN;		// ó��
			}
		}
		else {
			KEY_STATE& state = _states[key];

			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN) {
				state = KEY_STATE::UP;			// ó�� UP
			}
			else {
				state = KEY_STATE::NONE;		// �ƿ� ����
			}
		}
	}

}


/*
	 ----- �ѹ��� �� Ű�� ���ȴ��� Ȯ���Ѵ�. ----- 
	if (::GetKeyboardState(_ascilKeys) == false) {
		return;
	}
	�׳� ::GetAsyncState�� 256�� �ϴ� �ͺ��� ���� �������� �ٲ�����
	�ʱ� FPS 1400 -> FPS 4000
	���� �ٲ�� ��������� �� �� ����
*/