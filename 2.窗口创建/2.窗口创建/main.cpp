#include<Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
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
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wc = {0};
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
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(
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

	MoveWindow(hWnd,200,50,800,600,true);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = {0};
	while (GetMessage(&msg,NULL,0,0))//���ϴ���Ϣ������ȡ����Ϣ
	{
		TranslateMessage(&msg);//���������Ϣת�����ַ���Ϣ
		DispatchMessage(&msg);//�ַ�һ����Ϣ�����ڳ���
	}


	UnregisterClass("pClassName",wc.hInstance);

	return 0;
}

