#include <windows.h>
#include <tchar.h>

#include "d3d9.h"
#pragma comment(lib, "d3d9.lib")

#include "d3dx9.h"
#pragma comment(lib, "d3dx9.lib")

LPDIRECT3D9 pDirect3D = NULL;
LPDIRECT3DDEVICE9 pDirect3DDevice = NULL;
LPDIRECT3DINDEXBUFFER9 pIndexBuffer = NULL;


struct CUSTOMVERTEX {
	float x, y, z;
	DWORD color;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;

void Matrix() {
	D3DXMATRIX MatrixWorld;
	D3DXMATRIX MatrixView;
	D3DXMATRIX MatrixProjection;

	// MatrixWorld

	static float Angle = 0;
	//Angle += 0.05;

	D3DXMatrixRotationX(&MatrixWorld, Angle);

	pDirect3DDevice->SetTransform(D3DTS_WORLD, &MatrixWorld);

	// MatrixView
	D3DXMatrixLookAtLH(
		&MatrixView,
		&D3DXVECTOR3(0.f, 0.f, -18.f),
		&D3DXVECTOR3(0.f, 0.f, 0.f),
		&D3DXVECTOR3(0.f, 1.f, 0.f));
	pDirect3DDevice->SetTransform(D3DTS_VIEW, &MatrixView);

	// MatrixProjection
	D3DXMatrixPerspectiveFovLH(
		&MatrixProjection,
		D3DX_PI / 4.f, 1.f,
		1.f,
		100.f);
	pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &MatrixProjection);

}



float p[][2] = {
	{ -1.376, 3.977 },
	{ -2.955, 3.924 },
	{ -3.995, 1.713 },
	{ -3.206, -0.498 },

	{ -3.206, -0.498 },
	{ -2.253, -2.57 },
	{ 1.808, -1.801 },
	{ 2.203, 1.963 },


	{ 2.203, 1.963 },
	{ 2.598, 4.095 },
	{ 0.44, 5.109 },
	{ -0.784, 2.858 },


	{ -0.784, 2.858 },
	{ -1.324, 1.713 },
	{ -0.271, 1.24 },
	{ 0.624, 1.871 },


	{ 0.624, 1.871 },
	{ 0.929, 2.247 },
	{ 0.715, 2.263 },
	{ 0.552, 2.209 },

	{ 0.552, 2.209 },
	{ 0.304, 2.103 },
	{ -0.284, 2.664 },
	{ 0.378, 3.346 },

	{ 0.378, 3.346 },
	{ 1.188, 4.312 },
	{ 2.187, 3.272 },
	{ 1.744, 1.741 },

	{ 1.744, 1.741 },
	{ 1.58, 0.06 },
	{ -0.315, -1.934 },
	{ -1.731, -0.758 },

	{ -1.731, -0.758 },
	{ -2.788, 0.658 },
	{ -2.549, 2.852 },
	{ -1.611, 3.61 },

	{ -1.611, 3.61 },
	{ -1.303, 3.933 },
	{ -1.304, 3.979 },
	{ -1.376, 3.977 },

	// вторая кривая

	{ -0.441, -0.121 },
	{ 1.48, 1.023 },
	{ 0.546, -1.546 },
	{ 2.513, -0.564 },

	{ 2.513, -0.564 },
	{ 2.513, -0.564 },
	{ 2.537, -0.842 },
	{ 2.537, -0.842 },

	{ 2.537, -0.842 },
	{ 0.664, -2.076 },
	{ 0.647, 0.074 },
	{ -0.393, -0.221 },

	{ -0.393, -0.221 },
	{ -0.509, -0.223 },
	{ -0.534, -0.183 },
	{ -0.441, -0.121 }

};


int countNurbs = 14;
int segments = 500;
int totalPoints = countNurbs * segments;
int indexBufferSize = 0;

HRESULT InitialVertexBuffer() {

	int i, j;
	float t;
	float step = 1.f / (segments - 1);

	indexBufferSize = totalPoints;
	unsigned short * Indexes = new unsigned short[indexBufferSize];

	for (i = 0; i < indexBufferSize; i++) {
		Indexes[i] = i;
	}



	CUSTOMVERTEX* Vertexes = new CUSTOMVERTEX[totalPoints];


	for (i = 0; i < totalPoints; i++)
	{
		Vertexes[i].z = 0.5f;
		Vertexes[i].color = 0x00ff0000;
	}


	int c = 0;
	int w = 0;

	for (i = 0; i < countNurbs; i++) {

		c = i * 4;


		float a[4] = {
			-p[c][0] + 3.f*p[c + 1][0] - 3.f*p[c + 2][0] + p[c + 3][0],
			3.f*p[c][0] - 6.f*p[c + 1][0] + 3.f*p[c + 2][0],
			-3.f*p[c][0] + 3.f*p[c + 1][0],
			p[c][0]
		};

		float b[4] = {
			-p[c][1] + 3.f*p[c + 1][1] - 3.f*p[c + 2][1] + p[c + 3][1],
			3.f*p[c][1] - 6.f*p[c + 1][1] + 3.f*p[c + 2][1],
			-3.f*p[c][1] + 3.f*p[c + 1][1],
			p[c][1]
		};



		t = 0.f;
		for (j = 0; j < segments; j++) {
			t += step;
			Vertexes[w].x = ((a[0] * t + a[1])*t + a[2])*t + a[3];
			Vertexes[w].y = ((b[0] * t + b[1])*t + b[2])*t + b[3];
			w++;

		}


	}



	// создание буфера вершин
	if (FAILED(pDirect3DDevice->CreateVertexBuffer(
		totalPoints * sizeof(CUSTOMVERTEX),	// размер буфера вешир в байтах
		0,							// способ применения буфера вершин
		D3DFVF_CUSTOMVERTEX,		// формат вершин	
		D3DPOOL_DEFAULT,			// флаг описывающий формат буфера вершин для размещения в памяти
		&pVertexBuffer,				// адрес указателя, где будет храниться адрес создаваемого буфера вершин
		NULL						// зарезервированный параметр
		))) {
		return E_FAIL;
	}

	// блокировка буфера
	void *pVB = NULL;
	if (FAILED(pVertexBuffer->Lock(
		0,					// смещение
		totalPoints *sizeof(CUSTOMVERTEX),	// размер блокировки буфера вершин
		(void **)&pVB,		// адрес указателя на указатель, с данными буфера вершин
		0					// тип осуществляемой блокировки
		))) {
		return E_FAIL;
	}

	// копирование данных
	memcpy(pVB, Vertexes, totalPoints * sizeof(CUSTOMVERTEX));

	// разблокировка буфера
	pVertexBuffer->Unlock();


	if (FAILED(pDirect3DDevice->CreateIndexBuffer(
		// размер индексного буфера в байтах
		indexBufferSize * sizeof(unsigned short),
		// способ применения
		0,
		// формат
		D3DFMT_INDEX16,
		// фортма для размещения в памяти
		D3DPOOL_DEFAULT,
		// адрес указателя, в котором хранится создаваемый индексный буфер
		&pIndexBuffer,
		// зарезервинован
		NULL
		))) {
		return E_FAIL;
	}

	void *pIB = NULL;
	if (FAILED(pIndexBuffer->Lock(
		0,
		indexBufferSize * sizeof(unsigned short),
		(void**)&pIB, 0
		))) {
		return E_FAIL;
	}
	memcpy(pIB, Indexes, indexBufferSize * sizeof(unsigned short));
	pIndexBuffer->Unlock();

	return S_OK;
}





HRESULT RenderingDirect3D() {

	if (pDirect3DDevice == NULL) {
		return E_FAIL;
	}

	// очищаем задний буфер
	pDirect3DDevice->Clear(
		0,							// количество прямоугольных областей
		NULL,						// адрес массива прямоугольников
		D3DCLEAR_TARGET,			// очищаем поверхность заданыным цветом
		D3DCOLOR_XRGB(255, 250, 0),	// цвет
		1.0f,						// значение ждя очистки Z-буфера[0-1]
		0							// значение для очистки буфера трафарета
		);

	// начало сцены
	pDirect3DDevice->BeginScene();

	Matrix();

	// буфер вершин связывается с потоком данных устройства
	pDirect3DDevice->SetStreamSource(
		0,						// определение потока данных[0-1]
		pVertexBuffer,			// указатель на буфер вершин
		0,						// смещение от начала данных, до начала данных вершин
		sizeof(CUSTOMVERTEX)	// "шаг в байтах" от одной вершины до другой
		);

	// устновка формата вершин
	pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	pDirect3DDevice->SetIndices(pIndexBuffer);




	for (int i = 0; i < countNurbs; i++) {
		//pDirect3DDevice->DrawPrimitive(
		//	D3DPT_LINESTRIP,		// тип примитива. последовательность изолированных вершин
		//	segments * i,						// индекс первой вершины
		//	segments - 1			// количество примитивов
		//	);
		pDirect3DDevice->DrawIndexedPrimitive(
			D3DPT_LINESTRIP,	// тип примитива
			segments * i,		// смещение индекса
			0,					// минимальный индекс
			0,					// количество рисуемых вершин
			0,					// стартовый индекс
			segments - 1		// количество рисуемых примитивов
			);

	}




	// конец сцены
	pDirect3DDevice->EndScene();


	// копирование заднего буфера на передний
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT InitialDirect3D(HWND hvnd) {

	// создаем указатель на интерфейс IDerect3D9
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirect3D == NULL) {
		return E_FAIL;
	}

	/*
	получаем информацию о текущем режиме визуального отображения дисплея
	разрешение экрана, цветность и формат поверхности
	*/
	D3DDISPLAYMODE Display;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display))) {
		return E_FAIL;
	}

	// объект параметров представления
	D3DPRESENT_PARAMETERS Direct3DParameter;
	// ощичаем создаваемую структуру от всевозможного "мусора"
	ZeroMemory(&Direct3DParameter, sizeof Direct3DParameter);

	// видеорежим. TRUE - оконный режим, FALSE - полноэкранный
	Direct3DParameter.Windowed = TRUE;
	// задание буфера обмена. 
	Direct3DParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// формат поверхности заднего буфера
	Direct3DParameter.BackBufferFormat = Display.Format;

	// создание объекта для интерфейса устройства
	if (FAILED(pDirect3D->
		CreateDevice(
			D3DADAPTER_DEFAULT,						// видеокарта
			D3DDEVTYPE_HAL,							// тип устройства(от парамента зависит, будет ли использоваться акселерация видеокарты)
													// D3DDEVTYPE_HALL - использование возможности аппаратного обеспечения
			hvnd,									// дескриптор окна
			D3DCREATE_HARDWARE_VERTEXPROCESSING,	// как будет происходить обработка вершин
													// D3DCREATE_HARDWARE_VERTEXPROCESSING - ипользуется видеокарта и происходит аппаратная обработка вершин
			&Direct3DParameter,						// указатель на структуру D3DPRESENT_PARAMTERS
			&pDirect3DDevice						// адрес указателя для объекта-интерфейса IDerect3DDevice9
			))) {
		return E_FAIL;
	}

	//deactivation of culling Direct3D
	pDirect3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//deactivation of lighting Direct3D
	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}


void DeleteDirect3D() {

	if (pVertexBuffer != NULL) {
		pVertexBuffer->Release();
	}

	if (pDirect3DDevice != NULL) {
		pDirect3DDevice->Release();
	}

	if (pDirect3D != NULL) {
		pDirect3D->Release();
	}
}



/*
функция обработки событий
*/
LRESULT CALLBACK MainWinProc(
	HWND hwnd,								// дескриптор окна
	UINT msg,								// идентификатор события
	WPARAM wparam,							// дополнительная инф.
	LPARAM lparam) {						// дополнительная инф.

	switch (msg) {
	case WM_PAINT:							// необходимость перерисовки всего окна, которое было перемещено, увеличего и так далее
		RenderingDirect3D();
		ValidateRect(hwnd, NULL);
		break;
	case WM_DESTROY:						// окно должно быть закрыто(освобождение ресурсов)
		DeleteDirect3D();
		// после освобождения ресурсов вызывается WM_QUIT - закрывающее приложение
		// ставит в очередь WM_QUIT
		PostQuitMessage(0);
		return 0;
	}

	// обработка сообщений, которые не используем
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


/*
главная функция
*/
int WINAPI WinMain(
	HINSTANCE hinstance,		// дескриптор/индикатор приложения
	HINSTANCE hprevinstance,	// не используется, равен 0
	LPSTR lpCmdLine,			// указатель на командную строку, которая идет сразу за именем запускаемой команды
	int nCmdShow) {				// парамент, указывающий - как будет отображаться окно на экране монитора при создании


	WNDCLASSEX windowsclass;	// класс создаваемого окна

	windowsclass.cbSize = sizeof(WNDCLASSEX);								// размер создаваемой структуры
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;	// комбинация флагов
																			/*
																			CS_VREDRAW - если размер высоты окна меняется или окно было перемещено, то требуется перерисовка окна
																			CS_HREDRAW - если размер ширины окна меняется или окно было перемещено, то требуется перерисовка окна
																			CS_OWNDC - для каждого окна данного класса дается свой контекст устройств
																			CS_DBLCLKS - при совершенном в окне двойном щелчке мышью окну посылается информация о двойном щелчке
																			*/
	windowsclass.lpfnWndProc = MainWinProc;									// указатель на call back. Существует цикл обработки событий, при работе которого происходит вызов
	windowsclass.cbClsExtra = 0;											// для кранения доп. информации
	windowsclass.cbWndExtra = 0;											// для кранения доп. информации
	windowsclass.hInstance = hinstance;										// поле, отвечающее за экземпляр создаваемого приложения, передаваемого в функцию WinMain
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// служит для определения пиктограммы приложения
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);						// поле, отвечает за курсор
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		// поле структуры отвечает за цвет фона окна
	windowsclass.lpszMenuName = NULL;										// поле, для подключения стандартного меню к окну
	windowsclass.lpszClassName = _T("WINDOWSCLASS");						// название класса окна
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// малая пиктограмма

																			// регистрируем созданный класс
	if (!RegisterClassEx(&windowsclass)) {
		return 0;
	}


	// создание окна
	HWND hwnd = CreateWindowEx(
		NULL,								// стиль окна(WS_EX_TOPMOST - окно появляется поверх других окон)
		_T("WINDOWSCLASS"),					// имя класса создаваемого окна
		_T("Базовое Окно для DirectX"),		// заголовок окна
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// флаги описывающие стиль и поведение окна
											/*
											WS_OVERLAPPED - окно с полоской заголовка и рамкой
											WS_VISIBLE - изначально видимое окно
											WS_CAPTION - окно с полоской заголовка (включает в себя стиль WS_BORDER)
											WS_BORDER - окно в тонкой рамке
											WS_ICONIC - окно изаначально минимизированно
											WS_OVERLAPPEDWINDOW - перекрывающиеся окно(включает: WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, WS_MAXIMIZEBOX)
											WS_MINIMIZE - изканально минимизированно
											WS_MAXIMIZE -изканачально максимизированно
											WS_MINIMIZEBOX - окно с кнопкой минимизации(используется со стилем WS_SYSMENU)
											WS_MAXIMIZEBOX -окно с кнопкой максимизации(используется со стилем WS_SYSMENU)
											*/
		0, 0,								// координаты левого верхнего угола
		500, 400,							// ширина и высота окна
		NULL,								// дескриптор родительского окна
		NULL,								// дескриптор меню
		hinstance,							// дескриптор приложения
		NULL);								// указатель на данное окно

											// проверка, смогли ли создать
	if (!hwnd) {
		return 0;
	}


	// идентификатор сообщения
	MSG msg;
	if (SUCCEEDED(InitialDirect3D(hwnd))) {
		if (SUCCEEDED(InitialVertexBuffer())) {
			// выводим окно
			ShowWindow(hwnd, SW_SHOWDEFAULT);
			// обновляем окно
			UpdateWindow(hwnd);

			ZeroMemory(&msg, sizeof msg);
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					// преобразование клавиатурного ввода
					TranslateMessage(&msg);
					// обработка и пересылка сообщений в MainWinProc()
					DispatchMessage(&msg);
				}
				else {
					RenderingDirect3D();
				}
			}
		}
	}

	return msg.wParam;
}
