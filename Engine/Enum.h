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

	END
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = static_cast<uint8>(CBV_REGISTER::END),
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - static_cast<uint8>(CBV_REGISTER_COUNT),
	REGISTER_COUNT = static_cast<uint8>(CBV_REGISTER_COUNT) + static_cast<uint8>(SRV_REGISTER_COUNT),
};

/* --------------------------- KEY --------------------------- */

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,
	SPACE = VK_SPACE,

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
	// ...
	MONO_BEHAVIOUR,
	END,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1,
<<<<<<< Updated upstream
=======
};

/* --------------------------- SCNEN --------------------------- */

enum class LAYER_TYPE : uint32
{
	UI,
	CAMERA,

	// TO DO : 추가적으로 만들어준다.

	DEFAULT,
	END = 32
};

enum class SCENE_TYPE : uint32
{
	TOOL,

	START,
	STAGE_0,
	STAGE_1,
	STAGE_2,

	END,
>>>>>>> Stashed changes
};