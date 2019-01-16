#pragma once

#include "GameCtrl.hpp"

#include <Windows.h>
#include <stdlib.h>

class GameApp
{
	bool m_isGameOver;
	bool m_isUpdateUI;
	Map m_map;
	PlayerCtrl m_player1, m_player2;
public:
	GameApp() : m_isGameOver(false), m_isUpdateUI(false), m_map(), 
		m_player1("玩家一", m_map, m_isUpdateUI, { E_Color::LWhite,DEFAULT_BACK_COLOR }, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT),
		m_player2("玩家二", m_map, m_isUpdateUI, { E_Color::LCyan, DEFAULT_BACK_COLOR }, 'W', 'A', 'S', 'D')
	{
		m_player1.SetEnemy(m_player2);
		m_player2.SetEnemy(m_player1);
	}
	void Run();
	void Game();
	void Reset();
	void CheckOver();
};

int main();