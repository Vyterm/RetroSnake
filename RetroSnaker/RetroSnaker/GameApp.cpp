#include "GameApp.hpp"
#include "GameLib.hpp"
#include "GameSurface.hpp"
#include "GameModel.hpp"
#include "GameCtrl.hpp"

#include <iostream>
#include <iomanip>
#include <vector>

#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::endl;

constexpr Color FOOD_COLOR = { 12, 0 };
static constexpr auto SLEEP_DELTA = 10;

inline bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

inline bool GenerateRandomFood(Map &map)
{
	std::vector<Point> emptyPoints;
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
			if (E_CellType::None == map.Index(ci, ri))
				emptyPoints.push_back({ ci,ri });
	if (0 == emptyPoints.size())
		return false;
	auto point = emptyPoints[rand() % emptyPoints.size()];
	map.Index(point) = E_CellType::Food;
	map.ColorIndex(point) = FOOD_COLOR;
	return true;
}
static bool G_IsGameOver = false;

inline bool ProcessSnake(Map &map, PlayerCtrl &player)
{
	E_MoveState moveState = player.Process(SLEEP_DELTA);
	switch (moveState)
	{
	case E_MoveState::Over:
		OverSurface(player.get_Name(), player.get_Color(), false);
		G_IsGameOver = true;
		break;
	case E_MoveState::Eat:
		if (!GenerateRandomFood(map))
		{
			OverSurface(player.get_Name(), player.get_Color(), true);
			G_IsGameOver = true;
		}
		player.IncreaseScore();
		player.IncreaseSpeed();
		return true;
	case E_MoveState::Kill:
		OverSurface(player.get_Name(), player.get_Color(), true);
		G_IsGameOver = true;
		break;
	case E_MoveState::Done:
	default:
		break;
	}
	return false;
}

void Game()
{
	srand((unsigned)time(nullptr));
	bool isGamePause = false;
	Map map;
	InitSurface(map);
	PlayerCtrl player1("玩家一", map, { GAME_WIDTH / 2 - 5,GAME_HEIGHT / 2 }, { 10,0 }, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT);
	PlayerCtrl player2("玩家二", map, { GAME_WIDTH / 2 + 5,GAME_HEIGHT / 2 }, { 9, 0 }, 'W', 'A', 'S', 'D');
	int eatFoodCount = 0;
	GenerateRandomFood(map);
	GenerateRandomFood(map);
	GenerateRandomFood(map);
	ShowMsg(player1.get_Score(), player2.get_Score(), player1.get_Speed(), player2.get_Speed());
	while (!G_IsGameOver)
	{
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (isGamePause)
			continue;
		DrawMap(map);
		Sleep(SLEEP_DELTA);
		bool updateFlag = false;
		updateFlag |= ProcessSnake(map, player1);
		updateFlag |= ProcessSnake(map, player2);
		if (updateFlag)
			ShowMsg(player1.get_Score(), player2.get_Score(), player1.get_Speed(), player2.get_Speed());
	}
}

int main()
{
	SetTitle("贪吃蛇大作战(Console Version) by 郭弈天");
	SetConsoleWindowSize();

	char c = '\0';
	while ('q' != c)
	{
		G_IsGameOver = false;
		Game();
		fflush(stdin);
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
	}

	return 0;
}

void TestGetAsyncKeyState()
{
	int invokeCount = 0;
	while (true)
	{
		Sleep(80);
		SetPosition(0, 0);
		SHORT key;
		key = GetAsyncKeyState(VK_LEFT);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		key = GetAsyncKeyState(VK_RIGHT);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		key = GetAsyncKeyState(VK_UP);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		key = GetAsyncKeyState(VK_DOWN);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		cout << "InvokeCount : " << ++invokeCount << endl;
	}
}