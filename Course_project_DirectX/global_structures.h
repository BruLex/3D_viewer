#pragma once

//--------------------------------------------------------------------------------------
// ���������
//--------------------------------------------------------------------------------------
typedef struct {
	bool up;
	bool down;
	bool left;
	bool right;
	bool AOX;
	bool AOY;
	bool zoomPlus;
	bool zoomMinus;
}MOVE;

typedef struct {
	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;
	FLOAT scale;
}CurrentViewObject;

typedef struct {
	FLOAT xPos;
	FLOAT zPos;

	FLOAT xAngle;
	FLOAT yAngle;

	DWORD cntOfPoligons;
	DWORD cntOfIndex;

}CurrentPositionObject;

typedef struct
{
	XMFLOAT3 Pos;	 // ���������� ����� � ������������
	XMFLOAT2 TexCoord;     // ���������� ��������
	XMFLOAT3 Normal; // ������� �������
}SimpleVertex;

// ��������� ������������ ������ (��������� �� ���������� � �������)
typedef struct
{
	XMMATRIX mWorld;       // ������� ����
	XMMATRIX mView;        // ������� ����
	XMMATRIX mProjection;  // ������� ��������
}ConstantBuffer;

struct CacheEntry
{
	UINT index;
	CacheEntry* pNext;
};
