#pragma once


// WELL ������� ���� ���� ������
// ���� : ������ �׳��ϱ⿡�� �ɽ��ϰ� ������ ������ ���� ���°� �� ���ٰ� ����� ����
// �ı� : ���θ� ������...

/*------------------
*		Random
------------------*/
class Random
{
private:
	enum {
		DIFFER_VALUE = 100,
	};
public:
	Random() { Init(); }

	void Init() {
		_index = 0;
		uint64 seed = static_cast<uint64>(time(nullptr));
		for (int32 i = 0; i < 16; ++i) {
			_state[i] = seed;
			seed += seed + DIFFER_VALUE;
		}
	}

	void Init(uint64 uSeed) {
		_index = 0;
		uint64 seed = uSeed;
		for (int32 i = 0; i < 16; ++i) {
			_state[i] = seed;
			seed += seed + DIFFER_VALUE;
		}
	}

public:
	uint64 Randint(void) {
		uint64 a = _state[_index];
		uint64 c = _state[(_index + 13) & 15];
		uint64 b = a ^ c ^ (a << 16) ^ (c << 15);
		c = _state[(_index + 9) & 15];
		c ^= (c >> 11);
		a = _state[_index] = b ^ c;
		uint64 d = a ^ ((a << 5) & 0xda442d24U);
		_index = (_index + 15) & 15;
		a = _state[_index];
		_state[_index] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);

		return _state[_index];
	}

	uint64 Randint(uint64 min, uint64 max) {
		return min + (Randint() % (max - min));
	}
	float Rand() {
		uint64 value = Randint();
		// ����ü�� �̿��ؼ� float�� �ٲ��ش�.
		union { uint64 u64; float f; } unit;
		unit.u64 = (((value *= 16807) & 0x007fffff) - 1) | 0x3f800000;
		return unit.f - 1.0f;
	}
	float Rand(float min, float max) {
		return min + Rand() * (max - min);
	}

private:

	uint64 _index = 0;
	uint64 _state[16] = { 0 };
};
