#pragma once
#include "Course_project_DirectX.h"

//--------------------------------------------------------------------------------------
// ��������������� ���������� �������
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);  // �������� ����
HRESULT InitDevice();	// ������������� ��������� DirectX

HRESULT InitSheders();
HRESULT InitModel(LPCSTR file);    // ������������� ������� ����� � ������ ������
HRESULT InitGeometry_Textures();

HRESULT InitMatrixes();    // ������������� ������
HRESULT Read_file_obj(HWND g_hWnd, LPCSTR name_file, SimpleVertex* &vertices, WORD* &index, DWORD &cntOfPoligons, DWORD &cntOfIndex);
OPENFILENAME File_OBJ(HWND hWnd);

void UpdateLight();		// ���������� ���������� �����
void UpdateMatrix(UINT nLightIndex); // ���������� ������� ����

void CleanupDevice();	// �������� ���������� ��������� DirectX
void Render();	// ������� ���������
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	  // ������� ����

void SetNewMatrixes(LPARAM lParam, MOVE moving);
FLOAT normalization(float angle);