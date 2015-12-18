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

	// ������ ������

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



	// �������� ������ ������
	if (FAILED(pDirect3DDevice->CreateVertexBuffer(
		totalPoints * sizeof(CUSTOMVERTEX),	// ������ ������ ����� � ������
		0,							// ������ ���������� ������ ������
		D3DFVF_CUSTOMVERTEX,		// ������ ������	
		D3DPOOL_DEFAULT,			// ���� ����������� ������ ������ ������ ��� ���������� � ������
		&pVertexBuffer,				// ����� ���������, ��� ����� ��������� ����� ������������ ������ ������
		NULL						// ����������������� ��������
		))) {
		return E_FAIL;
	}

	// ���������� ������
	void *pVB = NULL;
	if (FAILED(pVertexBuffer->Lock(
		0,					// ��������
		totalPoints *sizeof(CUSTOMVERTEX),	// ������ ���������� ������ ������
		(void **)&pVB,		// ����� ��������� �� ���������, � ������� ������ ������
		0					// ��� �������������� ����������
		))) {
		return E_FAIL;
	}

	// ����������� ������
	memcpy(pVB, Vertexes, totalPoints * sizeof(CUSTOMVERTEX));

	// ������������� ������
	pVertexBuffer->Unlock();


	if (FAILED(pDirect3DDevice->CreateIndexBuffer(
		// ������ ���������� ������ � ������
		indexBufferSize * sizeof(unsigned short),
		// ������ ����������
		0,
		// ������
		D3DFMT_INDEX16,
		// ������ ��� ���������� � ������
		D3DPOOL_DEFAULT,
		// ����� ���������, � ������� �������� ����������� ��������� �����
		&pIndexBuffer,
		// ��������������
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

	// ������� ������ �����
	pDirect3DDevice->Clear(
		0,							// ���������� ������������� ��������
		NULL,						// ����� ������� ���������������
		D3DCLEAR_TARGET,			// ������� ����������� ��������� ������
		D3DCOLOR_XRGB(255, 250, 0),	// ����
		1.0f,						// �������� ��� ������� Z-������[0-1]
		0							// �������� ��� ������� ������ ���������
		);

	// ������ �����
	pDirect3DDevice->BeginScene();

	Matrix();

	// ����� ������ ����������� � ������� ������ ����������
	pDirect3DDevice->SetStreamSource(
		0,						// ����������� ������ ������[0-1]
		pVertexBuffer,			// ��������� �� ����� ������
		0,						// �������� �� ������ ������, �� ������ ������ ������
		sizeof(CUSTOMVERTEX)	// "��� � ������" �� ����� ������� �� ������
		);

	// �������� ������� ������
	pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	pDirect3DDevice->SetIndices(pIndexBuffer);




	for (int i = 0; i < countNurbs; i++) {
		//pDirect3DDevice->DrawPrimitive(
		//	D3DPT_LINESTRIP,		// ��� ���������. ������������������ ������������� ������
		//	segments * i,						// ������ ������ �������
		//	segments - 1			// ���������� ����������
		//	);
		pDirect3DDevice->DrawIndexedPrimitive(
			D3DPT_LINESTRIP,	// ��� ���������
			segments * i,		// �������� �������
			0,					// ����������� ������
			0,					// ���������� �������� ������
			0,					// ��������� ������
			segments - 1		// ���������� �������� ����������
			);

	}




	// ����� �����
	pDirect3DDevice->EndScene();


	// ����������� ������� ������ �� ��������
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);

	return S_OK;
}

HRESULT InitialDirect3D(HWND hvnd) {

	// ������� ��������� �� ��������� IDerect3D9
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirect3D == NULL) {
		return E_FAIL;
	}

	/*
	�������� ���������� � ������� ������ ����������� ����������� �������
	���������� ������, ��������� � ������ �����������
	*/
	D3DDISPLAYMODE Display;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display))) {
		return E_FAIL;
	}

	// ������ ���������� �������������
	D3DPRESENT_PARAMETERS Direct3DParameter;
	// ������� ����������� ��������� �� ������������� "������"
	ZeroMemory(&Direct3DParameter, sizeof Direct3DParameter);

	// ����������. TRUE - ������� �����, FALSE - �������������
	Direct3DParameter.Windowed = TRUE;
	// ������� ������ ������. 
	Direct3DParameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// ������ ����������� ������� ������
	Direct3DParameter.BackBufferFormat = Display.Format;

	// �������� ������� ��� ���������� ����������
	if (FAILED(pDirect3D->
		CreateDevice(
			D3DADAPTER_DEFAULT,						// ����������
			D3DDEVTYPE_HAL,							// ��� ����������(�� ��������� �������, ����� �� �������������� ����������� ����������)
													// D3DDEVTYPE_HALL - ������������� ����������� ����������� �����������
			hvnd,									// ���������� ����
			D3DCREATE_HARDWARE_VERTEXPROCESSING,	// ��� ����� ����������� ��������� ������
													// D3DCREATE_HARDWARE_VERTEXPROCESSING - ����������� ���������� � ���������� ���������� ��������� ������
			&Direct3DParameter,						// ��������� �� ��������� D3DPRESENT_PARAMTERS
			&pDirect3DDevice						// ����� ��������� ��� �������-���������� IDerect3DDevice9
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
������� ��������� �������
*/
LRESULT CALLBACK MainWinProc(
	HWND hwnd,								// ���������� ����
	UINT msg,								// ������������� �������
	WPARAM wparam,							// �������������� ���.
	LPARAM lparam) {						// �������������� ���.

	switch (msg) {
	case WM_PAINT:							// ������������� ����������� ����� ����, ������� ���� ����������, ��������� � ��� �����
		RenderingDirect3D();
		ValidateRect(hwnd, NULL);
		break;
	case WM_DESTROY:						// ���� ������ ���� �������(������������ ��������)
		DeleteDirect3D();
		// ����� ������������ �������� ���������� WM_QUIT - ����������� ����������
		// ������ � ������� WM_QUIT
		PostQuitMessage(0);
		return 0;
	}

	// ��������� ���������, ������� �� ����������
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


/*
������� �������
*/
int WINAPI WinMain(
	HINSTANCE hinstance,		// ����������/��������� ����������
	HINSTANCE hprevinstance,	// �� ������������, ����� 0
	LPSTR lpCmdLine,			// ��������� �� ��������� ������, ������� ���� ����� �� ������ ����������� �������
	int nCmdShow) {				// ��������, ����������� - ��� ����� ������������ ���� �� ������ �������� ��� ��������


	WNDCLASSEX windowsclass;	// ����� ������������ ����

	windowsclass.cbSize = sizeof(WNDCLASSEX);								// ������ ����������� ���������
	windowsclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;	// ���������� ������
																			/*
																			CS_VREDRAW - ���� ������ ������ ���� �������� ��� ���� ���� ����������, �� ��������� ����������� ����
																			CS_HREDRAW - ���� ������ ������ ���� �������� ��� ���� ���� ����������, �� ��������� ����������� ����
																			CS_OWNDC - ��� ������� ���� ������� ������ ������ ���� �������� ���������
																			CS_DBLCLKS - ��� ����������� � ���� ������� ������ ����� ���� ���������� ���������� � ������� ������
																			*/
	windowsclass.lpfnWndProc = MainWinProc;									// ��������� �� call back. ���������� ���� ��������� �������, ��� ������ �������� ���������� �����
	windowsclass.cbClsExtra = 0;											// ��� �������� ���. ����������
	windowsclass.cbWndExtra = 0;											// ��� �������� ���. ����������
	windowsclass.hInstance = hinstance;										// ����, ���������� �� ��������� ������������ ����������, ������������� � ������� WinMain
	windowsclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);					// ������ ��� ����������� ����������� ����������
	windowsclass.hCursor = LoadCursor(NULL, IDC_ARROW);						// ����, �������� �� ������
	windowsclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);		// ���� ��������� �������� �� ���� ���� ����
	windowsclass.lpszMenuName = NULL;										// ����, ��� ����������� ������������ ���� � ����
	windowsclass.lpszClassName = _T("WINDOWSCLASS");						// �������� ������ ����
	windowsclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);					// ����� �����������

																			// ������������ ��������� �����
	if (!RegisterClassEx(&windowsclass)) {
		return 0;
	}


	// �������� ����
	HWND hwnd = CreateWindowEx(
		NULL,								// ����� ����(WS_EX_TOPMOST - ���� ���������� ������ ������ ����)
		_T("WINDOWSCLASS"),					// ��� ������ ������������ ����
		_T("������� ���� ��� DirectX"),		// ��������� ����
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// ����� ����������� ����� � ��������� ����
											/*
											WS_OVERLAPPED - ���� � �������� ��������� � ������
											WS_VISIBLE - ���������� ������� ����
											WS_CAPTION - ���� � �������� ��������� (�������� � ���� ����� WS_BORDER)
											WS_BORDER - ���� � ������ �����
											WS_ICONIC - ���� ����������� ���������������
											WS_OVERLAPPEDWINDOW - ��������������� ����(��������: WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, WS_MAXIMIZEBOX)
											WS_MINIMIZE - ���������� ���������������
											WS_MAXIMIZE -������������ ����������������
											WS_MINIMIZEBOX - ���� � ������� �����������(������������ �� ������ WS_SYSMENU)
											WS_MAXIMIZEBOX -���� � ������� ������������(������������ �� ������ WS_SYSMENU)
											*/
		0, 0,								// ���������� ������ �������� �����
		500, 400,							// ������ � ������ ����
		NULL,								// ���������� ������������� ����
		NULL,								// ���������� ����
		hinstance,							// ���������� ����������
		NULL);								// ��������� �� ������ ����

											// ��������, ������ �� �������
	if (!hwnd) {
		return 0;
	}


	// ������������� ���������
	MSG msg;
	if (SUCCEEDED(InitialDirect3D(hwnd))) {
		if (SUCCEEDED(InitialVertexBuffer())) {
			// ������� ����
			ShowWindow(hwnd, SW_SHOWDEFAULT);
			// ��������� ����
			UpdateWindow(hwnd);

			ZeroMemory(&msg, sizeof msg);
			while (msg.message != WM_QUIT) {
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					// �������������� ������������� �����
					TranslateMessage(&msg);
					// ��������� � ��������� ��������� � MainWinProc()
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
