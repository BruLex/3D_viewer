#include "Course_project_DirectX.h"
#include "gloabal_initializations.h"

#define MX_SETWORLD 0x10001

//--------------------------------------------------------------------------------------
// Функция структуры для открытия файла
//--------------------------------------------------------------------------------------
OPENFILENAME File_OBJ(HWND hWnd)
{
	OPENFILENAME ofn;
	PSTR FileName = new char[200];
	ZeroMemory(&ofn, sizeof(ofn)); // очистим структуру
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = FileName;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 9999;
	ofn.lpstrFilter = "Obj файлы\0 * .obj\0";
	ofn.lpstrTitle = "Выберите файл obj для открытия.";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	return ofn;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//Создание начального окна для загрузки объекта
	if (!GetOpenFileName(&ofn))
		return 0;
	//Создание главного окна программи
	if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	//Создание устройства DirectX
	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}
	//Создание буфера шейдеров
	if (FAILED(InitSheders()))
	{
		CleanupDevice();
		return 0;
	}
	// Создание буфера вершин и инициализация объекта
	if (FAILED(InitModel(ofn.lpstrFile)))
	{
		CleanupDevice();
		return 0;
	}
	//Создание текстурного буфера
	if (FAILED(InitGeometry_Textures()))
	{
		CleanupDevice();
		return 0;
	}
	// Инициализация матриц вида
	if (FAILED(InitMatrixes()))
	{
		CleanupDevice();
		return 0;
	}

	// Главный цикл сообщений
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else	// Если сообщений нет
		{
			Render();	// Рисуем
		}
	}

	CleanupDevice();
	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// Регистрация класса и создание окна
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Регистрация классa
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_COURSEPROJECTDIRECTX);
	wcex.lpszClassName = "WindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_COURSEPROJECTDIRECTX);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1024, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow("WindowClass", "Course Project_Dracaris_fire", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return E_FAIL;
	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
} 

//--------------------------------------------------------------------------------------
// Функция обработки сообщений диалогового окна
//--------------------------------------------------------------------------------------
BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG: {
		// Различные инициализации
		return TRUE;
	}
	case WM_COMMAND: {
		// Идентификатор элемента управления
		int id = LOWORD(wParam);
		int code = HIWORD(wParam); // Код уведомления
		if (id == IDCANCEL) {
			// Нажаты или ESC, или пункт
			// меню IDCANCEL, или кнопка IDCANCEL
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		}
		if (id == IDOK) {
			// Нажата кнопка IDOK или пункт меню IDOK
			EndDialog(hDlg, IDOK);
			return TRUE;
		}
		break;
	};
	case WM_CLOSE: { // Закрытие окна при помощи системного меню
					 // Эмуляция отказа от работы с панелью
		PostMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
		return TRUE;
	};
	}
	return FALSE;
}
//--------------------------------------------------------------------------------------
// Функция обработки сообщений окна
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	MOVE action;
	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		SetNewMatrixes(lParam, action); // Обновить матрицу мира
		break;
	case WM_LBUTTONUP:
		ShowCursor(1);
		break;
	case WM_LBUTTONDOWN:
		ShowCursor(0);
		break;
	case WM_MOUSEMOVE: {
		UINT key_state = wParam; // Cостояние кнопок мыши
		if (key_state & MK_LBUTTON) {
			action.AOX = true;
			action.AOY = true;
			SetNewMatrixes(lParam, action); // Обновить матрицу мира
		}
		break;
	}
	case WM_MOUSEWHEEL: {
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if (zDelta < 0) {
			action.zoomPlus = true;
		}
		else if (zDelta > 0) {
			action.zoomMinus = true;
		}
		SetNewMatrixes(lParam, action);
	}
	case WM_CHAR: {
		char code = (char)wParam; // ASCII-код символа
		switch (code) {
		case  VK_F5:
			AnimationActivation = !AnimationActivation;
			break;
		}
	}
	case WM_KEYDOWN: {
		int v_key = (int)wParam; // Код виртуальной клавиши
		UINT key_state = wParam; // Cостояние кнопок мыши
		switch (v_key) {
		case VK_RIGHT:
			action.right = true;
			break;
		case VK_LEFT:
			action.left = true;
			break;
		case VK_UP:
			action.up = true;
			break;
		case VK_DOWN:
			action.down = true;
			break;
		case VK_F5:
			AnimationActivation = !AnimationActivation;
			break;
		}
		SetNewMatrixes(lParam, action);
		break;
	}
	case WM_COMMAND: {
		// Проверка идентификатора командного пункта меню.
		switch (LOWORD(wParam)) {
		case IDM_LOAD:
			if (GetOpenFileName(&ofn))
				InitModel(ofn.lpstrFile);
			break;
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, DlgProc);
			break;
		case IDM_EXIT:
			PostQuitMessage(0);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

//--------------------------------------------------------------------------------------
// Вспомогательная функция для компиляции шейдеров в D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(LPCSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob;

	hr = D3DX11CompileFromFile((LPCSTR)szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Создание устройств Direct3D: девайса, контекста и цепи связи
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;	// получаем ширину
	UINT height = rc.bottom - rc.top;	// и высоту окна
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	// Тут мы создаем список поддерживаемых версий DirectX
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	// Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
	// которая описывает свойства переднего буфера и привязывает его к нашему окну.
	DXGI_SWAP_CHAIN_DESC sd;		// Структура, описывающая цепь связи (Swap Chain)
	ZeroMemory(&sd, sizeof(sd));	// очищаем ее
	sd.BufferCount = 1;					// у нас один задний буфер
	sd.BufferDesc.Width = width;		// ширина буфера
	sd.BufferDesc.Height = height;		// высота буфера
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// формат пикселя в буфере
	sd.BufferDesc.RefreshRate.Numerator = 75;			// частота обновления экрана
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// назначение буфера - задний буфер
	sd.OutputWindow = g_hWnd;							// привязываем к нашему окну
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;						// не полноэкранный режим

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))  // Если устройства созданы успешно, то выходим из цикла
			break;
	}
	if (FAILED(hr)) return hr;

	// Теперь создаем задний буфер
	// RenderTargetOutput - это передний буфер, а RenderTargetView - задний.
	// Извлекаем описание заднего буфера
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;

	// По полученному описанию создаем поверхность рисования
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;

	// Переходим к созданию буфера глубин
	// Создаем текстуру-описание буфера глубин
	D3D11_TEXTURE2D_DESC descDepth;	// Структура с параметрами
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;		// ширина и
	descDepth.Height = height;		// высота текстуры
	descDepth.MipLevels = 2;		// уровень интерполяции
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// формат (размер пикселя)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;		// вид - буфер глубин
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// При помощи заполненной структуры-описания создаем объект текстуры
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, NULL, &g_pDepthStencil);
	if (FAILED(hr)) return hr;

	// Теперь надо создать сам объект буфера глубин
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;	// Структура с параметрами
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;		// формат как в текстуре
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// При помощи заполненной структуры-описания и текстуры создаем объект буфера глубин
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr)) return hr;

	// Подключаем объект заднего буфера и объект буфера глубин к контексту устройства
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	// Установки вьюпорта (масштаб и система координат).
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);
	return S_OK;
}
//--------------------------------------------------------------------------------------
// Создание буфера шейдеров (shaders) 
//--------------------------------------------------------------------------------------
HRESULT InitSheders()
{
	HRESULT hr = S_OK;
	// Компиляция вершинного шейдера из файла
	ID3DBlob* pVSBlob = NULL; // Вспомогательный объект - просто место в оперативной памяти
	hr = CompileShaderFromFile("sheder.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", "Ошибка", MB_OK);
		return hr;
	}

	// Создание вершинного шейдера
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Определение шаблона вершин
	D3D11_INPUT_ELEMENT_DESC layout[] ={
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }};
	UINT numElements = ARRAYSIZE(layout);
	// Создание шаблона вершин
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr)) return hr;

	// Подключение шаблона вершин
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
	// Компиляция пиксельного шейдера для основного большого куба из файла
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile("sheder.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", "Ошибка", MB_OK);
		return hr;
	}
	// Создание пиксельного шейдера
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;
	// Компиляция пиксельного шейдера для источников света из файла
	pPSBlob = NULL;
	hr = CompileShaderFromFile("sheder.fx", "PSSolid", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", "Ошибка", MB_OK);
		return hr;
	}
	// Создание пиксельного шейдера
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShaderSolid);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;
	return hr;
}

//--------------------------------------------------------------------------------------
// Создание буфера вершин, шейдеров (shaders) и описания формата вершин (input layout)
//--------------------------------------------------------------------------------------
HRESULT InitModel(LPCSTR file)
{
	HRESULT hr = S_OK;
	SimpleVertex* vertices = NULL;
	WORD* indices = NULL;
	GlobalPos.cntOfPoligons = 0;
	GlobalPos.cntOfIndex = 0;

	hr = Read_file_obj(g_hWnd, file, vertices, indices, GlobalPos.cntOfPoligons, GlobalPos.cntOfIndex);

	if (FAILED(hr))
	{
		MessageBox(NULL, "Ошибка чтения файла 3d модели obj.", "Ошибка", MB_OK);
		return hr;
	}
	D3D11_BUFFER_DESC bd;	// Структура, описывающая создаваемый буфер
	ZeroMemory(&bd, sizeof(bd));				// очищаем ее
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * GlobalPos.cntOfPoligons;	// размер буфера
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// тип буфера - буфер вершин
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;	// Структура, содержащая данные буфера
	ZeroMemory(&InitData, sizeof(InitData));	// очищаем ее
	InitData.pSysMem = vertices;				// указатель на вершини
												// Вызов метода g_pd3dDevice создаст объект буфера вершин
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr)) return hr;
	bd.Usage = D3D11_USAGE_DEFAULT;		// Структура, описывающая создаваемый буфер
	bd.ByteWidth = sizeof(WORD) * GlobalPos.cntOfIndex;	// для 6 треугольников необходимо 18 вершин
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER; // тип - буфер индексов
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;				// указатель на наш массив индексов
											// Вызов метода g_pd3dDevice создаст объект буфера индексов
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr)) return hr;

	// Установка буфера вершин
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	// Установка буфера индексов
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Установка способа отрисовки вершин в буфере (в данном случае - TRIANGLE LIST,
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static bool a = 0;
	// Создание константного буфера
	if (!a) {
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ConstantBuffer);		// размер буфера = размеру структуры
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// тип - константный буфер
		bd.CPUAccessFlags = 0;
		hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
		if (FAILED(hr)) return hr;
		a = 1;
	}
	free(vertices);
	free(indices);
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Создание текстурного буфера буфера 
//--------------------------------------------------------------------------------------
HRESULT InitGeometry_Textures()
{
	HRESULT hr = S_OK;
	LPCSTR file = "texture.png";

	// Загрузка текстуры из файла
	hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, file, NULL, NULL, &g_pTextureRV, NULL);
	if (FAILED(hr)) return hr;
	// Создание сэмпла (описания) текстуры
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// Тип фильтрации
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// Задаем координаты
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Создаем интерфейс сэмпла текстурирования
	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	if (FAILED(hr)) return hr;
	g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Инициализация матриц
//--------------------------------------------------------------------------------------
HRESULT InitMatrixes()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;	// получаем ширину
	UINT height = rc.bottom - rc.top;	// и высоту окна
	// Инициализация матрицы мира
	g_World = XMMatrixIdentity();
	// Инициализация матрицы вида
	GlobalView = {
	XMVectorSet(0.0f, 500.0f, -500.0f, 0.0f),	
	XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),	
	XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
	500.0f};
	g_View = XMMatrixLookAtLH(GlobalView.Eye, GlobalView.At, GlobalView.Up);
	// Инициализация матрицы проекции
	// Параметры: 1) ширина угла объектива 2) "квадратность" пикселя
	// 3) самое ближнее видимое расстояние 4) самое дальнее видимое расстояние
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 10000.0f);
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Рисование кадра
//--------------------------------------------------------------------------------------

void Render()
{
	// Очистить задний буфер
	float ClearColor[4] = { 0.7f,  0.7f,  0.7f, 1.0f }; // красный, зеленый, синий, альфа-канал
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	UpdateLight();
	UpdateMatrix(MX_SETWORLD);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);
	//Рисуем центральную фигуру
	g_pImmediateContext->DrawIndexed(GlobalPos.cntOfIndex, 0, 0);

	g_pImmediateContext->PSSetShader(g_pPixelShaderSolid, NULL, 0);
	for (int m = 0; m < COUNT_OF_LIGHTS; m++)
	{
		//  Устанавливаем матрицу мира источника света
		UpdateMatrix(m);
	}
	// Вывести в передний буфер (на экран) информацию, нарисованную в заднем буфере.
	g_pSwapChain->Present(0, 0);
}

//--------------------------------------------------------------------------------------
// Удалить все созданные объекты
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	if (g_pSamplerLinear) g_pSamplerLinear->Release();
	if (g_pTextureRV) g_pTextureRV->Release();
	if (g_pConstantBuffer) g_pConstantBuffer->Release();
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pIndexBuffer) g_pIndexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShaderSolid) g_pPixelShaderSolid->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pDepthStencil) g_pDepthStencil->Release();
	if (g_pDepthStencilView) g_pDepthStencilView->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}

//--------------------------------------------------------------------------------------
// Вращение объекта с помощю мыши
//--------------------------------------------------------------------------------------
void SetNewMatrixes(LPARAM lParam, MOVE moving)
{
	XMMATRIX translation;
	XMMATRIX rotation;
	POINT mousePos;
	RECT rc;
	GetWindowRect(g_hWnd, &rc);
	int middleX = ((rc.right - rc.left) / 2) + rc.left;   // Вычисляем половину ширины
	int middleY = ((rc.bottom - rc.top) / 2) + rc.top;
	mousePos.x = GET_X_LPARAM(lParam); // Горизонтальная позиция курсора
	mousePos.y = GET_Y_LPARAM(lParam); // Вертикальная позиция курсора
	POINT s;
	GetCursorPos(&s);
	// Теперь, получив координаты курсора, возвращаем его обратно в середину.
	if (moving.AOY)
		GlobalPos.yAngle += normalization(middleX - s.x) / (XM_PI / 2.0f);
	if (moving.AOX)
		GlobalPos.xAngle += normalization(middleY - s.y) / (XM_PI / 2.0f);
	rotation *= XMMatrixRotationX(GlobalPos.xAngle);
	rotation *= XMMatrixRotationY(GlobalPos.yAngle);
	SetCursorPos(middleX, middleY);

	if (moving.left) {
		translation = XMMatrixTranslation(--GlobalPos.xPos, 0.0f, GlobalPos.zPos);
	}
	else if (moving.right) {
		translation = XMMatrixTranslation(++GlobalPos.xPos, 0.0f, GlobalPos.zPos);
	}
	else if (moving.up) {
		translation = XMMatrixTranslation(GlobalPos.xPos, 0.0f, ++GlobalPos.zPos);
	}
	else if (moving.down) {
		translation = XMMatrixTranslation(GlobalPos.xPos, 0.0f, --GlobalPos.zPos);
	}
	else {
		translation = XMMatrixTranslation(GlobalPos.xPos, 0.0f, GlobalPos.zPos);
	}
	if (moving.zoomPlus && GlobalView.scale >20) {
		GlobalView.scale -= 20;
	}
	if (moving.zoomMinus) {
		GlobalView.scale += 20;
	}

	g_View = XMMatrixLookAtLH(XMVectorSet(0.0f, GlobalView.scale, -GlobalView.scale, 0.0f), GlobalView.At, GlobalView.Up);
	g_World *= rotation * translation * XMMatrixScaling(2.0f, 2.0f, 2.0f);

	// Обновить константный буфер
	// создаем временную структуру и загружаем в нее матрицы
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	// загружаем временную структуру в константный буфер g_pConstantBuffer
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
}

void UpdateMatrix(UINT nLightIndex)
{
	// Небольшая проверка индекса
	if (nLightIndex == MX_SETWORLD) {
		// Если рисуем центральный куб: его надо просто рисвовать
		if (AnimationActivation)
			g_World = XMMatrixTranslation(GlobalPos.xPos, 0.0f, GlobalPos.zPos)
			//*XMMatrixRotationRollPitchYaw(0.0f, t * 10, 0.0f)
			*XMMatrixScaling(2.0f, 2.0f, 2.0f);
		else
			g_World = XMMatrixTranslation(GlobalPos.xPos, 0.0f, GlobalPos.zPos)
			*XMMatrixRotationRollPitchYaw(GlobalPos.xAngle, GlobalPos.yAngle, 0.0f)
			*XMMatrixScaling(2.0f, 2.0f, 2.0f);
		nLightIndex = 0;
	}
	else if (nLightIndex < COUNT_OF_LIGHTS) {
		// Если рисуем источники света: перемещаем матрицу в точку и уменьшаем 
		g_World = XMMatrixTranslationFromVector(5000.0f * XMLoadFloat4(&vLightDirs[nLightIndex]));
		XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		g_World = mLightScale * g_World*XMMatrixRotationAxis(XMVectorSet(2.0f, 1.0f, 1.0f, 0.0f), t);
	}
	else {
		nLightIndex = 0;
	}
	if (AnimationActivation)
		g_View = XMMatrixRotationY(t)
		*XMMatrixLookAtLH(XMVectorSet(0.0f, GlobalView.scale, -GlobalView.scale, 0.0f), GlobalView.At, GlobalView.Up);
	// Обновление содержимого константного буфера
	ConstantBuffer cb1;	// временный контейнер
	cb1.mWorld = XMMatrixTranspose(g_World);	// загружаем в него матрицы
	cb1.mView = XMMatrixTranspose(g_View);
	cb1.mProjection = XMMatrixTranspose(g_Projection);
	for (int i = 0; i < COUNT_OF_LIGHTS; i++) {
		cb1.vLightDir[i] = vLightDirs[i];			// загружаем данные о свете
		cb1.vLightColor[i] = vLightColors[i];
	}
	cb1.vOutputColor = vLightColors[nLightIndex];
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);
}

void UpdateLight()
{
	// Обновление переменной-времени
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}
	static XMFLOAT4 lightDirs[COUNT_OF_LIGHTS]{
		XMFLOAT4((float)(rand() % 2001 - 1000) / 1000.0, (float)(rand() % 2001 - 1000) / 1000.0,(float)(rand() % 2001 - 1000) / 1000.0, 1.0f),
		XMFLOAT4((float)(rand() % 2001 - 1000) / 1000.0, (float)(rand() % 2001 - 1000) / 1000.0,(float)(rand() % 2001 - 1000) / 1000.0, 1.0f),
		XMFLOAT4((float)(rand() % 2001 - 1000) / 1000.0, (float)(rand() % 2001 - 1000) / 1000.0,(float)(rand() % 2001 - 1000) / 1000.0, 1.0f),
		XMFLOAT4((float)(rand() % 2001 - 1000) / 1000.0, (float)(rand() % 2001 - 1000) / 1000.0,(float)(rand() % 2001 - 1000) / 1000.0, 1.0f)
	};
	// Задаем начальные координаты источников света
	for (int i = 0; i < COUNT_OF_LIGHTS; i++) {
		vLightDirs[i] = lightDirs[i];
	}
	// Задаем цвет источников света
	vLightColors[0] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vLightColors[1] = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	vLightColors[2] = XMFLOAT4(0.0f, 0.1f, 0.0f, 1.0f);
	vLightColors[3] = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	static float lol[COUNT_OF_LIGHTS] = {
		rand() % 3 - 1,  rand() % 3 - 1, rand() % 3 - 1, rand() % 3 - 1
	};
	for (int i = 0; i < COUNT_OF_LIGHTS; i++) {
		XMMATRIX mRotate = XMMatrixRotationY(lol[i] - 2.0f * t);
		XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[i]);
		vLightDir = XMVector3Transform(vLightDir, mRotate);
		XMStoreFloat4(&vLightDirs[i], vLightDir);
	}
}

FLOAT normalization(float angle) {
	if (angle > 0) {
		angle = sensitivity;
	}
	else if (angle < 0) {
		angle = -sensitivity;
	}
	return angle / 1000.0f;
}