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

	return 0;
}

void GameApp::Run()
{
	SetTitle("贪吃蛇大作战(Console Version) by 郭弈天");
	SetConsoleWindowSize();
	ResetCursor();

	char c = '\0';
	while ('q' != c)
	{
		Reset();
		Game();
		while (_kbhit())
			_getch();
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
	}
}

void GameApp::Game()
{
	bool isGamePause = false;
	while (!m_isGameOver)
	{
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (isGamePause)
			continue;
		TimerManager::get_instance().HandleClock();
		if (m_isUpdateUI)
		{
			ShowMsg(m_player1, m_player2);
			m_isUpdateUI = false;
		}
		m_map.Draw();
		//Sleep(SLEEP_DELTA);
		CheckOver();
	}
}

void GameApp::Reset()
{
	srand((unsigned)time(nullptr));
	m_isGameOver = false;
	m_isUpdateUI = false;
	SetColor(DEFAULT_COLOR);
	system("cls");
	m_map.Init();
	m_player1.Reset({ GAME_WIDTH / 2 + 5,GAME_HEIGHT / 2 });
	m_player2.Reset({ GAME_WIDTH / 2 - 5,GAME_HEIGHT / 2 });
	m_map.GenerateRandomFood(3);
	m_map.GenerateEntryPoint(JUMP_COLOR);
	m_map.GenerateEntryPoint(EXIT_COLOR);
	m_map.Draw();
	ShowMsg(m_player1, m_player2);
}

void GameApp::CheckOver()
{
	PlayerCtrl *winer = !m_player1.IsAlive() ? &m_player2 : !m_player2.IsAlive() ? &m_player1 : nullptr;
	if (nullptr == winer) return;
	winer->IncreaseScore();
	OverSurface(*winer, true);
	m_isGameOver = true;
	m_player1.ClearBuff();
	m_player2.ClearBuff();
	TimerManager::get_instance().HandleClock();
}
