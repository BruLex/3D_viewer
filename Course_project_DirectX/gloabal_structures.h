#pragma once
#include "Course_project_DirectX.h"
//--------------------------------------------------------------------------------------
// Структуры
//--------------------------------------------------------------------------------------
typedef struct {
	bool up  = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool AOX = false;
	bool AOY = false;
	bool zoomPlus = false;
	bool zoomMinus = false;
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
	XMFLOAT3 Pos;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Normal; 
}SimpleVertex;

typedef struct
{
	XMMATRIX mWorld;       // Матрица мира
	XMMATRIX mView;        // Матрица вида
	XMMATRIX mProjection;  // Матрица проекции
	XMFLOAT4 vLightDir[6];	// Направление света
	XMFLOAT4 vLightColor[6];// Цвет источника
	XMFLOAT4 vOutputColor;	// Активный цвет (для второго PSSolid)
}ConstantBuffer;

