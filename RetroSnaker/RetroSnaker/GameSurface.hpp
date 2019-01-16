#pragma once
#include "GameLib.hpp"
#include "GameModel.hpp"

const char MapItems[5] = { ' ', '#', 'a', '@', 'O' };

void InitSurface(Map &map);
void DrawMap(const Map &map);
void OverSurface(bool isWin);