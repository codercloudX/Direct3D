#include<Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message)
	{
	case WM_KEYDOWN://键盘按下消息
		if (wParam == VK_ESCAPE){//ESC键
			if (MessageBox(hWnd, "您确定要关闭窗口吗？", "关闭窗口", MB_OKCANCEL) == IDOK){
				PostQuitMessage(0);
			}
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, "您确定要关闭窗口吗？", "关闭窗口", MB_OKCANCEL) == IDOK){
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
	wc.cbSize = sizeof(WNDCLASSEX);//设置结构体大小
	wc.style = CS_VREDRAW | CS_HREDRAW;//设置窗口的样式
	wc.lpfnWndProc = WndProc;//设置指向窗口函数的指针
	wc.cbClsExtra = 0;//设置窗口类附加内存
	wc.cbWndExtra = 0;//设置窗口附加内存
	wc.hInstance = hInstance;//指定包含窗口过程的程序的实例句柄
	wc.hIcon = nullptr;//图标的句柄，空
	wc.hCursor = nullptr;//光标的句柄，空
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);//背景画刷的句柄，空
	wc.lpszMenuName = nullptr;//指向菜单的指针，空
	wc.lpszClassName = "pClassName";//指向类名称的指针
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(
		0,//dwExStyle，//窗口的扩展风格
		"pClassName",//lpClassName，指向类名称的指针
		"happy world",//lpWindowName，指向窗口名称的指针
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//dwStyle，窗口风格
		200, 200, 640, 800,//分别为：窗口的水平位置，窗口的垂直位置，窗口的宽度，窗口的高度
		NULL,//hWndParent,父窗口的句柄
		NULL,//hMenu,菜单的句柄或是子窗口的标识符
		hInstance,//应用程序实例的句柄
		NULL//指向窗口的创建数据
		);

	MoveWindow(hWnd,200,50,800,600,true);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = {0};
	while (GetMessage(&msg,NULL,0,0))//不断从消息队列中取出消息
	{
		TranslateMessage(&msg);//将虚拟键消息转化成字符消息
		DispatchMessage(&msg);//分发一个消息给窗口程序
	}


	UnregisterClass("pClassName",wc.hInstance);

	return 0;
}

