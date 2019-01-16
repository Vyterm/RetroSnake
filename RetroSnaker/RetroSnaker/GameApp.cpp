#include "GameApp.hpp"
#include "GameLib.hpp"
#include "GameSurface.hpp"
#include "GameModel.hpp"
#include "GameCtrl.hpp"
#include "PointerVector.hpp"
#include "GameTimer.hpp"

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

constexpr Color JUMP_COLOR = { E_Color::LBlue, DEFAULT_BACK_COLOR };
constexpr Color EXIT_COLOR = { E_Color::LPurple, DEFAULT_BACK_COLOR };

inline bool IsKey(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) == 0x8000;
}

inline bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

int main()
{
	GameApp app;
	app.Run();
	//GameMapModel mapModel(GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT);

	return 0;
}

GameApp::GameApp() : m_isUpdateUI(false)
{
}

void GameApp::Run()
{
	SetTitle(GAME_NAME);
	SetConsoleWindowSize();
	ResetCursor();

	while (Home())
		continue;
}

bool GameApp::Home()
{
	size_t selectIndex = 0;
	StartSurface(selectIndex);
	switch (selectIndex)
	{
	case 0:
		Game(1);
		break;
	case 1:
		Game(2);
		break;
	case 2:
		UnfinishedSurface(23, 20, 300, "游戏设置尚未完成，敬请期待");
		break;
	case 3:
		// ToDo : Editor
		UnfinishedSurface(22, 20, 300, "地图编辑器尚未完成，敬请期待");
		break;
	case 4:
		return false;
	}
	return true;
}

void GameApp::Game(size_t playerCount)
{
	GameMap map;
	PlayerCtrl player1("玩家一", map, m_isUpdateUI, { E_Color::LCyan, DEFAULT_BACK_COLOR }, 'W', 'A', 'S', 'D');
	PlayerCtrl player2("玩家二", map, m_isUpdateUI, { E_Color::LWhite,DEFAULT_BACK_COLOR }, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT);
	char c = '\0';
	while ('q' != c)
	{
		GameInit(map, player1, playerCount == 2 ? player2 : player1);
		GameMain(map, player1, playerCount == 2 ? player2 : player1);
		while (_kbhit())
			_getch();
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
	}
}

void GameApp::GameInit(GameMap & map, PlayerCtrl & player1, PlayerCtrl & player2)
{
	srand((unsigned)time(nullptr));
	m_isUpdateUI = false;
	SetColor(DEFAULT_COLOR);
	system("cls");
	map.Init();
	if (&player1 == &player2)
	{
		player1.Reset({ GAME_WIDTH / 2,GAME_HEIGHT / 2 });
		player1.SetEnemy(player1);
	}
	else
	{
		player1.Reset({ GAME_WIDTH / 2 - 5,GAME_HEIGHT / 2 });
		player2.Reset({ GAME_WIDTH / 2 + 5,GAME_HEIGHT / 2 });
		player1.SetEnemy(player2);
		player2.SetEnemy(player1);
	}
	map.GenerateRandomFood(3);
	map.GenerateEntryPoint(JUMP_COLOR);
	map.GenerateEntryPoint(EXIT_COLOR);
	map.Draw();
	ShowMsg(player1, player2);
}

void GameApp::GameMain(GameMap & map, PlayerCtrl & player1, PlayerCtrl & player2)
{
	bool isGameOver = false;
	bool isGamePause = false;
	while (!isGameOver)
	{
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (isGamePause)
			continue;
		TimerManager::get_instance().HandleClock();
		if (m_isUpdateUI)
		{
			ShowMsg(player1, player2);
			m_isUpdateUI = false;
		}
		map.Draw();
		GameOver(player1, player2, isGameOver);
	}
}

void GameApp::GameOver(PlayerCtrl & player1, PlayerCtrl & player2, bool & isGameOver)
{
	PlayerCtrl *winer = !player1.IsAlive() ? &player2 : !player2.IsAlive() ? &player1 : nullptr;
	if (nullptr == winer) return;
	winer->IncreaseScore();
	OverSurface(*winer, true);
	isGameOver = true;
	player1.Clear();
	player2.Clear();
	TimerManager::get_instance().HandleClock();
}
