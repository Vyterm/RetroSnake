#pragma once

#include "GameCtrl.hpp"

#include <Windows.h>
#include <stdlib.h>

class GameApp
{
	bool m_isUpdateUI;

	void GameInit(GameMap &map, PlayerCtrl &player1, PlayerCtrl &player2);
	void GameMain(GameMap &map, PlayerCtrl &player1, PlayerCtrl &player2);
	void GameOver(PlayerCtrl &player1, PlayerCtrl &player2, bool &isGameOver);
public:
	GameApp();
	void Run();
	bool Home();
	void Game(size_t playerCount);
};

int main();