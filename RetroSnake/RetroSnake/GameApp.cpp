#include "GameApp.hpp"
#include "GameLib.hpp"
#include "GameSurface.hpp"
#include "GameCtrl.hpp"
#include "vytTimer.hpp"

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
	GameMap map(playerCount, m_isUpdateUI);
	char c = '\0';
	while ('q' != c)
	{
		GameMain(map);
		while (_kbhit())
			_getch();
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
	}
}

void GameApp::GameMain(GameMap & map)
{
	map.Reset();
	bool isGameOver = false;
	bool isGamePause = false;
	m_isUpdateUI = true;
	while (!isGameOver)
	{
		if (m_isUpdateUI)
		{
			ShowMsg(map.GetPlayer(0), map.GetPlayer(1));
			m_isUpdateUI = false;
		}
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (isGamePause)
			continue;
		vyt::timer::get_instance().HandleClock();
		map.Draw();
		auto winer = map.CheckOver();
		if (nullptr != winer)
		{
			winer->IncreaseScore();
			OverSurface(*winer, true);
			isGameOver = true;
		}
	}
}
