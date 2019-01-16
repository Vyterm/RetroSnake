#include "GameSurface.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;

static constexpr auto GAME_OVER_S_INDEXY = GAME_HEIGHT / 2 - 5;
static constexpr auto GAME_OVER_E_INDEXY = GAME_HEIGHT / 2 + 5;
static constexpr auto GAME_OVER_S_INDEXX = GAME_WIDTH / 2 - 15;
static constexpr auto GAME_OVER_E_INDEXX = GAME_WIDTH / 2 + 15;

static constexpr auto GAME_MSG_S_INDEXY = 0;
static constexpr auto GAME_MSG_E_INDEXY = MSG_HEIGHT;
static constexpr auto GAME_MSG_S_INDEXX = GAME_WIDTH + 1;
static constexpr auto GAME_MSG_E_INDEXX = GAME_WIDTH + MAZE_WIDTH - 1;

inline void DrawBorder(int posXS, int posXE, int posYS, int posYE, Map *map = nullptr)
{
	for (int ri = posYS; ri <= posYE; ++ri)
	{
		SetPosition(posXS, ri);
		for (int ci = posXS; ci <= posXE; ++ci)
		{
			E_CellType cellType = (ri == posYS || ri == posYE || ci == posXS || ci == posXE) ? E_CellType::Wall : E_CellType::None;
			cout << Map::ToString({ cellType });
		}
	}
}

inline void SetTree(Map &map, int treeX, int treeY)
{
	//map.Index(treeX - 2, treeY) = map.Index(treeX - 1, treeY) = map.Index(treeX, treeY) = map.Index(treeX + 1, treeY)
	//	= map.Index(treeX + 2, treeY) = map.Index(treeX, treeY - 1) = map.Index(treeX, treeY + 1) = E_CellType::Wall;
	//map.Index(treeX - 2, treeY) = map.Index(treeX - 1, treeY) = map.Index(treeX, treeY) = map.Index(treeX + 1, treeY)
	//	= map.Index(treeX + 2, treeY) = map.Index(treeX, treeY - 1) = map.Index(treeX, treeY + 1) = { 8, 0 };
	map.Index(treeX - 1, treeY) = map.Index(treeX, treeY) = map.Index(treeX + 1, treeY)
		= map.Index(treeX, treeY - 1) = map.Index(treeX, treeY + 1) = E_CellType::Wall;
	map.Index(treeX - 1, treeY) = map.Index(treeX, treeY) = map.Index(treeX + 1, treeY)
		= map.Index(treeX, treeY - 1) = map.Index(treeX, treeY + 1) = { 8, 0 };
}

void InitSurface(Map &map)
{
	map.Init(0, GAME_WIDTH, 0, GAME_HEIGHT);
	map.Init(GAME_WIDTH, MAZE_WIDTH, MSG_HEIGHT, MAZE_HEIGHT);
	map.Init(GAME_WIDTH, MSG_WIDTH, 0, MSG_HEIGHT);
	SetTree(map, 5, 5);
	SetTree(map, 34, 5);
	SetTree(map, 5, 34);
	SetTree(map, 34, 34);
	map.Draw(true);
}

void DrawMap(Map &map)
{
	map.Draw();
}

void OverSurface(string playerName, Color playerColor, bool isWin)
{
	SetColor(15, 0);
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	SetPosition(GAME_OVER_S_INDEXX + 13, GAME_OVER_S_INDEXY + 4);
	SetColor(playerColor.fore, playerColor.back);
	cout << playerName;
	SetColor(15, 0);
	cout << (isWin ? "胜利" : "失败");
	SetPosition(GAME_OVER_S_INDEXX + 8, GAME_OVER_S_INDEXY + 5);
	cout << "输入q退出游戏，输入r重新开始";
}

void ShowMsg(Msgs && msgs)
{
	SetColor(7, 0);
	int ri = GAME_MSG_S_INDEXY + 1;
	for (auto msg : msgs)
	{
		if (++ri >= (GAME_MSG_E_INDEXY - 1))
			break;
		int ci = GAME_MSG_S_INDEXX + (GAME_MSG_E_INDEXX - GAME_MSG_S_INDEXX - msg.size()/2) / 2;
		SetPosition(ci, ri);
		cout << msg;
	}
}
