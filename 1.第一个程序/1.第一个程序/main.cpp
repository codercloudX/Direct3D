#include<Windows.h>
#pragma comment(lib,"winmm.lib")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,LPSTR lpCmdLine,int nCmdShow){
	//1.
	/*int res = MessageBox(NULL, "���", "����", MB_ABORTRETRYIGNORE | MB_YESNOCANCEL);
	if (res==IDCANCEL){
		MessageBox(NULL, "�㰴�����Ǽ�", "����", MB_ICONWARNING | MB_YESNOCANCEL);
	}
	else if (res == IDYES){
		MessageBox(NULL, "�㰴����ȷ�ϼ�", "����", MB_ICONWARNING | MB_YESNOCANCEL);
	}
	else if (res == IDNO){
		MessageBox(NULL, "�㰴���˷��", "����", MB_ICONWARNING | MB_YESNOCANCEL);
	}*/
	
	//2.
	PlaySound("first.wav", NULL, SND_FILENAME | SND_ASYNC);
	MessageBox(NULL, "first blood", "first blood", 0);
	return 0;
}