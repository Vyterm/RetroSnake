#include "GameSurface.hpp"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;

static constexpr auto GAME_MAP_STARTPOSY = 1;
static constexpr auto GAME_MAP_ENDPOSY = 60;
static constexpr auto GAME_MAP_STARTPOSX = 1;
static constexpr auto GAME_MAP_ENDPOSX = 80;

static constexpr auto GAME_MAP_S_INDEXY = 0;
static constexpr auto GAME_MAP_E_INDEXY = 59;
static constexpr auto GAME_MAP_S_INDEXX = 0;
static constexpr auto GAME_MAP_E_INDEXX = 79;

static constexpr auto GAME_OVER_S_INDEXY = 20;
static constexpr auto GAME_OVER_E_INDEXY = 39;
static constexpr auto GAME_OVER_S_INDEXX = 0;
static constexpr auto GAME_OVER_E_INDEXX = 79;

static E_MapItem zCacheMap[GAME_WIDTH][GAME_HEIGHT];

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
	for (int ri = GAME_OVER_S_INDEXY; ri <= GAME_OVER_E_INDEXY; ++ri)
	{
		SetPosition(GAME_MAP_STARTPOSX, GAME_MAP_STARTPOSY + ri);
		for (int ci = GAME_OVER_S_INDEXX; ci < GAME_OVER_E_INDEXX; ++ci)
		{
			if (ri == GAME_MAP_S_INDEXY || ri == GAME_MAP_E_INDEXY || ci == GAME_MAP_S_INDEXX || ci == GAME_MAP_E_INDEXX)
				cout << "#";
			else
				cout << " ";
		}
	}
	SetPosition(10, 30);
	cout << (isWin ? "你赢了" : "你输了");
	SetPosition(10, 31);
	cout << "输入q退出游戏，输入r重新开始";
}
