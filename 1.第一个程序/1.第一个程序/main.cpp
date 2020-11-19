#include<Windows.h>
#pragma comment(lib,"winmm.lib")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,LPSTR lpCmdLine,int nCmdShow){
	//1.
	/*int res = MessageBox(NULL, "你好", "标题", MB_ABORTRETRYIGNORE | MB_YESNOCANCEL);
	if (res==IDCANCEL){
		MessageBox(NULL, "你按下了是键", "标题", MB_ICONWARNING | MB_YESNOCANCEL);
	}
	else if (res == IDYES){
		MessageBox(NULL, "你按下了确认键", "标题", MB_ICONWARNING | MB_YESNOCANCEL);
	}
	else if (res == IDNO){
		MessageBox(NULL, "你按下了否键", "标题", MB_ICONWARNING | MB_YESNOCANCEL);
	}*/
	
	//2.
	PlaySound("first.wav", NULL, SND_FILENAME | SND_ASYNC);
	MessageBox(NULL, "first blood", "first blood", 0);
	return 0;
}