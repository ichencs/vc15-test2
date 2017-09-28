#include "stdafx.h"
#include "windows.h"
#include "vlpp/Vlpp.h"
using namespace vl;
using namespace vl::unittest;
using namespace vl::collections;

// HHOOK myhook;   // 保存当前钩子句柄
// 
// /****************************************************************
// WH_KEYBOARD hook procedure
// 鍵盤钩子处理过程
// ****************************************************************/
// LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
// {
// 	wchar_t text[50], data[20];    // 输出字符串
// 	const wchar_t *info = NULL;    // 类型字符指针
// 	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam; // 获取按键消息
// 	HDC hdc;    // 画图设备句柄
// 
// 				// 判断是否收到键盘消息
// 	if (nCode >= 0)
// 	{
// 		// 判断消息类型
// 		if (wParam == WM_KEYDOWN)      info = "普通按鍵抬起";
// 		else if (wParam == WM_KEYUP)        info = "普通按鍵按下";
// 		else if (wParam == WM_SYSKEYDOWN)   info = "系統按鍵抬起";
// 		else if (wParam == WM_SYSKEYUP)     info = "系統按鍵按下";
// 
// 		// 初始化数组
// 		ZeroMemory(text, sizeof(text));
// 		ZeroMemory(data, sizeof(data));
// 		// 拼装字符串
// 		
// 		wsprintf(text, L"%s - 键盘码 [%04d], 扫描码 [%04d]  ", info, p->vkCode, p->scanCode);
// 		wsprintf(data, L"按鍵目测为： %c  ", p->vkCode);
// 
// 		// 此处调用 GDI 画图函数来将截取到的内容画在窗口上
// 		hdc = GetDC(画图的窗口句柄);       // 获取要画图的设备句柄
// 		TextOut(hdc, 10, 10, text, strlen(text));   // 在窗口上画文字 
// 		TextOut(hdc, 10, 30, data, strlen(data));   // 参数分别是 目标设备, x坐标, y坐标, 字符串内容, 字符串长度
// 		ReleaseDC(画图的窗口句柄, hdc);        // 释放设备句柄
// 	}
// 
// 	// 将消息继续往下传递
// 	return CallNextHookEx(myhook, nCode, wParam, lParam);
// }
// 
// TEST_CASE(TestHook)
// {
// 	HINSTANCE hInstance = GetModuleHandle(NULL);
// 	myhook = SetWindowsHookEx(
// 		WH_KEYBOARD_LL, // 监听类型【键盘消息】
// 		KeyboardProc,   // 处理函数
// 		hInstance,      // 当前实例句柄
// 		0               // 监听线程ID(NULL为全局监听)
// 	);
// 
// 	// 判断是否成功
// 	if (myhook == NULL)
// 	{
// 		wsprintf(text, L"键盘监听失败！error : %d n", GetLastError());
// 		MessageBox(hwnd, text, TEXT("错误"), MB_OK);
// 	}
// 
// }

HWND hgWnd;
HHOOK myhook;

/****************************************************************
WH_KEYBOARD hook procedure
鍵盤钩子处理过程
****************************************************************/
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	const wchar_t *info = NULL;
	wchar_t text[50], data[20];

	PAINTSTRUCT ps;
	HDC hdc;

	if (nCode >= 0)
	{
 		if (wParam == WM_KEYDOWN)      info = L"普通按鍵抬起";
		else if (wParam == WM_KEYUP)        info = L"普通按鍵按下";
		else if (wParam == WM_SYSKEYDOWN)   info = L"系統按鍵抬起";
		else if (wParam == WM_SYSKEYUP)     info = L"系統按鍵按下";

		ZeroMemory(text, sizeof(text));
		ZeroMemory(data, sizeof(data));
		wsprintf(text, L"%s - 键盘码 [%04d], 扫描码 [%04d]  ", info, p->vkCode, p->scanCode);
		wsprintf(data, L"按鍵目测为： %c  ", p->vkCode);

		hdc = GetDC(hgWnd);
		TextOut(hdc, 10, 10, text, wcslen(text));
		TextOut(hdc, 10, 30, data, wcslen(data));
		ReleaseDC(hgWnd, hdc);
	}

	return CallNextHookEx(myhook, nCode, wParam, lParam);
}

// 5. 窗口过程处理
LRESULT CALLBACK WndHookProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hgWnd = hwnd;

	switch (msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

TEST_CASE(TestHookMouse)
{
	return;

	WNDCLASSEX wc;  // http://baike.baidu.com/view/1750396.htm
	HWND hwnd;
	MSG Msg;
	
	HINSTANCE hInstance = GetModuleHandle(NULL);
	wchar_t text[30];

	const wchar_t szClassName[] = L"myWindowClass";

	// 1. 设置注册窗口结构体
	wc.cbSize = sizeof(WNDCLASSEX);              // 注册窗口结构体的大小
	wc.style = 0;                               // 窗口的样式
	wc.lpfnWndProc = WndHookProc;                         // 指向窗口处理过程的函数指针
	wc.cbClsExtra = 0;                               // 指定紧跟在窗口类结构后的附加字节数
	wc.cbWndExtra = 0;                               // 指定紧跟在窗口事例后的附加字节数
	wc.hInstance = hInstance;                       // 本模块的实例句柄
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 图标的句柄
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);     // 光标的句柄
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);        // 背景画刷的句柄
	wc.lpszMenuName = NULL;                            // 指向菜单的指针
	wc.lpszClassName = szClassName;                     // 指向类名称的指针
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // 和窗口类关联的小图标

												  // 2. 使用【窗口结构体】注册窗口
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("窗口注册失败！"), TEXT("错误"), MB_ICONEXCLAMATION | MB_OK);
		return ;
	}

	// 3. 创建窗口
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,       // 窗口的扩展风格
		szClassName,            // 指向注册类名的指针
		TEXT("窗口标题"),       // 指向窗口名称的指针
		WS_OVERLAPPEDWINDOW,    // 窗口风格
		CW_USEDEFAULT, CW_USEDEFAULT, 350, 200, // 窗口的 x,y 坐标以及宽高
		NULL,                   // 父窗口的句柄
		NULL,                   // 菜单的句柄
		hInstance,              // 应用程序实例的句柄
		NULL                    // 指向窗口的创建数据
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("窗口创建失败"), TEXT("错误"), MB_ICONEXCLAMATION | MB_OK);
		return ;
	}

	// 4. 显示窗口
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// 设置键盘全局监听
	myhook = SetWindowsHookEx(
		WH_KEYBOARD_LL, // 监听类型【键盘】
		KeyboardProc,   // 处理函数
		hInstance,      // 当前实例句柄
		0               // 监听窗口句柄(NULL为全局监听)
	);

	if (myhook == NULL)
	{
		wsprintf(text, L"键盘监听失败！error : %d n", GetLastError());
		MessageBox(hwnd, text, TEXT("错误"), MB_OK);
	}


	// 5. 消息循环
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	 Msg.wParam;
	 return;
}
