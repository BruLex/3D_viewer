#include "Course_project_DirectX.h"

HRESULT GetIndex(SimpleVertex* selected, SimpleVertex* &vertices, WORD* &index, DWORD &cntOfPoligons, DWORD &cntOfIndex);
bool cmp(SimpleVertex *a, SimpleVertex *b);

XMFLOAT3*   Tmp_v_Coords = NULL;
XMFLOAT3*   Tmp_vn_Coords = NULL;
XMFLOAT2*   Tmp_vt_Coords = NULL;

HRESULT Read_file_obj(HWND g_hWnd, LPCSTR name_file, SimpleVertex* &vertices, WORD* &index, DWORD &cntOfPoligons, DWORD &cntOfIndex)
{
	std::string line;
	int  coords_v_index = 0,
		coords_vn_index = 0,
		coords_vt_index = 0; 

	std::fstream My_File_obj;
	HRESULT hr = S_OK;
	My_File_obj.open(name_file);
	if (My_File_obj.is_open()) //если открыли будем читать данные из файла
	{
		//перечитсывем файл до концы
		My_File_obj.seekg(0, std::ios::end);
		//получаем количество считанных из файла символов
		int  File_Size = My_File_obj.tellg();
		//задаем размер временного массива для координат
		Tmp_v_Coords = (XMFLOAT3*)malloc(File_Size);
		Tmp_vn_Coords = (XMFLOAT3*)malloc(File_Size);
		Tmp_vt_Coords = (XMFLOAT2*)malloc(File_Size);
		//устанавливаем курсор в начало файла
		My_File_obj.seekg(0, std::ios::beg);
		while (!My_File_obj.eof())
		{
			//читаем строку текста из файла
			getline(My_File_obj, line);
			if ((line.c_str()[0] == 'v') && (line.c_str()[1] == ' '))
			{
				//обнуляем 1 символ
				line[0] = ' ';
				//обнуляем 1 символ
				line[1] = ' ';
				//чтобы функция правельно разбирала координаты обнуляем начало строки
				sscanf_s(line.c_str(), "%f %f %f ",
					&Tmp_v_Coords[coords_v_index].x,   //координата X
					&Tmp_v_Coords[coords_v_index].y, //координата Y
					&Tmp_v_Coords[coords_v_index].z  //координата Z
				);
				coords_v_index++;
			}
			if ((line.c_str()[0] == 'v') && (line.c_str()[1] == 'n'))
			{
				//обнуляем 1 символ
				line[0] = ' ';
				//обнуляем 1 символ
				line[1] = ' ';
				//чтобы функция правельно разбирала координаты обнуляем начало строки
				sscanf_s(line.c_str(), "%f %f %f",
					&Tmp_vn_Coords[coords_vn_index].x,   //координата X
					&Tmp_vn_Coords[coords_vn_index].y, //координата Y
					&Tmp_vn_Coords[coords_vn_index].z  //координата Z
				);
				coords_vn_index++;
			}
			if ((line.c_str()[0] == 'v') && (line.c_str()[1] == 't'))
			{
				//обнуляем 1 символ
				line[0] = ' ';
				//обнуляем 1 символ
				line[1] = ' ';
				//чтобы функция правельно разбирала координаты обнуляем начало строки
				sscanf_s(line.c_str(), "%f %f ",
					&Tmp_vt_Coords[coords_vt_index].x,   //координата X
					&Tmp_vt_Coords[coords_vt_index].y //координата Y
				);
				coords_vt_index++;
			}
			//проверяем не нашли ли грань
			if ((line.c_str()[0] == 'f') && (line.c_str()[1] == ' '))
			{
				//обнуляем 1 символ
				line[0] = ' ';
				//получаем параметры поверхности
				//     v1       v2     v3      
				int tmp_point[9];
				sscanf_s(line.c_str(), "%i/%i/%i %i/%i/%i %i/%i/%i",
					// номер точки  номер нормали   номер текстуры
					&tmp_point[0], &tmp_point[1], &tmp_point[2],
					&tmp_point[3], &tmp_point[4], &tmp_point[5],
					&tmp_point[6], &tmp_point[7], &tmp_point[8]);

				SimpleVertex selected;
				selected = { Tmp_v_Coords[tmp_point[0] - 1],Tmp_vt_Coords[tmp_point[1] - 1],Tmp_vn_Coords[tmp_point[2] - 1] };
				if (FAILED(GetIndex(&selected, vertices, index, cntOfPoligons, cntOfIndex)))
					return S_FALSE;
				
				selected = { Tmp_v_Coords[tmp_point[3] - 1],Tmp_vt_Coords[tmp_point[4] - 1],Tmp_vn_Coords[tmp_point[5] - 1] };
				if (FAILED(GetIndex(&selected, vertices, index, cntOfPoligons, cntOfIndex)))
					return S_FALSE;

				selected = { Tmp_v_Coords[tmp_point[6] - 1],Tmp_vt_Coords[tmp_point[7] - 1],Tmp_vn_Coords[tmp_point[8] - 1] };
				if (FAILED(GetIndex(&selected, vertices, index, cntOfPoligons, cntOfIndex)))
					return S_FALSE;
			}
		}
		index[0] = 0;
		free(Tmp_v_Coords);
		free(Tmp_vn_Coords);
		free(Tmp_vt_Coords);
		My_File_obj.close();
		return S_OK;
	}
	else return S_FALSE;
}

HRESULT GetIndex(SimpleVertex* selected, SimpleVertex* &vertices, WORD* &index, DWORD &cntOfPoligons, DWORD &cntOfIndex)
{
	bool bFoundInList = false;
	DWORD curentInd = 0;
	WORD* newIndex = NULL;
	SimpleVertex* newVertex = NULL;

	if (vertices == NULL) {
		vertices = (SimpleVertex*)malloc(sizeof(*vertices));
		index = (WORD*)malloc(sizeof(*index));

		vertices[0].Pos = selected[0].Pos;
		vertices[0].TexCoord = selected[0].TexCoord;
		vertices[0].Normal = selected[0].Normal;

		index[0] = 0;
		cntOfPoligons++;
		cntOfIndex++;
		return S_OK;
	}
	else {
		cntOfIndex++;
		for (int i = 0; i < cntOfPoligons; i++)
		{
			SimpleVertex* pCacheVertex = &vertices[i];
			if (cmp(selected, pCacheVertex))
			{
				curentInd = i;
				bFoundInList = true;
				break;
			}
		}

		if (!bFoundInList) {
			cntOfPoligons++;
			index[0] += 1;
			newIndex = (WORD*)realloc(index, sizeof(WORD)*cntOfIndex);

			index = newIndex;
			index[cntOfIndex - 1] = index[0];
			newVertex = (SimpleVertex*)realloc(vertices, sizeof(*vertices)*cntOfPoligons);

			vertices = newVertex;
			vertices[cntOfPoligons - 1].Pos = selected[0].Pos;
			vertices[cntOfPoligons - 1].TexCoord = selected[0].TexCoord;
			vertices[cntOfPoligons - 1].Normal = selected[0].Normal;
			return S_OK;
		}
		else {
			newIndex = (WORD*)realloc(index, sizeof(*index)*cntOfIndex);
			index = newIndex;
			index[cntOfIndex - 1] = curentInd;
			return S_OK;
		}
	}
	return S_FALSE;
}

bool cmp(SimpleVertex *a, SimpleVertex *b) {
	if (a->Normal.x == b->Normal.x &&
		a->Normal.y == b->Normal.y &&
		a->Normal.z == b->Normal.z &&
		a->Pos.x == b->Pos.x &&
		a->Pos.y == b->Pos.y &&
		a->Pos.z == b->Pos.z &&
		a->TexCoord.x == b->TexCoord.x &&
		a->TexCoord.y == b->TexCoord.y) {
		return true;
	}
	else return false;
}
