#pragma once
#include "Component.h"

struct TransformMatrix
{
	Vec4 offset;
};

/*--------------
	Transform
---------------*/
class Transform : public Component
{
public:
	/* ----- Constructor Function ----- */
	Transform();
	virtual ~Transform();

	// TODO : �°� Parent / Child ���� ����
private:
	// TODO : World ��ġ ����
};

