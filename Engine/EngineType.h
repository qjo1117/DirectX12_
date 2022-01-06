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
	Vec3 pos;
	Vec4 color;
	Vec2 uv;
};
