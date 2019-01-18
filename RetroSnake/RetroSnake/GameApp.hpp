#pragma once

#include "GameCtrl.hpp"

class GameApp
{
	bool m_isUpdateUI;

	void GameMain(GameMap &map);
public:
	GameApp();
	void Run();
	bool Home();
	void Game();
	void Editor();
};

int main();