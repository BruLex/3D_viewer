#pragma once

#include "resource.h"
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
// Файлы заголовков DirectX
#include <D3D11.h>
#include <D3DX11.h>
#include <d3dcompiler.h> 
#include <xnamath.h>

//Файлы заголовков для чтения obj
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <Psapi.h>

#include "gloabal_structures.h"
#include "functions.h"