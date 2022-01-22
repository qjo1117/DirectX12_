#pragma once

/* --------------------------- REGISTER --------------------------- */

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	t7,
	t8,
	t9,

	END
};

enum class UAV_REGISTER : uint8
{
	u0 = static_cast<uint8>(SRV_REGISTER::END),
	u1,
	u2,
	u3,
	u4,

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = static_cast<uint8>(CBV_REGISTER::END),
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - static_cast<uint8>(CBV_REGISTER_COUNT),
	CBV_SRV_REGISTER_COUNT = static_cast<uint8>(CBV_REGISTER_COUNT) + static_cast<uint8>(SRV_REGISTER_COUNT),
	UAV_REGISTER_COUNT = static_cast<uint8>(UAV_REGISTER::END),
	TOTAL_REGISTER_COUNT = static_cast<uint8>(CBV_SRV_REGISTER_COUNT) + static_cast<uint8>(UAV_REGISTER_COUNT),
};

/* --------------------------- KEY --------------------------- */

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	SPACE = VK_SPACE,
	DEL = VK_DELETE,
	SHIFT = VK_SHIFT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	Q = 'Q',
	E = 'E',
	Z = 'Z',
	C = 'C',

	R = 'R',
	T = 'T',
	I = 'I',
	J = 'J',
	O = 'O',
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END)
};

/* --------------------------- TIMER --------------------------- */

enum class TIMER_STATE
{
	NONE,
	STOP,
	FAST,
	SLOW
};

/* --------------------------- COMPONENT --------------------------- */

enum class COMPONENT_TYPE : uint8
{
	TRANSFORM,
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	PARTICLE_SYSTEM,
	// ...
	MONO_BEHAVIOUR,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1,
};

/* --------------------------- SCNEN --------------------------- */

enum class SCENE_TYPE : uint32
{
	TOOL,

	START,
	STAGE_0,
	STAGE_1,
	STAGE_2,

	END,
};