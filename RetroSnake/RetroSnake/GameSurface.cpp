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

void DrawBorder(int posXS, int posXE, int posYS, int posYE)
{
	for (int ri = posYS; ri <= posYE; ++ri)
	{
		SetPosition(posXS, ri);
		for (int ci = posXS; ci <= posXE; ++ci)
		{
			E_CellType cellType = (ri == posYS || ri == posYE || ci == posXS || ci == posXE) ? E_CellType::Land : E_CellType::None;
			cout << GameMap::ToString({ cellType });
		}
	}
}

void DrawHollowBorder(int posXS, int posXE, int posYS, int posYE)
{
	string cellString = GameMap::ToString({ E_CellType::Land });
	SetPosition(posXS, posYS);
	for (int ci = posXS; ci <= posXE; ++ci)
		cout << cellString;
	SetPosition(posXS, posYE);
	for (int ci = posXS; ci <= posXE; ++ci)
		cout << cellString;
	for (int ri = posYS + 1; ri < posYE; ++ri)
	{
		SetPosition(posXS, ri);
		cout << cellString;
		SetPosition(posXE, ri);
		cout << cellString;
	}
}

void UnfinishedSurface(int x, int y, DWORD millseconds, string text)
{
	SetColor(DEFAULT_COLOR);
	system("cls");
	SetPosition(x, y);
	cout << text;
	Sleep(millseconds);
	while (!IsKeyDown(VK_RETURN))
		continue;
}

void StartSurface(size_t &selectIndex)
{
	Msgs msgs = {
		{"开始游戏"},
		{"游戏设置"},
		{"地图编辑器"},
		{"退出游戏"}
	};
	SetColor(DEFAULT_COLOR);
	system("cls");
	DrawHollowBorder(0, 59, 0, 3);
	DrawHollowBorder(0, 59, 3, 40);
	SetPosition(21, 1);
	SetColor({ E_4BitColor::LCyan, DEFAULT_BACK_COLOR });
	cout << GAME_NAME;
	SetPosition(27, 2);
	SetColor({ E_4BitColor::LWhite, DEFAULT_BACK_COLOR });
	cout << GAME_VERSION;
	for (size_t i = 0; i < msgs.size(); ++i)
	{
		SetPosition(28, 20 + i);
		cout << msgs[i];
	}
	bool isUpdateUI = true;
	size_t minIndex = 0, maxIndex = msgs.size() - 1;
	while (!IsKeyDown(VK_RETURN))
	{
		if (isUpdateUI)
		{
			for (size_t i = 0; i < msgs.size(); ++i)
			{
				SetPosition(25, 20 + i);
				cout << (i == selectIndex ? "→_→" : "     ");
			}
		}
		isUpdateUI = true;
		if (IsKeyDown(VK_UP) || IsKeyDown('W'))
			selectIndex = selectIndex == minIndex ? maxIndex : --selectIndex;
		else if (IsKeyDown(VK_DOWN) || IsKeyDown('S'))
			selectIndex = selectIndex == maxIndex ? minIndex : ++selectIndex;
		else
			isUpdateUI = false;
	}
}

void OverSurface(const PlayerCtrl &winer, bool isWin)
{
	SetColor(DEFAULT_COLOR);
	DrawBorder(GAME_OVER_S_INDEXX, GAME_OVER_E_INDEXX, GAME_OVER_S_INDEXY, GAME_OVER_E_INDEXY);
	SetPosition(GAME_OVER_S_INDEXX + 13, GAME_OVER_S_INDEXY + 4);
	SetColor({ winer.get_Color(), DEFAULT_BACK_COLOR });
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
		int ci = GAME_MSG_S_INDEXX + (GAME_MSG_E_INDEXX - GAME_MSG_S_INDEXX - msg.size() / 2) / 2;
		SetPosition(ci, ri);
		cout << msg;
	}
}

void ShowMsg(const PlayerCtrl & player1, const PlayerCtrl & player2)
{
	if (&player1 == &player2)
	{
		ShowMsg({
			{ "" },
			{ "" },
			{ "玩家一" },
			{ "分数", player1.get_Score(), 3, '0' },
			{ "速度", player1.get_Speed(), 3, '0' },
			{ "刚体:", player1.get_BuffRemainSecond(E_BuffType::Unstoppable), 2, '0', "秒" },
			{ "失控:", player1.get_BuffRemainSecond(E_BuffType::Incontrollable), 2, '0', "秒" },
			{ "打滑:", player1.get_BuffRemainSecond(E_BuffType::Slippage), 2, '0', "秒" },
			{ "" },
			{ "" },
			{ "W,A,S,D操作玩家一" },
			{ "" },
			{ "空格键暂停游戏" },
			});
	}
	else
	{
		ShowMsg({
			{ "" },
			{ "" },
			{ "玩家一", "玩家二" },
			{ "分数", player1.get_Score(), player2.get_Score(), 3, '0' },
			{ "速度", player1.get_Speed(), player2.get_Speed(), 3, '0' },
			{ "刚体:", player1.get_BuffRemainSecond(E_BuffType::Unstoppable), player2.get_BuffRemainSecond(E_BuffType::Unstoppable), 2, '0', "秒" },
			{ "失控:", player1.get_BuffRemainSecond(E_BuffType::Incontrollable), player2.get_BuffRemainSecond(E_BuffType::Incontrollable), 2, '0', "秒" },
			{ "打滑:", player1.get_BuffRemainSecond(E_BuffType::Slippage), player2.get_BuffRemainSecond(E_BuffType::Slippage), 2, '0', "秒" },
			{ "" },
			{ "" },
			{ "W,A,S,D操作玩家一" },
			{ "↑,↓,←,→操作玩家二" },
			{ "空格键暂停游戏" },
			});
	}
}
