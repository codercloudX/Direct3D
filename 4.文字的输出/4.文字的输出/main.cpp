#include<Windows.h>
#include<time.h>
#include<string>


//-------------���궨�岿�֡�-------------------//
//����������һЩ������
//---------------------------------------------------//
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "GDI������Ŀ��"//���ڱ��ⶨ��ĺ�

//-------------��ȫ�ֱ����������֡�-------------------//
//������ȫ�ֱ�������
//---------------------------------------------------//
HDC g_hdc = NULL;//ȫ�ֻ����豸���
HWND hWnd = NULL;
HFONT hFont = NULL;

//-------------��ȫ�ֺ����������֡�-------------------//
//������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
//---------------------------------------------------//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool Game_Init(HWND hWnd);//��Դ��ʼ��
void Game_Paint(HWND hWnd);//��ͼ����
bool Game_CleanUp(HWND hWnd);//��Դ����



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmdLine, int nCmdShow){
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
		L"happy world",//lpWindowName��ָ�򴰿����Ƶ�ָ��
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//dwStyle�����ڷ��
		200, 200, 640, 800,//�ֱ�Ϊ�����ڵ�ˮƽλ�ã����ڵĴ�ֱλ�ã����ڵĿ�ȣ����ڵĸ߶�
		NULL,//hWndParent,�����ڵľ��
		NULL,//hMenu,�˵��ľ�������Ӵ��ڵı�ʶ��
		hInstance,//Ӧ�ó���ʵ���ľ��
		NULL//ָ�򴰿ڵĴ�������
		);

	MoveWindow(hWnd, 200, 50, 800, 600, true);
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
		if (MessageBox(hWnd, L"��ȷ��Ҫ�رմ�����", L"�رմ���", MB_OKCANCEL) == IDOK){
			PostQuitMessage(0);
		}
		break;
	case WM_PAINT://����ͻ����ػ���Ϣ
		g_hdc = BeginPaint(hWnd, &paintStruct);
		Game_Paint(hWnd);
		EndPaint(hWnd, &paintStruct);
		ValidateRect(hWnd, NULL);
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
	//��ʼ�����ʺͻ�ˢ����ʽ��ֵ//
	/*srand((unsigned)time(NULL));
	for (int i = 0; i < 6; i++){
		g_hPen[i] = CreatePen(g_iPenStyle[i], 5, RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i == 6){
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		else{
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i], RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	}*/
	//---------------//
	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);
	return true;
}

void Game_Paint(HWND hWnd){

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, L"΢���ź�");//��������
	SelectObject(g_hdc,hFont);//������ѡ���豸����
	SetBkMode(g_hdc,TRANSPARENT);//�����������Ϊ����͸��

	wchar_t t1[] =L"all" ;
	wchar_t t2[] = L"��";
	wchar_t t3[] = L"������";

	//�����������ֵ���ɫ,�����
	SetTextColor(g_hdc,RGB(50,255,50));
	TextOut(g_hdc,30,150,t1,wcslen(t1));

	SetTextColor(g_hdc, RGB(50, 50, 255));
	TextOut(g_hdc, 30, 200,t2, wcslen(t2));

	SetTextColor(g_hdc, RGB(255, 150, 50));
	TextOut(g_hdc, 500, 250, t3, wcslen(t3));

}

bool Game_CleanUp(HWND hWnd){
	DeleteObject(hFont);
	return true;
}