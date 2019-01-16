#include "GameSurface.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

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

static Map zCachemap;

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

inline void SetTree(Map &map, int treeX, int treeY)
{
	map.Index(treeX - 2, treeY) = map.Index(treeX - 1, treeY) = map.Index(treeX, treeY) = map.Index(treeX + 1, treeY)
		= map.Index(treeX + 2, treeY) = map.Index(treeX, treeY - 1) = map.Index(treeX, treeY + 1) = E_CellType::Wall;
	map.ColorIndex(treeX - 2, treeY) = map.ColorIndex(treeX - 1, treeY) = map.ColorIndex(treeX, treeY) = map.ColorIndex(treeX + 1, treeY)
		= map.ColorIndex(treeX + 2, treeY) = map.ColorIndex(treeX, treeY - 1) = map.ColorIndex(treeX, treeY + 1) = { 8, 0 };
}

void InitSurface(Map &map)
{
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
	{
		SetPosition(GAME_MAP_STARTPOSX, GAME_MAP_STARTPOSY + ri);
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
		{
			E_CellType item = (ri == GAME_MAP_S_INDEXY || ri == GAME_MAP_E_INDEXY || ci == GAME_MAP_S_INDEXX || ci == GAME_MAP_E_INDEXX)
				? E_CellType::Wall : E_CellType::None;
			zCachemap.Index(ci, ri) = map.Index(ci, ri) = item;
			cout << MapItems[(int)zCachemap.Index(ci, ri)];
		}
	}
	DrawBorder(GAME_MSG_S_INDEXX, GAME_MSG_E_INDEXX, GAME_MSG_S_INDEXY, GAME_MSG_E_INDEXY);
	SetTree(map, 10, 5);
	SetTree(map, 68, 5);
	SetTree(map, 10, 34);
	SetTree(map, 68, 34);
}

void DrawMap(const Map &map)
{
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
	{
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
		{
			//if (map.Index(0][ci][ri) == E_MapItem::Head)
			//	continue;
			if (zCachemap.Index(ci, ri) == map.Index(ci, ri))
				continue;
			zCachemap.Index(ci, ri) = map.Index(ci, ri);
			SetPosition(GAME_MAP_STARTPOSX + ci, GAME_MAP_STARTPOSY + ri);
			auto color = map.ColorIndex(ci, ri);
			SetColor(color.fore, color.back);
			cout << MapItems[(int)zCachemap.Index(ci, ri)];
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