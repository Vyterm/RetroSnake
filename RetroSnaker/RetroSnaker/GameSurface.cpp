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

void OverSurface(const PlayerCtrl &winer, bool isWin)
{
	SetColor(DEFAULT_COLOR);
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	SetPosition(GAME_OVER_S_INDEXX + 13, GAME_OVER_S_INDEXY + 4);
	SetColor(winer.get_Color());
	cout << winer.get_Name();
	SetColor(DEFAULT_COLOR);
	cout << (isWin ? "胜利" : "失败");
	SetPosition(GAME_OVER_S_INDEXX + 8, GAME_OVER_S_INDEXY + 5);
	cout << "输入q退出游戏，输入r重新开始";
}

void ShowMsg(Msgs && msgs)
{
	SetColor(DEFAULT_COLOR);
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
