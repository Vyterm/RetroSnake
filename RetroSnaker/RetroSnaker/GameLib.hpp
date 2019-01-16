#pragma once

#ifndef GAMELIB_H_INCLUDED
#define GAMELIB_H_INCLUDED

#include <windows.h>
#include <Windows.h>
#include <time.h>
#include "GameColor.hpp"

constexpr auto GAME_NAME = "贪吃蛇大作战(Console Version) by 郭弈天";
constexpr auto GAME_VERSION = "Version 0.7.4";

constexpr auto WIN_HEIGHT = 43;
constexpr auto WIN_WIDTH = 120;
constexpr auto GAME_HEIGHT = 40;
constexpr auto GAME_WIDTH = 40;
constexpr auto MSG_WIDTH = 20;
constexpr auto MSG_HEIGHT = 20;
constexpr auto MAZE_HEIGHT = 20;
constexpr auto MAZE_WIDTH = 20;

 /* 设置控制台窗口标题 */
void SetTitle(const char * title);
/* 0-黑色, 1-蓝色,   2-绿色,      3-浅绿色,     4-红色,   5-紫色,   6-黄色,   7-白色,
 * 8-灰色, 9-淡蓝色, 10-淡绿色,   11-淡浅绿色   12-淡红色 13-淡紫色 14-淡黄色 15-亮白色
 */
void SetColor(Color);

/* 根据宽高设置控制台窗口的大小 */
void SetConsoleWindowSize();
void ResetCursor();
/* 设置光标的x、y坐标-正常坐标轴 */
void SetPosition(int x, int y);

/* 动态打印文字信息 */
void PrintString(const char * msgString);

/* 类似fgets, 读取对应长度的字符串*/
void Read(char * Str, int BufferSize);

/* 得到用户输入的键 */
int GetKey();

/* 从起始坐标开始，清rowCount行 */
void ClearByCoord(int x, int y, int rowCount);

#endif // GAMELIB_H_INCLUDED
