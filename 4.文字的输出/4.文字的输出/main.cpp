#include<Windows.h>
#include<time.h>
#include<string>


//-------------【宏定义部分】-------------------//
//描述：定义一些辅助宏
//---------------------------------------------------//
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "GDI程序核心框架"//窗口标题定义的宏

//-------------【全局变量声明部分】-------------------//
//描述：全局变量声明
//---------------------------------------------------//
HDC g_hdc = NULL;//全局环境设备句柄
HWND hWnd = NULL;
HFONT hFont = NULL;

//-------------【全局函数声明部分】-------------------//
//描述：全局函数声明，防止“未声明的标识”系列错误
//---------------------------------------------------//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool Game_Init(HWND hWnd);//资源初始化
void Game_Paint(HWND hWnd);//绘图代码
bool Game_CleanUp(HWND hWnd);//资源清理



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wc = { 0 };
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
	wc.lpszClassName = L"pClassName";//指向类名称的指针
	if (!RegisterClassEx(&wc)){
		MessageBox(hWnd, L"注册窗口类失败", L"错误", 0);
		return -1;
	}

	hWnd = CreateWindowEx(
		0,//dwExStyle，//窗口的扩展风格
		L"pClassName",//lpClassName，指向类名称的指针
		L"happy world",//lpWindowName，指向窗口名称的指针
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//dwStyle，窗口风格
		200, 200, 640, 800,//分别为：窗口的水平位置，窗口的垂直位置，窗口的宽度，窗口的高度
		NULL,//hWndParent,父窗口的句柄
		NULL,//hMenu,菜单的句柄或是子窗口的标识符
		hInstance,//应用程序实例的句柄
		NULL//指向窗口的创建数据
		);

	MoveWindow(hWnd, 200, 50, 800, 600, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//hdc创建三部曲
	//1.游戏资源初始化
	if (!Game_Init(hWnd)){
		MessageBox(hWnd, L"游戏资源初始化失败", L"错误", 0);
		return false;
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))//不断从消息队列中取出消息
	{
		TranslateMessage(&msg);//将虚拟键消息转化成字符消息
		DispatchMessage(&msg);//分发一个消息给窗口程序
	}
	UnregisterClass(L"pClassName", wc.hInstance);

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT paintStruct;//定义一个LPPAINTSTRUCT结构体
	switch (message)
	{
	case WM_KEYDOWN://键盘按下消息
		if (wParam == VK_ESCAPE){//ESC键
			if (MessageBox(hWnd, L"您确定要关闭窗口吗？", L"关闭窗口", MB_OKCANCEL) == IDOK){
				PostQuitMessage(0);
			}
		}
		break;
	case WM_CLOSE:
		if (MessageBox(hWnd, L"您确定要关闭窗口吗？", L"关闭窗口", MB_OKCANCEL) == IDOK){
			PostQuitMessage(0);
		}
		break;
	case WM_PAINT://如果客户区重绘信息
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
	//开始给画笔和画刷的样式赋值//
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

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, L"微软雅黑");//创建字体
	SelectObject(g_hdc,hFont);//将字体选入设备环境
	SetBkMode(g_hdc,TRANSPARENT);//设置输出文字为背景透明

	wchar_t t1[] =L"all" ;
	wchar_t t2[] = L"是";
	wchar_t t3[] = L"？？？";

	//设置三段文字的颜色,并输出
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