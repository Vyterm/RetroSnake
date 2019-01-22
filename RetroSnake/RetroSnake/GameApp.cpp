#include "GameApp.hpp"
#include "GameSurface.hpp"
#include "GameCtrl.hpp"
#include "GameEditor.hpp"
#include "GameGraphic.hpp"
#include "vytTimer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::endl;

static LPCSTR BGAudioPath[] =
{
	"sound_bg1.wav",
	"sound_bg2.wav",
	"sound_bg3.wav",
	"sound_bg4.wav",
	"sound_bg5.wav",
};

int main()
{
	//game::RenderLayer render;
	//render.SetString({ 10,10 }, "Crazy", { 2,1,2,0 });
	//render.Draw();

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
	//PlaySound(TEXT("C:\\Project\\RetroSnake\\RetroSnake\\RetroSnake\\Debug\\background.wav"), NULL, SND_FILENAME | SND_ASYNC);
	PlaySound(TEXT(BGAudioPath[rand()%5]), NULL, SND_FILENAME | SND_ASYNC);
	size_t selectIndex = 0;
	StartSurface(selectIndex);
	switch (selectIndex)
	{
	case 0:
		Game();
		break;
	case 1:
		UnfinishedSurface(23, 20, 300, "游戏设置尚未完成，敬请期待");
		break;
	case 2:
		Editor();
		break;
	case 3:
		return false;
	}
	return true;
}

void GameApp::Game()
{
	GameMap map(m_isUpdateUI);

	GameMapModel reloadModel;
	string path = OpenFile();
	if ("" == path) return;
	std::ifstream ifs;
	ifs.open(path);
	ifs >> reloadModel;
	ifs.close();

	map.SetModel(reloadModel);
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
