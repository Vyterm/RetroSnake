#ifndef GAME_SURFACE_HPP
#define GAME_SURFACE_HPP

#include "GameLib.hpp"
#include "GameCtrl.hpp"

#include <vector>
#include <string>
#include <sstream>
using std::string;

class SurfaceText
{
	string m_text;
	const string PLAYER_SPACE = "        ";
public:
	SurfaceText(string text) : m_text(text) { }
	SurfaceText(string player1, string player2)
	{
		std::ostringstream oss;
		oss << player1 << PLAYER_SPACE << player2;
		m_text = oss.str();
	}
	template <typename T>
	SurfaceText(string prefix, T player1, T player2, std::streamsize width, char fill)
	{
		std::ostringstream oss;
		oss << prefix << std::setw(width) << std::setfill(fill) << player1 << PLAYER_SPACE << prefix << std::setw(width) << std::setfill(fill) << player2;
		m_text = oss.str();
	}
	size_t size()const { return m_text.size(); }
	friend std::ostream& operator<<(std::ostream &os, const SurfaceText& text)
	{
		os << text.m_text;
		return os;
	}
};
using Msgs = std::vector<SurfaceText>;

void InitSurface(Map &map);
void DrawMap(Map &map);
void OverSurface(string playerName, Color playerColor, bool isWin);

void ShowMsg(Msgs &&msgs);

inline void ShowMsg(int player1Score, int player2Score, const PlayerCtrl &player1, const PlayerCtrl &player2)
{
	ShowMsg({
		{ "" },
		{ "" },
		{ "玩家一", "玩家二" },
		{ "分数", player1Score, player2Score, 3, '0' },
		{ "速度", player1.get_Speed(), player2.get_Speed(), 3, '0' },
		{ "" },
		{ "" },
		{ "" },
		{ "" },
		{ "" },
		{ "↑,↓,←,→操作玩家一" },
		{ "W,A,S,D操作玩家二" },
		{ "空格键暂停游戏" },
		});
}

#endif