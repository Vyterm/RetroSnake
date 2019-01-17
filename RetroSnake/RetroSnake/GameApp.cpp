#include "GameApp.hpp"
#include "GameSurface.hpp"
#include "GameCtrl.hpp"
#include "GameEditor.hpp"
#include "vytTimer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <iomanip>
#include <vector>

#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::endl;

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
		Editor();
		break;
	case 4:
		return false;
	}
	return true;
}

inline GameMapModel GenerateMapModel(size_t playerCount)
{
	GameMapModel m_model;
	m_model.SetHollowLand({ 0, 0 }, { (GAME_WIDTH + 0 - 1), (GAME_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand);
	m_model.SetHollowLand({ GAME_WIDTH, MSG_HEIGHT }, { (MAZE_WIDTH + GAME_WIDTH - 1), (MAZE_HEIGHT + MSG_HEIGHT - 1) }, E_StaticCellType::JebelLand);
	m_model.SetHollowLand({ GAME_WIDTH, 0 }, { (MSG_WIDTH + GAME_WIDTH - 1), (MSG_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand);
	m_model.SetCloseyLand({ 10, 1 }, { (20 + 10 - 1), (10 + 1 - 1) }, E_StaticCellType::GrassLand);
	m_model.SetCloseyLand({ 1, 38 }, { (38 + 1 - 1), (1 + 38 - 1) }, E_StaticCellType::MagmaLand);
	m_model.SetCloseyLand({ 1, 36 }, { (38 + 1 - 1), (2 + 36 - 1) }, E_StaticCellType::FrostLand);
	m_model.SetCross({ 5, 5 });
	m_model.SetCross({ 34, 5 });
	m_model.SetCross({ 5, 34 });
	m_model.SetCross({ 34, 34 });
	if (playerCount == 1)
	{
		m_model.SetPlayer({ GAME_WIDTH / 2, GAME_HEIGHT / 2 }, E_Color::LCyan);
		m_model.SetJumpPoint({ 20, 18 }, { 50, 30 }, E_Color::LBlue);
		m_model.SetJumpPoint({ 20, 30 }, { 50, 35 }, E_Color::LPurple);
	}
	else
	{
		m_model.SetPlayer({ GAME_WIDTH / 2 - 5, GAME_HEIGHT / 2 }, E_Color::LCyan);
		m_model.SetPlayer({ GAME_WIDTH / 2 + 4, GAME_HEIGHT / 2 }, E_Color::LWhite);
		m_model.SetJumpPoint({ 19, 18 }, { 49, 30 }, E_Color::LBlue);
		m_model.SetJumpPoint({ 20, 18 }, { 50, 30 }, E_Color::LBlue);
		m_model.SetJumpPoint({ 19, 30 }, { 49, 35 }, E_Color::LPurple);
		m_model.SetJumpPoint({ 20, 30 }, { 50, 35 }, E_Color::LPurple);
	}
	m_model.set_FoodCount(3);
	return m_model;
}

void GameApp::Game(size_t playerCount)
{
	GameMap map(m_isUpdateUI);
	map.SetModel(GenerateMapModel(playerCount));
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

void GameApp::Editor()
{
	GameEditor editor;
	editor.Run();
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
