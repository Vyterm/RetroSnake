#pragma once
#include "GameLib.hpp"
#include "GameModel.hpp"

#include <vector>
#include <string>
using std::string;
using Msgs = std::vector<string>;

void InitSurface(Map &map);
void DrawMap(const Map &map);
void OverSurface(string playerName, Color playerColor, bool isWin);

void ShowMsg(int player1Score, int player2Score, int player1Speed, int player2Speed);
void ShowMsg(Msgs &&msgs);