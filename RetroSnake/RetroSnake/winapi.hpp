#ifndef WINAPI_HPP_INCLUDED
#define WINAPI_HPP_INCLUDED

#include "GameModel.hpp"
#include <string>
#include <windows.h>

#include <Mmsystem.h>
#pragma comment(lib,"winmm.lib")

using std::string;

bool IsKey(int vKey);
bool IsKeyDown(int vKey);

void ResetCursor();

// Code from http://www.xuetang9.com
 // 设置控制台标题
void SetTitle(const char * title);
// 设置控制台颜色
void SetColor(Color);
// 根据宽高设置控制台窗口的大小
void SetConsoleWindowSize();
// 设置光标的x、y坐标-正常坐标轴(x左至右，y上至下)
void SetPosition(int x, int y);

// Code from https://blog.csdn.net/zuishikonghuan/article/details/47441163
string OpenFile(LPCSTR filter = "贪吃蛇地图文件(*.vrs)\0*.vrs\0\0");
string SaveFile(LPCSTR filter = "贪吃蛇地图文件(*.vrs)\0*.vrs\0\0");
//string OpenFile(string filter = "贪吃蛇地图文件\0*.vrs\0\0");
//string SaveFile(string filter = "贪吃蛇地图文件\0*.vrs\0\0");

#endif