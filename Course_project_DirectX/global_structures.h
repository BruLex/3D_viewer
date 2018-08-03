#pragma once

//--------------------------------------------------------------------------------------
// Структуры
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
	XMFLOAT3 Pos;	 // Координаты точки в пространстве
	XMFLOAT2 TexCoord;     // Координаты текстуры
	XMFLOAT3 Normal; // Нормаль вершины
}SimpleVertex;

// Структура константного буфера (совпадает со структурой в шейдере)
typedef struct
{
	XMMATRIX mWorld;       // Матрица мира
	XMMATRIX mView;        // Матрица вида
	XMMATRIX mProjection;  // Матрица проекции
}ConstantBuffer;

struct CacheEntry
{
	UINT index;
	CacheEntry* pNext;
};
