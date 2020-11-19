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
HPEN g_hPen[7] = {0};//���ʾ��
HBRUSH g_hBrush[7] = {0};//��ˢ���
int g_iPenStyle[7] = {
	PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT,PS_NULL,PS_INSIDEFRAME
};//���廭����ʽ����ʼ��
int g_iBrushStyle[6] = {
	HS_VERTICAL, HS_HORIZONTAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_BDIAGONAL
};//���廭ˢ��ʽ����ʼ��

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
	wc.lpszClassName = "pClassName";//ָ�������Ƶ�ָ��
	if (!RegisterClassEx(&wc)){
		MessageBox(hWnd, "ע�ᴰ����ʧ��", "����", 0);
		return -1;
	}

	hWnd = CreateWindowEx(
		0,//dwExStyle��//���ڵ���չ���
		"pClassName",//lpClassName��ָ�������Ƶ�ָ��
		"happy world",//lpWindowName��ָ�򴰿����Ƶ�ָ��
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
		MessageBox(hWnd, "��Ϸ��Դ��ʼ��ʧ��", "����", 0);
		return false;
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))//���ϴ���Ϣ������ȡ����Ϣ
	{
		TranslateMessage(&msg);//���������Ϣת�����ַ���Ϣ
		DispatchMessage(&msg);//�ַ�һ����Ϣ�����ڳ���
	}
	UnregisterClass("pClassName", wc.hInstance);

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT paintStruct;//����һ��LPPAINTSTRUCT�ṹ��
	switch (message)
	{
	case WM_KEYDOWN://���̰�����Ϣ
		if (wParam == VK_ESCAPE){//ESC��
			if (MessageBox(hWnd, "��ȷ��Ҫ�رմ�����", "�رմ���", MB_OKCANCEL) == IDOK){
				PostQuitMessage(0);
			}
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, "��ȷ��Ҫ�رմ�����", "�رմ���", MB_OKCANCEL) == IDOK){
			PostQuitMessage(0);
		}
		break;
	case WM_PAINT://����ͻ����ػ���Ϣ
		g_hdc = BeginPaint(hWnd,&paintStruct);
		Game_Paint(hWnd);
		EndPaint(hWnd,&paintStruct);
		ValidateRect(hWnd,NULL);
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
	srand((unsigned)time(NULL));
	for (int i = 0; i < 6; i++){
		g_hPen[i] = CreatePen(g_iPenStyle[i], 5, RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i==6){
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		else{
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i], RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	}
	//---------------//
	Game_Paint(hWnd);
	ReleaseDC(hWnd, g_hdc);
	return true;
}

void Game_Paint(HWND hWnd){
	//����һ��y����
	int y = 0;
	//һ��forѭ���������ֲ�ͬ�Ļ��ʻ�������
	for (int i = 0; i <= 6;i++){
		y = (i+1)*70;

		SelectObject(g_hdc,g_hPen[i]);//����Ӧ�Ļ���ѡ��
		MoveToEx(g_hdc,30,y,NULL);//����ƶ�����Ӧ�ģ�30��y����
		LineTo(g_hdc,100,y);//�ӣ�30��y�����ƶ�����100��y��
	}
	/*ע�����滭���y=420��x���滭����ʱ������*/
	//��������x��yֵ
	int x1 = 120;
	int x2 = 190;

	//��7�в�ͬ�Ļ�ˢ������
	for (int i = 0; i < 7;i++){
		SelectObject(g_hdc,g_hBrush[i]);//ѡ�û�ˢ
		Rectangle(g_hdc,x1,70,x2,y);
		x1 += 90;
		x2 += 90;
	}
}

bool Game_CleanUp(HWND hWnd){
	//�ͷ����еĻ��ʺͻ�ˢ
	for (int i = 0; i < 7; i++){
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return true;
}