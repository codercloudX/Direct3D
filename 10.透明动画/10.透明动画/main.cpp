#include<Windows.h>
#include<time.h>
#include<string>


//-------------【宏定义部分】-------------------//
//描述：定义一些辅助宏
//---------------------------------------------------//
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE L"游戏循环动画表示" //窗口标题定义的宏

//-------------【全局变量声明部分】-------------------//
//描述：全局变量声明
//---------------------------------------------------//
HDC g_hdc = NULL;//全局环境设备句柄
HDC g_mdc = NULL;//创建一个和设备环境相兼容的内存设备环境
HBITMAP g_hBackGround[3] = {0};//三个背景图的句柄
DWORD g_tPre = 0, g_tNow = 0;//声明两个变量记录时间，上一次绘画的时间，这次准备绘画的时间
int g_iNum = NULL;//记录图片图号


//-------------【全局函数声明部分】-------------------//
//描述：全局函数声明，防止“未声明的标识”系列错误
//---------------------------------------------------//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool Game_Init(HWND hWnd);//资源初始化
void Game_Paint(HWND hWnd);//绘图代码
bool Game_CleanUp(HWND hWnd);//资源清理



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmdLine, int nCmdShow){
	HWND hWnd;
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
		WINDOW_TITLE,//lpWindowName，指向窗口名称的指针
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,//dwStyle，窗口风格
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,//分别为：窗口的水平位置，窗口的垂直位置，窗口的宽度，窗口的高度
		NULL,//hWndParent,父窗口的句柄
		NULL,//hMenu,菜单的句柄或是子窗口的标识符
		hInstance,//应用程序实例的句柄
		NULL//指向窗口的创建数据
		);

	MoveWindow(hWnd, 200, 50, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//hdc创建三部曲
	//1.游戏资源初始化
	if (!Game_Init(hWnd)){
		MessageBox(hWnd, L"游戏资源初始化失败", L"错误", 0);
		return false;
	}

	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)//如果消息不是WM_QUIT退出
	{
		if (PeekMessage(&msg, 0, 0, 0,PM_REMOVE)){//查看应用程序消息队列，有消息时将消息派发出去
			TranslateMessage(&msg);//将虚拟键消息转化成字符消息
			DispatchMessage(&msg);//分发一个消息给窗口程序
		}else{
			g_tNow = GetTickCount();//获取系统当前时间
			if (g_tNow-g_tPre>=100){//此次循环运行与上次绘图相差0.1秒时再进行重绘
				Game_Paint(hWnd);
			}
		}
		
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
		if (MessageBox(hWnd,L"您确定要关闭窗口吗？", L"关闭窗口", MB_OKCANCEL) == IDOK){
			PostQuitMessage(0);
		}
		break;
	case WM_TIMER://如果客户区重绘信息
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
	//SetTimer(hWnd,1,900,NULL);//每隔一秒发出消息

	return true;
}

void Game_Paint(HWND hWnd){
	//处理图号
	if (g_iNum==3){
		g_iNum = 0;
	}

	SelectObject(g_mdc, g_hBackGround[g_iNum]);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	//记录此次绘图时间，供下次游戏循环中判断是否已达到画面更新操作设定的时间间隔
	g_tPre = GetTickCount();
	//图号自增
	g_iNum++;
}

bool Game_CleanUp(HWND hWnd){
	KillTimer(hWnd,1);//删除定时器
	for (int i = 0; i < 3;i++){
		DeleteObject(g_hBackGround[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hWnd,g_hdc);
	return true;
}