#include<Windows.h>
#include<time.h>
#include<string>


//-------------���궨�岿�֡�-------------------//
//����������һЩ������
//---------------------------------------------------//
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE L"��ʱ��������ʾ" //���ڱ��ⶨ��ĺ�

//-------------��ȫ�ֱ����������֡�-------------------//
//������ȫ�ֱ�������
//---------------------------------------------------//
HDC g_hdc = NULL;//ȫ�ֻ����豸���

HDC g_mdc = NULL;
HBITMAP g_hBackGround[3] = {0};
int g_iNum = NULL;//��¼ͼƬͼ��


//-------------��ȫ�ֺ����������֡�-------------------//
//������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//---------------------------------------------------//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool Game_Init(HWND hWnd);//��Դ��ʼ��
void Game_Paint(HWND hWnd);//��ͼ����
bool Game_CleanUp(HWND hWnd);//��Դ����



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmdLine, int nCmdShow){
	HWND hWnd;
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);//���ýṹ���С
	wc.style = CS_VREDRAW | CS_HREDRAW;//���ô��ڵ���ʽ
	wc.lpfnWndProc = WndProc;//����ָ�򴰿ں�����ָ��
	wc.cbClsExtra = 0;//���ô����฽���ڴ�
	wc.cbWndExtra = 0;//���ô��ڸ����ڴ�
	wc.hInstance = hInstance;//ָ���������ڹ��̵ĳ����ʵ�����
	wc.hIcon = nullptr;//ͼ��ľ������
	wc.hCursor = nullptr;//���ľ������
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);//������ˢ�ľ������
	wc.lpszMenuName = nullptr;//ָ��˵���ָ�룬��
	wc.lpszClassName = L"pClassName";//ָ�������Ƶ�ָ��
	if (!RegisterClassEx(&wc)){
		MessageBox(hWnd, L"ע�ᴰ����ʧ��", L"����", 0);
		return -1;
	}

	hWnd = CreateWindowEx(
		0,//dwExStyle��//���ڵ���չ���
		L"pClassName",//lpClassName��ָ�������Ƶ�ָ��
		WINDOW_TITLE,//lpWindowName��ָ�򴰿����Ƶ�ָ��
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//dwStyle�����ڷ��
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,//�ֱ�Ϊ�����ڵ�ˮƽλ�ã����ڵĴ�ֱλ�ã����ڵĿ�ȣ����ڵĸ߶�
		NULL,//hWndParent,�����ڵľ��
		NULL,//hMenu,�˵��ľ�������Ӵ��ڵı�ʶ��
		hInstance,//Ӧ�ó���ʵ���ľ��
		NULL//ָ�򴰿ڵĴ�������
		);

	MoveWindow(hWnd, 200, 50, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//hdc����������
	//1.��Ϸ��Դ��ʼ��
	if (!Game_Init(hWnd)){
		MessageBox(hWnd, L"��Ϸ��Դ��ʼ��ʧ��", L"����", 0);
		return false;
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))//���ϴ���Ϣ������ȡ����Ϣ
	{
		TranslateMessage(&msg);//���������Ϣת�����ַ���Ϣ
		DispatchMessage(&msg);//�ַ�һ����Ϣ�����ڳ���
	}
	UnregisterClass(L"pClassName", wc.hInstance);

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT paintStruct;//����һ��LPPAINTSTRUCT�ṹ��
	switch (message)
	{
	case WM_KEYDOWN://���̰�����Ϣ
		if (wParam == VK_ESCAPE){//ESC��
			if (MessageBox(hWnd, L"��ȷ��Ҫ�رմ�����", L"�رմ���", MB_OKCANCEL) == IDOK){
				PostQuitMessage(0);
			}
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd,L"��ȷ��Ҫ�رմ�����", L"�رմ���", MB_OKCANCEL) == IDOK){
			PostQuitMessage(0);
		}
		break;
	case WM_TIMER://����ͻ����ػ���Ϣ
		//g_hdc = BeginPaint(hWnd, &paintStruct);
		Game_Paint(hWnd);
		//EndPaint(hWnd, &paintStruct);
		//ValidateRect(hWnd, NULL);
		break;
	case WM_DESTROY:
		Game_CleanUp(hWnd);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Game_Init(HWND hWnd){
	g_hdc = GetDC(hWnd);
	
	g_hBackGround[0] = (HBITMAP)LoadImage(NULL,L"B1.bmp",IMAGE_BITMAP,WINDOW_WIDTH,WINDOW_HEIGHT,LR_LOADFROMFILE);
	g_hBackGround[1] = (HBITMAP)LoadImage(NULL, L"B2.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hBackGround[2] = (HBITMAP)LoadImage(NULL, L"B3.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	
	//---------------//
	g_mdc = CreateCompatibleDC(g_hdc);
	g_iNum = 0;
	SetTimer(hWnd,1,900,NULL);//ÿ��һ�뷢����Ϣ

	return true;
}

void Game_Paint(HWND hWnd){
	//����ͼ��
	if (g_iNum==3){
		g_iNum = 0;
	}

	SelectObject(g_mdc, g_hBackGround[g_iNum]);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	//ͼ������
	g_iNum++;
}

bool Game_CleanUp(HWND hWnd){
	KillTimer(hWnd,1);//ɾ����ʱ��
	for (int i = 0; i < 3;i++){
		DeleteObject(g_hBackGround[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hWnd,g_hdc);
	return true;
}