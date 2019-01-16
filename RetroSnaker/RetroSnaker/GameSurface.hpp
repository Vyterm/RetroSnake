#pragma once
#include "GameLib.hpp"
#include "GameModel.hpp"

#include <vector>
#include <string>
using std::string;
using Msgs = std::vector<string>;

const char MapItems[5] = { ' ', '#', 'a', '@', 'O' };

void InitSurface(Map &map);
void DrawMap(const Map &map);
void OverSurface(bool isWin);

void ShowMsg(Msgs &&msgs);