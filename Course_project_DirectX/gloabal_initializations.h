#pragma once
#include "Course_project_DirectX.h"

#define COUNT_OF_LIGHTS 4
//--------------------------------------------------------------------------------------
// Глобальные переменные
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;		// Устройство (для создания объектов)
ID3D11DeviceContext*    g_pImmediateContext = NULL;	// Контекст (устройство рисования)
IDXGISwapChain*         g_pSwapChain = NULL;		// Цепь связи (буфера с экраном)
ID3D11RenderTargetView* g_pRenderTargetView = NULL;	// Объект вида, задний буфер
ID3D11Texture2D*        g_pDepthStencil = NULL;		// Текстура буфера глубин
ID3D11DepthStencilView* g_pDepthStencilView = NULL;	// Объект вида, буфер глубин

ID3D11VertexShader*     g_pVertexShader = NULL;		// Вершинный шейдер
ID3D11PixelShader*      g_pPixelShader = NULL;		// Пиксельный шейдер для куба
ID3D11PixelShader*      g_pPixelShaderSolid = NULL;	// Пиксельный шейдер для источников света
ID3D11InputLayout*      g_pVertexLayout = NULL;		// Описание формата вершин
ID3D11Buffer*           g_pVertexBuffer = NULL;		// Буфер вершин
ID3D11Buffer*           g_pIndexBuffer = NULL;		// Буфер индексов вершин
ID3D11Buffer*           g_pConstantBuffer = NULL;	// Константный буфер

ID3D11Buffer*           g_pCBMatrixes = NULL;       // Константный буфер с информацией о матрицах
ID3D11Buffer*           g_pCBLight = NULL;          // Константный буфер с информацией о свете


XMMATRIX                g_World;					// Матрица мира
XMMATRIX                g_View;						// Матрица вида
XMMATRIX                g_Projection;				// Матрица проекции

FLOAT					t = 0.0f;					// Переменная-время
FLOAT					sensitivity = 100;           //Чувствительность мыши

XMFLOAT4				vLightDirs[6];				// Направление света (позиция источников)
XMFLOAT4				vLightColors[6];			// Цвет источников

ID3D11ShaderResourceView* g_pTextureRV = NULL;        // Объект текстуры
ID3D11SamplerState*       g_pSamplerLinear = NULL;    // Параметры наложения текстуры
CurrentPositionObject GlobalPos = { 0,0,0,0,0,0 };
CurrentViewObject GlobalView;

BOOL AnimationActivation = FALSE;
OPENFILENAME ofn = File_OBJ(g_hWnd);


