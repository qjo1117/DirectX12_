#pragma once

struct WindowInfo
{
	HWND	hWnd; // ��� ������
	int32	width; // �ʺ�
	int32	height; // ����
	bool	windowed; // â��� or ��üȭ��
};

struct Vertex
{
	Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
};

struct TransformMatrix
{
	Matrix matWVP;
};