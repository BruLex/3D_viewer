#pragma once
#include "Course_project_DirectX.h"

//--------------------------------------------------------------------------------------
// Предварительные объявления функций
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);  // Создание окна
HRESULT InitDevice();	// Инициализация устройств DirectX

HRESULT InitSheders();
HRESULT InitModel(LPCSTR file);    // Инициализация шаблона ввода и буфера вершин
HRESULT InitGeometry_Textures();

HRESULT InitMatrixes();    // Инициализация матриц
HRESULT Read_file_obj(HWND g_hWnd, LPCSTR name_file, SimpleVertex* &vertices, WORD* &index, DWORD &cntOfPoligons, DWORD &cntOfIndex);
OPENFILENAME File_OBJ(HWND hWnd);

void UpdateLight();		// Обновление параметров света
void UpdateMatrix(UINT nLightIndex); // Обновление матрицы мира

void CleanupDevice();	// Удаление созданнных устройств DirectX
void Render();	// Функция рисования
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	  // Функция окна

void SetNewMatrixes(LPARAM lParam, MOVE moving);
FLOAT normalization(float angle);