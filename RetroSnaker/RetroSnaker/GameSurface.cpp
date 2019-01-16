#include "GameSurface.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;

static constexpr auto POSITION_TOARRAY_OFFSET = 1;

static constexpr auto GAME_MAP_S_INDEXY = 1;
static constexpr auto GAME_MAP_E_INDEXY = GAME_HEIGHT;
static constexpr auto GAME_MAP_S_INDEXX = 1;
static constexpr auto GAME_MAP_E_INDEXX = GAME_WIDTH;

static constexpr auto GAME_OVER_S_INDEXY = GAME_HEIGHT / 2 - 5;
static constexpr auto GAME_OVER_E_INDEXY = GAME_HEIGHT / 2 + 5;
static constexpr auto GAME_OVER_S_INDEXX = GAME_WIDTH / 2 - 20;
static constexpr auto GAME_OVER_E_INDEXX = GAME_WIDTH / 2 + 20;

static constexpr auto GAME_MSG_S_INDEXY = 1;
static constexpr auto GAME_MSG_E_INDEXY = 20;
static constexpr auto GAME_MSG_S_INDEXX = GAME_MAP_E_INDEXX + 1;
static constexpr auto GAME_MSG_E_INDEXX = WIN_WIDTH - 2;

static constexpr auto GAME_MAZE_S_INDEXY = 21;
static constexpr auto GAME_MAZE_E_INDEXY = 40;
static constexpr auto GAME_MAZE_S_INDEXX = GAME_MSG_S_INDEXX;
static constexpr auto GAME_MAZE_E_INDEXX = GAME_MSG_E_INDEXX;

static Map zCachemap;

inline void DrawBorder(int posXS, int posXE, int posYS, int posYE, Map *map = nullptr)
{
	for (int ri = posYS; ri <= posYE; ++ri)
	{
		SetPosition(posXS, ri);
		for (int ci = posXS; ci <= posXE; ++ci)
		{
			E_CellType cellType = (ri == posYS || ri == posYE || ci == posXS || ci == posXE) ? E_CellType::Wall : E_CellType::None;
			cout << char(cellType);
			if (nullptr != map)
				zCachemap.Index(ci - POSITION_TOARRAY_OFFSET, ri - POSITION_TOARRAY_OFFSET)
				= map->Index(ci - POSITION_TOARRAY_OFFSET, ri - POSITION_TOARRAY_OFFSET) = cellType;
		}
	}
}

inline void SetTree(Map &map, int treeX, int treeY)
{
	map.Index(treeX - 2, treeY) = map.Index(treeX - 1, treeY) = map.Index(treeX, treeY) = map.Index(treeX + 1, treeY)
		= map.Index(treeX + 2, treeY) = map.Index(treeX, treeY - 1) = map.Index(treeX, treeY + 1) = E_CellType::Wall;
	map.ColorIndex(treeX - 2, treeY) = map.ColorIndex(treeX - 1, treeY) = map.ColorIndex(treeX, treeY) = map.ColorIndex(treeX + 1, treeY)
		= map.ColorIndex(treeX + 2, treeY) = map.ColorIndex(treeX, treeY - 1) = map.ColorIndex(treeX, treeY + 1) = { 8, 0 };
}

void InitSurface(Map &map)
{
	DrawBorder(GAME_MAP_S_INDEXX, GAME_MAP_E_INDEXX, GAME_MAP_S_INDEXY, GAME_MAP_E_INDEXY, &map);
	DrawBorder(GAME_MSG_S_INDEXX, GAME_MSG_E_INDEXX, GAME_MSG_S_INDEXY, GAME_MSG_E_INDEXY);
	DrawBorder(GAME_MAZE_S_INDEXX, GAME_MAZE_E_INDEXX, GAME_MAZE_S_INDEXY, GAME_MAZE_E_INDEXY, &map);
	SetTree(map, 10, 5);
	SetTree(map, 68, 5);
	SetTree(map, 10, 34);
	SetTree(map, 68, 34);
}

void DrawMap(const Map &map)
{
	for (int ri = 0; ri < 60; ++ri)
	{
		for (int ci = 0; ci < 120; ++ci)
		{
			if (zCachemap.Index(ci, ri) == map.Index(ci, ri))
				continue;
			zCachemap.Index(ci, ri) = map.Index(ci, ri);
			SetPosition(GAME_MAP_S_INDEXX + ci, GAME_MAP_S_INDEXY + ri);
			auto color = map.ColorIndex(ci, ri);
			SetColor(color.fore, color.back);
			cout << char(zCachemap.Index(ci, ri));
		}
	}
	SetPosition(0, 0);
}

void OverSurface(string playerName, Color playerColor, bool isWin)
{
	SetColor(15, 0);
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	SetPosition(GAME_OVER_S_INDEXX + 17, GAME_OVER_S_INDEXY + 4);
	SetColor(playerColor.fore, playerColor.back);
	cout << playerName;
	SetColor(15, 0);
	cout << (isWin ? "胜利" : "失败");
	SetPosition(GAME_OVER_S_INDEXX + 6, GAME_OVER_S_INDEXY + 5);
	cout << "输入q退出游戏，输入r重新开始";
}

void ShowMsg(int player1Score, int player2Score, int player1Speed, int player2Speed)
{
	const string PLAYER_SPACE = "        ";
	Msgs msgs;
	std::ostringstream oss;
	oss << "玩家一" << PLAYER_SPACE << "玩家二";
	msgs.push_back(oss.str());
	oss.str("");
	oss << "分数" << std::setw(3) << std::setfill('0') << player1Score << PLAYER_SPACE << "分数" << std::setw(3) << std::setfill('0') << player2Score;
	msgs.push_back(oss.str());
	oss.str("");
	oss << "速度" << std::setw(3) << std::setfill('0') << player1Speed << PLAYER_SPACE << "速度" << std::setw(3) << std::setfill('0') << player2Speed;
	msgs.push_back(oss.str());
	oss.str("");
	ShowMsg(std::move(msgs));
}

void ShowMsg(Msgs &&msgs)
{
	SetColor(7, 0);
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