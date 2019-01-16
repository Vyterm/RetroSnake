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

static E_MapItem zCacheMap[GAME_WIDTH][GAME_HEIGHT];

void InitSurface(E_MapItem(*map)[GAME_WIDTH][GAME_HEIGHT])
{
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
	{
		SetPosition(GAME_MAP_STARTPOSX, GAME_MAP_STARTPOSY + ri);
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
		{
			E_MapItem item = (ri == GAME_MAP_S_INDEXY || ri == GAME_MAP_E_INDEXY || ci == GAME_MAP_S_INDEXX || ci == GAME_MAP_E_INDEXX)
				? E_MapItem::Wall : E_MapItem::None;
			zCacheMap[ci][ri] = map[0][ci][ri] = item;
			cout << MapItems[(int)zCacheMap[ci][ri]];
		}
	}
}

void DrawMap(const E_MapItem(*map)[GAME_WIDTH][GAME_HEIGHT])
{
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
	{
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
		{
			//if (map[0][ci][ri] == E_MapItem::Head)
			//	continue;
			if (zCacheMap[ci][ri] == map[0][ci][ri])
				continue;
			zCacheMap[ci][ri] = map[0][ci][ri];
			SetPosition(GAME_MAP_STARTPOSX + ci, GAME_MAP_STARTPOSY + ri);
			cout << MapItems[(int)zCacheMap[ci][ri]];
		}
	}
	SetPosition(0, 0);
}
