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
	void Game(size_t playerCount);
};

int main();