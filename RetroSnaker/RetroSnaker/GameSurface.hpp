#pragma once
#include "GameLib.hpp"

enum class E_MapItem
{
	None = 0,
	Wall = 1,
	Food = 2,
	Head = 3,
	Body = 4,
};

const char MapItems[5] = { ' ', '#', 'a', '@', 'O' };

void InitSurface(E_MapItem(*map)[GAME_WIDTH][GAME_HEIGHT]);
void DrawMap(const E_MapItem(*map)[GAME_WIDTH][GAME_HEIGHT]);