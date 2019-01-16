/***************************************************
 * 文件名：GameLib.c
 * 描  述：GameLib.h的实现文件
 * 创建人：老九学堂·窖头
 * 日  期：2016-11-10
 * 版  本：1.0
 * 官  网：http://www.xuetang9.com
 * 官方QQ：614940318（老九君）
 *
 **************************************************/
#include <stdio.h>
#include <string.h>
#include <Windows.h>
 //#include <unistd.h>// include usleep(int macro_seconds);
#include <stdarg.h>
#include "GameLib.hpp"

/* 设置控制台窗口标题 */
void SetTitle(const char * title) {
	SetConsoleTitle(title);
}
/* 0-黑色, 1-蓝色,   2-绿色,      3-浅绿色,     4-红色,   5-紫色,   6-黄色,   7-白色,
 * 8-灰色, 9-淡蓝色, 10-淡绿色,   11-淡浅绿色   12-淡红色 13-淡紫色 14-淡黄色 15-亮白色*/
void SetColor(int ForeColor, int BackGroundColor) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);                    //获取当前窗口句柄
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//设置颜色
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
	cursorPosition.X = x;   //设置x、y坐标
	cursorPosition.Y = y;
	SetConsoleCursorPosition(handle, cursorPosition);
	//CloseHandle(handle);
}
void PrintString(const char * msgString) {
	int length = strlen(msgString);
	int i = 0;
	for (; i < length; i++)
	{
		char tempChar = msgString[i];
		printf("%c", tempChar);
		Sleep(1000 * 20 / 1000000);//20毫秒
		fflush(stdout);
	}
}
void Read(char * Buffer, int BufferSize) {
	DWORD Read;
	HANDLE handle;          //当前窗口句柄
	handle = GetStdHandle(STD_INPUT_HANDLE);
	ReadConsole(handle, Buffer, BufferSize, &Read, NULL);
	//CloseHandle(handle);
}
int GetKey()
{
	HANDLE handle;          //当前窗口句柄
	handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Read;
	INPUT_RECORD Event;
	//得到控制台的输入
	ReadConsoleInput(handle, &Event, 1, &Read);
	//如果输入事件是一个键盘事件，就进行判断，返回按下键的虚拟键码
	if (Event.EventType == KEY_EVENT)
	{
		if (Event.Event.KeyEvent.bKeyDown)//返回值为true，表示有键被按下
		{
			return Event.Event.KeyEvent.wVirtualKeyCode;//返回按下键的虚拟键码
		}
	}
	return 0;
}
/*
 * 从起始坐标开始，清rowCount行,每行清除GAME_WIDTH(78)个字符
 */
void ClearByCoord(int x, int y, int rowCount)
{
	COORD coordStart = { short(x), short(y) };
	int i = 0;
	DWORD written;
	HANDLE handle;                              //当前窗口句柄
	CONSOLE_SCREEN_BUFFER_INFO bInfo;           //窗口信息结构
	handle = GetStdHandle(STD_OUTPUT_HANDLE);   //获得标准输出设备句柄
	GetConsoleScreenBufferInfo(handle, &bInfo);  //获得窗口信息

	for (i = 0; i < rowCount; i++)
	{
		SetPosition(coordStart.X, coordStart.Y);
		//FillConsoleOutputCharacter(handle, ' ', GAME_WIDTH, coordStart, &written);
		FillConsoleOutputCharacter(handle, ' ', WIN_WIDTH, coordStart, &written);
		coordStart.Y++;
	}
}
