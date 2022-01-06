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

	// TODO : 온갖 Parent / Child 관계 형성
private:
	// TODO : World 위치 관련
};

