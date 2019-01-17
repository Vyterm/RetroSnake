#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <stdarg.h>
#include "GameLib.hpp"

/* 设置控制台窗口标题 */
void SetTitle(const char * title) {
	SetConsoleTitle(title);
}
/* 0-黑色, 1-蓝色,   2-绿色,      3-浅绿色,     4-红色,   5-紫色,   6-黄色,   7-白色,
 * 8-灰色, 9-淡蓝色, 10-淡绿色,   11-淡浅绿色   12-淡红色 13-淡紫色 14-淡黄色 15-亮白色*/
void SetColor(Color color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);                    //获取当前窗口句柄
	SetConsoleTextAttribute(handle, short(color.fore) + short(color.back) * 0x10);//设置颜色
	//CloseHandle(handle);
}
/* 根据宽高设置控制台窗口的大小 */
void SetConsoleWindowSize() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//设置控制台窗口的缓冲区大小
	COORD size = { WIN_WIDTH, WIN_HEIGHT };
	SetConsoleScreenBufferSize(handle, size);
	//设置控制台窗口大小,最大坐标是缓冲区大小-1
	SMALL_RECT rect = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };
	SetConsoleWindowInfo(handle, 1, &rect);
	//CloseHandle(handle);
}
void ResetCursor()
{
	keybd_event(VK_SHIFT, 0, 0, 0);
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	CONSOLE_CURSOR_INFO cci;                    //控制台光标信息结构类型
	cci.dwSize = 1;                             //光标大小
	cci.bVisible = FALSE;                       //是否显示光标
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cci);      //设置指定控制台屏幕缓冲区光标大小和可见性
}
void SetPosition(int x, int y) {
	COORD cursorPosition;   //光标的坐标
	HANDLE handle;          //当前窗口句柄
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	cursorPosition.X = x*2;   //设置x、y坐标
	cursorPosition.Y = y;
	SetConsoleCursorPosition(handle, cursorPosition);
	//CloseHandle(handle);
}
