#include "GameSurface.hpp"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;

static constexpr auto GAME_MAP_STARTPOSY = 1;
static constexpr auto GAME_MAP_ENDPOSY = GAME_HEIGHT;
static constexpr auto GAME_MAP_STARTPOSX = 1;
static constexpr auto GAME_MAP_ENDPOSX = GAME_WIDTH;

static constexpr auto GAME_MAP_S_INDEXY = GAME_MAP_STARTPOSY - 1;
static constexpr auto GAME_MAP_E_INDEXY = GAME_MAP_ENDPOSY - 1;
static constexpr auto GAME_MAP_S_INDEXX = GAME_MAP_STARTPOSX - 1;
static constexpr auto GAME_MAP_E_INDEXX = GAME_MAP_ENDPOSX - 1;

static constexpr auto GAME_OVER_S_INDEXY = GAME_HEIGHT / 2 - 5;
static constexpr auto GAME_OVER_E_INDEXY = GAME_HEIGHT / 2 + 5;
static constexpr auto GAME_OVER_S_INDEXX = GAME_WIDTH / 2 - 20;
static constexpr auto GAME_OVER_E_INDEXX = GAME_WIDTH / 2 + 20;

static constexpr auto GAME_MSG_S_INDEXY = 1;
static constexpr auto GAME_MSG_E_INDEXY = 20;
static constexpr auto GAME_MSG_S_INDEXX = GAME_MAP_ENDPOSX + 1;
static constexpr auto GAME_MSG_E_INDEXX = WIN_WIDTH - 2;

static E_MapItem zCacheMap[GAME_WIDTH][GAME_HEIGHT];

inline void DrawBorder(int posXS, int posXE, int posYS, int posYE)
{
	for (int ri = posYS; ri <= posYE; ++ri)
	{
		SetPosition(posXS, ri);
		for (int ci = posXS; ci <= posXE; ++ci)
		{
			if (ri == posYS || ri == posYE || ci == posXS || ci == posXE)
				cout << "#";
			else
				cout << " ";
		}
	}
}

void InitSurface(Map &map)
{
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
	{
		SetPosition(GAME_MAP_STARTPOSX, GAME_MAP_STARTPOSY + ri);
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
		{
			E_MapItem item = (ri == GAME_MAP_S_INDEXY || ri == GAME_MAP_E_INDEXY || ci == GAME_MAP_S_INDEXX || ci == GAME_MAP_E_INDEXX)
				? E_MapItem::Wall : E_MapItem::None;
			zCacheMap[ci][ri] = map.Index(ci, ri) = item;
			cout << MapItems[(int)zCacheMap[ci][ri]];
		}
	}
	DrawBorder(GAME_MSG_S_INDEXX, GAME_MSG_E_INDEXX, GAME_MSG_S_INDEXY, GAME_MSG_E_INDEXY);
	ShowMsg({
		"玩家一        玩家二",
		"分数        分数",
		"速度        速度",
		});
}

void DrawMap(const Map &map)
{
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
	{
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
		{
			//if (map[0][ci][ri] == E_MapItem::Head)
			//	continue;
			if (zCacheMap[ci][ri] == map.Index(ci, ri))
				continue;
			zCacheMap[ci][ri] = map.Index(ci, ri);
			SetPosition(GAME_MAP_STARTPOSX + ci, GAME_MAP_STARTPOSY + ri);
			auto color = map.ColorIndex(ci, ri);
			SetColor(color.fore, color.back);
			cout << MapItems[(int)zCacheMap[ci][ri]];
		}
	}
	SetPosition(0, 0);
}

void OverSurface(bool isWin)
{
	SetColor(15, 0);
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	SetPosition(GAME_OVER_S_INDEXX + 17, GAME_OVER_S_INDEXY + 4);
	cout << (isWin ? "你赢了" : "你输了");
	SetPosition(GAME_OVER_S_INDEXX + 6, GAME_OVER_S_INDEXY + 5);
	cout << "输入q退出游戏，输入r重新开始";
}

void ShowMsg(Msgs &&msgs)
{
	int ri = GAME_MSG_S_INDEXY + 1;
	for (auto msg : msgs)
	{
		if (++ri >= (GAME_MSG_E_INDEXY - 1))
			break;
		int ci = GAME_MSG_S_INDEXX + (GAME_MSG_E_INDEXX - GAME_MSG_S_INDEXX - msg.size()) / 2;
		SetPosition(ci, ri);
		cout << msg;
	}
}