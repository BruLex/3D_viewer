#pragma once
#include "Course_project_DirectX.h"

#define COUNT_OF_LIGHTS 4
//--------------------------------------------------------------------------------------
// ���������� ����������
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;		// ���������� (��� �������� ��������)
ID3D11DeviceContext*    g_pImmediateContext = NULL;	// �������� (���������� ���������)
IDXGISwapChain*         g_pSwapChain = NULL;		// ���� ����� (������ � �������)
ID3D11RenderTargetView* g_pRenderTargetView = NULL;	// ������ ����, ������ �����
ID3D11Texture2D*        g_pDepthStencil = NULL;		// �������� ������ ������
ID3D11DepthStencilView* g_pDepthStencilView = NULL;	// ������ ����, ����� ������

ID3D11VertexShader*     g_pVertexShader = NULL;		// ��������� ������
ID3D11PixelShader*      g_pPixelShader = NULL;		// ���������� ������ ��� ����
ID3D11PixelShader*      g_pPixelShaderSolid = NULL;	// ���������� ������ ��� ���������� �����
ID3D11InputLayout*      g_pVertexLayout = NULL;		// �������� ������� ������
ID3D11Buffer*           g_pVertexBuffer = NULL;		// ����� ������
ID3D11Buffer*           g_pIndexBuffer = NULL;		// ����� �������� ������
ID3D11Buffer*           g_pConstantBuffer = NULL;	// ����������� �����

ID3D11Buffer*           g_pCBMatrixes = NULL;       // ����������� ����� � ����������� � ��������
ID3D11Buffer*           g_pCBLight = NULL;          // ����������� ����� � ����������� � �����


XMMATRIX                g_World;					// ������� ����
XMMATRIX                g_View;						// ������� ����
XMMATRIX                g_Projection;				// ������� ��������

FLOAT					t = 0.0f;					// ����������-�����
FLOAT					sensitivity = 100;           //���������������� ����

XMFLOAT4				vLightDirs[6];				// ����������� ����� (������� ����������)
XMFLOAT4				vLightColors[6];			// ���� ����������

ID3D11ShaderResourceView* g_pTextureRV = NULL;        // ������ ��������
ID3D11SamplerState*       g_pSamplerLinear = NULL;    // ��������� ��������� ��������
CurrentPositionObject GlobalPos = { 0,0,0,0,0,0 };
CurrentViewObject GlobalView;

BOOL AnimationActivation = FALSE;
OPENFILENAME ofn = File_OBJ(g_hWnd);


