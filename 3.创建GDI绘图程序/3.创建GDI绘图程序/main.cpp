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
HPEN g_hPen[7] = {0};//画笔句柄
HBRUSH g_hBrush[7] = {0};//笔刷句柄
int g_iPenStyle[7] = {
	PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT,PS_NULL,PS_INSIDEFRAME
};//定义画笔样式并初始化
int g_iBrushStyle[6] = {
	HS_VERTICAL, HS_HORIZONTAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_BDIAGONAL
};//定义画刷样式并初始化

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
	wc.lpszClassName = "pClassName";//指向类名称的指针
	if (!RegisterClassEx(&wc)){
		MessageBox(hWnd, "注册窗口类失败", "错误", 0);
		return -1;
	}

	hWnd = CreateWindowEx(
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

	MoveWindow(hWnd, 200, 50, 800, 600, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//hdc创建三部曲
	//1.游戏资源初始化
	if (!Game_Init(hWnd)){
		MessageBox(hWnd, "游戏资源初始化失败", "错误", 0);
		return false;
	}

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))//不断从消息队列中取出消息
	{
		TranslateMessage(&msg);//将虚拟键消息转化成字符消息
		DispatchMessage(&msg);//分发一个消息给窗口程序
	}
	UnregisterClass("pClassName", wc.hInstance);

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	PAINTSTRUCT paintStruct;//定义一个LPPAINTSTRUCT结构体
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
	case WM_PAINT://如果客户区重绘信息
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
	//开始给画笔和画刷的样式赋值//
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
	//定义一个y坐标
	int y = 0;
	//一个for循环，用七种不同的画笔绘制线条
	for (int i = 0; i <= 6;i++){
		y = (i+1)*70;

		SelectObject(g_hdc,g_hPen[i]);//将对应的画笔选好
		MoveToEx(g_hdc,30,y,NULL);//光标移动到对应的（30，y）处
		LineTo(g_hdc,100,y);//从（30，y），移动到（100，y）
	}
	/*注意上面画完后y=420，x下面画矩形时还有用*/
	//定义两个x，y值
	int x1 = 120;
	int x2 = 190;

	//用7中不同的画刷填充矩形
	for (int i = 0; i < 7;i++){
		SelectObject(g_hdc,g_hBrush[i]);//选用画刷
		Rectangle(g_hdc,x1,70,x2,y);
		x1 += 90;
		x2 += 90;
	}
}

bool Game_CleanUp(HWND hWnd){
	//释放所有的画笔和画刷
	for (int i = 0; i < 7; i++){
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return true;
}